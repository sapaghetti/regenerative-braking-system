# make_bin_file.py

import struct
import hashlib
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
import os
import time

# === 설정값 ===
ECU_ID = 0x03
VERSION = 0x06
MAGIC = 0xDEADBEEF
TIMESTAMP = int(time.time()) # ✅ 변경: 펌웨어 생성 시점 타임스탬프 추가 (최신성 보장)

with open("firmware.bin", "rb") as f:
    CODE = f.read()
CODE_LEN = len(CODE)

# === 개인키 로딩 ===
if not os.path.exists("private.pem"):
    raise FileNotFoundError("❌ 'private.pem' 파일이 현재 폴더에 없음. openssl로 먼저 생성해줘!")

with open("private.pem", "rb") as f:
    private_key = RSA.import_key(f.read())

# === SHA256 해시 및 전자서명 (무결성 및 인증) ===
# 논문 요구사항: 업데이트 패키지가 신뢰된 출처에서 왔는지 확인 (인증) 
# 논문 요구사항: 업데이트 패키지의 수정 감지 (무결성) 
hash_obj = SHA256.new(CODE)
sha256_digest = hash_obj.digest()
signature = pkcs1_15.new(private_key).sign(hash_obj)  # 256 bytes (RSA2048)

# === AES 암호화 (기밀성) ===
# 논문 요구사항: 외부 공격자가 업데이트 패키지에 접근할 수 없어야 함 (기밀성) 
aes_key = get_random_bytes(16)  # 128bit key
aes_nonce = get_random_bytes(8)  # CTR용 nonce

cipher = AES.new(aes_key, AES.MODE_CTR, nonce=aes_nonce)
encrypted_code = cipher.encrypt(CODE)

# === 헤더 구성: <I I B B H 32s 8s
#   - Magic:     4바이트 (시작 식별자)
#   - Timestamp: 4바이트 (✅ 추가된 필드, 최신성 검증용)
#   - ECU ID:    1바이트 (대상 ECU 식별자)
#   - Version:   1바이트 (펌웨어 버전)
#   - Code Len:  2바이트 (암호화된 코드 길이)
#   - SHA256:    32바이트 (원본 코드의 해시)
#   - AES Nonce: 8바이트 (복호화에 필요한 Nonce)
header_format = "<IIBBH32s8s"
header = struct.pack(header_format, MAGIC, TIMESTAMP, ECU_ID, VERSION, len(encrypted_code), sha256_digest, aes_nonce)

# === 최종 OTA 파일 쓰기
with open("firmware_secure.bin", "wb") as f:
    f.write(header)
    f.write(signature)
    f.write(encrypted_code)

# === AES 키는 별도 저장 (부트로더에 하드코딩하거나 보안 경로로 전송해야 함)
with open("aes_key.bin", "wb") as f:
    f.write(aes_key)

print(f"✅ firmware_secure.bin 생성 완료 (타임스탬프: {TIMESTAMP}, 전자서명 + 암호화 포함)")
print("   - 논문의 '최신성', '무결성', '인증', '기밀성' 요구사항을 반영했습니다.")

#openssl genpkey -algorithm RSA -out private.pem -pkeyopt rsa_keygen_bits:2048
#openssl rsa -in private.pem -pubout -out public.pem


# bin파일 만드는 코드
# gcc -o firmware.elf firmware.c
# objcopy -O binary firmware.elf firmware.bin