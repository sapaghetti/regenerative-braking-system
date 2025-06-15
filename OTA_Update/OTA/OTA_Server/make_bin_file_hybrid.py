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

# === 설정값 ===
ECU_ID = 0x03
VERSION = 0x06
MAGIC = 0xDEADBEEF
TIMESTAMP = int(time.time())

with open("firmware.bin", "rb") as f:
    CODE = f.read()
CODE_LEN = len(CODE)

# === 개인키/공개키 로딩 ===
# 서버는 서명을 위해 '자신의 개인키'를, 키 암호화를 위해 '차량의 공개키'를 사용합니다.
if not os.path.exists("private.pem") or not os.path.exists("public.pem"):
    raise FileNotFoundError("❌ 'private.pem'과 'public.pem' 파일이 현재 폴더에 없음. openssl로 먼저 생성해줘!")

with open("private.pem", "rb") as f:
    # 이 private.pem은 서버의 개인키입니다.
    server_private_key = RSA.import_key(f.read())

with open("public.pem", "rb") as f:
    # 이 public.pem은 펌웨어를 수신할 차량(ECU)의 공개키입니다.
    # 실제 환경에서는 차량 ID에 맞는 공개키를 DB 등에서 가져와야 합니다.
    vehicle_public_key = RSA.import_key(f.read())

# === SHA256 해시 및 전자서명 (무결성 및 인증) ===
# 원본 코드의 해시를 계산하고, '서버의 개인키'로 서명합니다.
hash_obj = SHA256.new(CODE)
sha256_digest = hash_obj.digest()
signature = pkcs1_15.new(server_private_key).sign(hash_obj)  # 256 bytes (RSA2048)

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

# === 헤더 구성: <I I B B H 32s 8s
#   - Magic:     4바이트 (시작 식별자)
#   - Timestamp: 4바이트 (최신성 검증용)
#   - ECU ID:    1바이트 (대상 ECU 식별자)
#   - Version:   1바이트 (펌웨어 버전)
#   - Code Len:  2바이트 (암호화된 코드 길이)
#   - SHA256:    32바이트 (원본 코드의 해시)
#   - AES Nonce: 8바이트 (복호화에 필요한 Nonce)
header_format = "<IIBBH32s8s"
header = struct.pack(header_format, MAGIC, TIMESTAMP, ECU_ID, VERSION, len(encrypted_code), sha256_digest, aes_nonce)

# === 최종 OTA 파일 쓰기 (헤더 + 암호화된 AES 키 + 서명 + 암호화된 코드) ===
# 파일 구조가 변경되었습니다. 암호화된 AES키가 파일 내에 포함됩니다.
with open("firmware_secure_hybrid.bin", "wb") as f:
    f.write(header)
    f.write(encrypted_aes_key) # 암호화된 AES 키 (256 바이트)
    f.write(signature)         # 전자 서명 (256 바이트)
    f.write(encrypted_code)    # 암호화된 펌웨어 코드

# 더 이상 aes_key.bin 파일을 별도로 저장할 필요가 없습니다.

print(f"✅ firmware_secure_hybrid.bin 생성 완료 (타임스탬프: {TIMESTAMP}, 하이브리드 암호화 적용)")
print("   - 이제 AES 키는 차량의 공개키로 암호화되어 패키지 내에 안전하게 포함됩니다.")