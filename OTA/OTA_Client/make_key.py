# 새 키 쌍 생성 예시 (Python 스크립트)
from Crypto.PublicKey import RSA
key = RSA.generate(2048)
with open("VehicleB_private.pem", "wb") as f:
    f.write(key.export_key("PEM"))
# corresponding_public_key = key.publickey()
# 이 공개키는 서버에 등록되어야 하지만, 지금은 시뮬레이션 목적이므로 서버에는 기존 public.pem만 있으면 됩니다.