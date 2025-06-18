# make_bin_file_hybrid.py

import struct
import hashlib
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto.Random import get_random_bytes
import os
import time

# --- Google Cloud Secret Manager 관련 임포트 ---
from google.cloud import secretmanager 
# ---

# === 설정값 ===
ECU_ID = 0x03
VERSION = 0x03
MAGIC = 0xDEADBEEF
TIMESTAMP = int(time.time())

# firmwae.bin 파일을 현재 디렉토리에서 읽습니다.
# 이 파일은 OTA 업데이트 대상이 되는 실제 펌웨어 바이너리입니다.
try:
    with open("firmware.bin", "rb") as f:
        CODE = f.read()
except FileNotFoundError:
    print("❌ 'firmware.bin' 파일을 찾을 수 없습니다. 현재 디렉토리에 펌웨어 원본 파일을 넣어주세요.")
    exit(1)
CODE_LEN = len(CODE)

# === 개인키/공개키 로딩 ===
# 서버는 서명을 위해 '자신의 개인키'를, 키 암호화를 위해 '차량의 공개키'를 사용합니다.

# 1. Secret Manager에서 서버의 개인키 불러오기
# TODO: 여기에 당신의 GCP 프로젝트 ID를 입력하세요!
project_id = "thematic-grin-463106-m2" 
secret_id = "ota-server-private-key" # Secret Manager에 저장한 비밀 이름

server_private_key = None # 초기화

try:
    print("🌍 Secret Manager에서 서버 개인키를 로드 시도 중...")
    client = secretmanager.SecretManagerServiceClient()
    # 최신 버전의 비밀에 접근
    name = f"projects/{project_id}/secrets/{secret_id}/versions/latest"
    response = client.access_secret_version(request={"name": name})
    server_private_key_pem = response.payload.data.decode("utf-8")
    server_private_key = RSA.import_key(server_private_key_pem)
    print("✅ 서버 개인키를 Secret Manager에서 성공적으로 로드했습니다.")
except Exception as e:
    print(f"❌ Secret Manager에서 서버 개인키를 로드하는 데 실패했습니다: {e}")
    print("❗ 대체: 로컬 파일 'private.pem'에서 개인키를 로드합니다. 이 방식은 운영 환경에서 권장되지 않습니다.")
    if not os.path.exists("private.pem"):
        print("❌ 로컬 'private.pem' 파일도 찾을 수 없습니다. 개인키 없이는 펌웨어를 생성할 수 없습니다.")
        exit(1)
    with open("private.pem", "rb") as f:
        server_private_key = RSA.import_key(f.read())
        print("✅ 로컬 'private.pem'에서 개인키를 로드했습니다. (보안 경고!)")

# 2. 차량의 공개키 로딩 (현재는 파일에서 로드)
# TODO: 실제 운영 환경에서는 차량별 공개키를 데이터베이스에서 조회하도록 변경해야 합니다.
vehicle_public_key = None # 초기화
public_pem_path = "public.pem" # 클라이언트용 공개키 파일 경로

if not os.path.exists(public_pem_path):
    print(f"❌ '{public_pem_path}' 파일을 찾을 수 없습니다. 차량의 공개키 파일이 있어야 합니다.")
    exit(1)
with open(public_pem_path, "rb") as f:
    # 이 public.pem은 펌웨어를 수신할 차량(ECU)의 공개키입니다.
    # 실제 시스템에서는 각 차량의 고유한 공개키를 사용하고, 서버는 이를 DB 등으로 관리합니다.
    vehicle_public_key = RSA.import_key(f.read())
print(f"✅ '{public_pem_path}'에서 차량의 공개키를 로드했습니다.")


# === 펌웨어 해시 (무결성 검증용) ===
hash_obj = SHA256.new(CODE)
firmware_hash = hash_obj.digest() # 32 bytes

# === 전자 서명 (인증) ===
# 서버의 개인키로 펌웨어 해시에 서명합니다.
signer = pkcs1_15.new(server_private_key)
signature = signer.sign(hash_obj) # 256 bytes (RSA2048)

# === AES 암호화 (기밀성) ===
# 펌웨어는 매번 새로운 임시(세션) AES 키로 암호화합니다.
aes_key = get_random_bytes(16)  # 128bit 임시 세션 키
aes_nonce = get_random_bytes(8)  # CTR용 nonce
cipher_aes = AES.new(aes_key, AES.MODE_CTR, nonce=aes_nonce)
encrypted_code = cipher_aes.encrypt(CODE)

# === [핵심 변경] AES 키를 차량의 공개키로 암호화 ===
# 위에서 생성한 임시 AES 키를 '차량의 공개키'를 사용해 암호화합니다.
# 이렇게 하면 오직 해당 차량의 개인키를 가진 장치만이 이 AES 키를 풀 수 있습니다.
cipher_rsa = PKCS1_OAEP.new(vehicle_public_key)
encrypted_aes_key = cipher_rsa.encrypt(aes_key) # 256 bytes (RSA-2048)

# === 헤더 구성: <I I B B H 32s 8s 256s 256s
#   - Magic:           4바이트 (시작 식별자)
#   - Timestamp:       4바이트 (최신성 검증용)
#   - ECU ID:          1바이트 (대상 ECU 식별자)
#   - Version:         1바이트 (펌웨어 버전)
#   - Code Len:        2바이트 (암호화된 펌웨어 코드의 길이)
#   - Firmware Hash:   32바이트 (원본 펌웨어의 SHA256 해시)
#   - AES Nonce:       8바이트 (AES 암호화에 사용된 논스)
#   - Encrypted AES Key: 256바이트 (차량 공개키로 암호화된 AES 세션 키)
#   - Signature:       256바이트 (서버 개인키로 서명된 펌웨어 해시)

header = struct.pack("<I I B B H 32s 8s 256s 256s",
                     MAGIC,
                     TIMESTAMP,
                     ECU_ID,
                     VERSION,
                     CODE_LEN, # 암호화되기 전 원본 CODE의 길이
                     firmware_hash,
                     aes_nonce,
                     encrypted_aes_key,
                     signature)

# === 최종 바이너리 파일 생성 ===
# 헤더와 암호화된 펌웨어 코드를 합쳐 최종 바이너리 파일을 만듭니다.
output_filename = "firmware_secure_hybrid.bin"
with open(output_filename, "wb") as f:
    f.write(header)
    f.write(encrypted_code)

print(f"\n🎉 '{output_filename}' 파일이 성공적으로 생성되었습니다!")
print(f"   - ECU ID: {hex(ECU_ID)}")
print(f"   - Version: {hex(VERSION)}")
print(f"   - Timestamp: {TIMESTAMP} ({time.ctime(TIMESTAMP)})")
print(f"   - Original Code Length: {CODE_LEN} bytes")
print(f"   - Secure File Size: {os.path.getsize(output_filename)} bytes")
print("🚨 'public.pem'은 모든 차량이 동일한 공개키를 사용합니다. 실제 운영에서는 차량별 고유 공개키를 DB로 관리하는 것이 좋습니다.")
print("🚨 'private.pem'은 이제 Secret Manager에서 로드됩니다. 테스트 후 로컬 'private.pem' 파일을 삭제하는 것을 잊지 마세요.")
