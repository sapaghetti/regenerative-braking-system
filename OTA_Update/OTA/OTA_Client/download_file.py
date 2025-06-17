# download_file.py

import requests
import time
import os
import hashlib
from py_canoe import CANoe
import struct
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Cipher import AES, PKCS1_OAEP

# 서버 설정
server_base_url = 'http://127.0.0.1:5000'
latest_version_api = f'{server_base_url}/latest_version'
get_nonce_api = f'{server_base_url}/get_nonce'
download_base_url = f'{server_base_url}/ota_download'

# 다운로드 저장 폴더
DOWNLOAD_FOLDER = "OTA_download"
UPDATE_FOLDER = "OTA_update_file" # 복호화된 펌웨어를 위해 재추가

# SHA 기록 파일
downloaded_sha_file = 'downloaded_sha.txt'

# 진행률 저장용 파일
progress_file = 'progress.txt'

# 펌웨어 만료 시간 (초). 30일로 설정.
FIRMWARE_EXPIRATION_SECONDS = 30 * 24 * 60 * 60 

# 차량 고유 ID 및 인증 토큰 (서버의 vehicles.json과 일치해야 함)
VEHICLE_ID = "Sapaghetii"
VEHICLE_TOKEN = "SECRET_API_KEY_FOR_Sapaghetii_VEHICLE"

os.makedirs(DOWNLOAD_FOLDER, exist_ok=True)
os.makedirs(UPDATE_FOLDER, exist_ok=True) # UPDATE_FOLDER가 있는지 확인

# canoe 연동
try:
    canoe_inst = CANoe()
    canoe_inst.open(canoe_cfg=r'C:\Users\Public\Documents\Vector\CANoe\canoe19_project\ota_project\ota_project.cfg')
    canoe_inst.start_measurement()
except Exception as e:
    print(f"[ERROR] CANoe 연결 실패: {e}. CANoe가 실행 중인지, 경로가 올바른지 확인하세요.")
    canoe_inst = None

# [핵심 변경] 키 로딩 방식 변경
# 클라이언트는 '자신의 개인키'와 '서버의 공개키'를 가지고 있어야 합니다.
try:
    with open("private.pem", "rb") as f:
        # 이 private.pem은 펌웨어를 수신하는 '차량(ECU)의 개인키'입니다.
        vehicle_private_key = RSA.import_key(f.read())

    with open("public.pem", "rb") as f:
        # 이 public.pem은 펌웨어를 보낸 '서버의 공개키'입니다. 서명 검증에 사용됩니다.
        server_public_key = RSA.import_key(f.read())

except FileNotFoundError as e:
    print(f"[ERROR] 키 파일('private.pem', 'public.pem')이 없습니다: {e}.")
    exit()

def get_latest_version(session):
    try:
        response = session.get(latest_version_api, timeout=5)
        response.raise_for_status()
        return response.json()
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Failed to get latest version: {e}")
        return None

def get_nonce(session):
    try:
        response = session.get(get_nonce_api, timeout=5)
        response.raise_for_status()
        return response.json().get('nonce')
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Failed to get nonce: {e}")
        return None

def download_file(session, url, local_filename):
    try:
        with session.get(url, stream=True, timeout=10) as r:
            r.raise_for_status()
            total_size = int(r.headers.get('content-length', 0))
            downloaded_size = 0
            with open(local_filename, 'wb') as f:
                for chunk in r.iter_content(chunk_size=8192):
                    f.write(chunk)
                    downloaded_size += len(chunk)
                    progress = int((downloaded_size * 100) / total_size) if total_size else 0
                    print(f"\rDownloading... {progress}%", end="")
            print("\n[INFO] Download complete!")
            return True
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Download failed: {e}")
        return False

def calculate_file_sha256(filepath):
    sha256_hash = hashlib.sha256()
    with open(filepath, "rb") as f:
        for byte_block in iter(lambda: f.read(4096), b""):
            sha256_hash.update(byte_block)
    return sha256_hash.hexdigest()

def load_downloaded_sha():
    if os.path.exists(downloaded_sha_file):
        with open(downloaded_sha_file, 'r') as f:
            return f.read().strip()
    return None

def save_downloaded_sha(sha256):
    with open(downloaded_sha_file, 'w') as f:
        f.write(sha256)

def process_secure_firmware(filepath):
    print(f"[INFO] 하이브리드 보안 펌웨어 처리 시작: {filepath}")
    try:
        with open(filepath, 'rb') as f:
            # 1. 헤더 언패킹 (make_bin_file_hybrid.py와 형식 일치)
            header_format = "<IIBBH32s8s"
            header_size = struct.calcsize(header_format)
            header_data = f.read(header_size)
            if len(header_data) < header_size:
                print("[ERROR] 헤더 데이터가 너무 짧습니다.")
                return None, None, None, None, None
            
            magic, timestamp, ecu_id, version, code_len, sha256_from_header, aes_nonce = struct.unpack(header_format, header_data)
            print(f"[INFO] 헤더 정보: Magic={hex(magic)}, Timestamp={timestamp}, ECU_ID={ecu_id}, Version={version}")

            # 최신성(Freshness) 검증
            current_time = int(time.time())
            if current_time - timestamp > FIRMWARE_EXPIRATION_SECONDS:
                print(f"[ERROR] 최신성 검증 실패! 펌웨어가 너무 오래되었습니다 (만료됨). 생성 시각: {timestamp}")
                return None, None, None, None, None

            # 2. [핵심 변경] 암호화된 데이터 분리 (순서 주의!)
            # 파일 구조에 맞게 데이터를 읽어옵니다.
            encrypted_aes_key = f.read(256) # RSA-2048 암호화 결과는 256바이트
            signature = f.read(256)         # 서명도 256바이트
            encrypted_code = f.read()       # 나머지 전체가 암호화된 코드
            
            if len(encrypted_aes_key) != 256 or len(signature) != 256 or len(encrypted_code) != code_len:
                print("[ERROR] 파일 구조가 손상되었거나 길이가 헤더 정보와 일치하지 않습니다.")
                return None, None, None, None, None

            # 3. [핵심 변경] 차량의 개인키로 AES 키 복호화
            # 자신의 개인키를 사용해 패키지에서 AES 세션 키를 추출합니다.
            cipher_rsa = PKCS1_OAEP.new(vehicle_private_key)
            decrypted_aes_key = cipher_rsa.decrypt(encrypted_aes_key)
            print("[INFO] 차량 개인키로 AES 세션 키 복호화 완료.")
            
            # 4. 복호화된 AES 키로 펌웨어 코드 복호화 (기밀성)
            cipher_aes = AES.new(decrypted_aes_key, AES.MODE_CTR, nonce=aes_nonce)
            decrypted_code = cipher_aes.decrypt(encrypted_code)
            print("[INFO] AES로 펌웨어 코드 복호화 완료.")

            # 5. SHA256 해시 검증 (무결성)
            hash_obj_verify = SHA256.new(decrypted_code)
            if hash_obj_verify.digest() != sha256_from_header:
                print("[ERROR] SHA256 해시 검증 실패! 파일이 변조되었을 수 있습니다.")
                return None, None, None, None, None
            print("[SUCCESS] SHA256 해시 검증 성공 (무결성 확인).")

            # 6. RSA 전자서명 검증 (서버의 공개키 사용, 인증)
            # 서명은 '서버의 공개키'로 검증하여 신뢰된 서버가 보낸 것인지 확인합니다.
            pkcs1_15.new(server_public_key).verify(hash_obj_verify, signature)
            print("[SUCCESS] RSA 전자서명 검증 성공 (인증 확인).")

            # 모든 검증을 통과한 깨끗한 데이터만 반환
            return decrypted_code, magic, ecu_id, version, code_len

    except Exception as e:
        # Crypto 라이브러리는 검증/복호화 실패 시 Exception을 발생시킵니다.
        print(f"[ERROR] 펌웨어 처리 중 심각한 오류 발생 (검증/복호화 실패 가능성 높음): {e}")
        return None, None, None, None, None

def wait_for_approval(timeout=30):
    if not canoe_inst: return False
    print("[INFO] 사용자 승인 대기 중...")
    start_time = time.time()
    while time.time() - start_time < timeout:
        flag = canoe_inst.get_system_variable_value('OTA::Ota_flag')
        if flag == 2:
            print("[INFO] 사용자 승인됨")
            return True
        elif flag == 3:
            print("[INFO] 사용자 거절됨")
            return False
        elif flag == 5: # If CANoe already finished the update
            print("[INFO] OTA 업데이트 완료")
            return True # 업데이트가 이미 완료된 경우 승인된 것으로 간주
        time.sleep(1)
    print("[WARN] 승인 응답 없음 (시간 초과)")
    return False

def wait_for_update_complete(timeout=60):
    if not canoe_inst: return False
    print("[INFO] OTA 업데이트 완료 대기 중...")
    start_time = time.time()
    while time.time() - start_time < timeout:
        flag = canoe_inst.get_system_variable_value('OTA::Ota_flag')
        if flag == 5:
            print("[SUCCESS] ECU로부터 업데이트 완료 신호 수신!")
            return True
        time.sleep(1)
    print("[WARN] OTA 업데이트 시간 초과")
    return False

def wait_until_version_updated(ecu_id, expected_version, timeout=20):
    if not canoe_inst: return False
    print("[INFO] 버전 업데이트 확인 대기 중...")
    start_time = time.time()
    while time.time() - start_time < timeout:
        versions = collect_ecu_versions()
        ecu_name = {1: "CGW", 2: "TMU", 3: "EDT"}.get(ecu_id, None) # ECU ID가 이름과 매핑된다고 가정
        if ecu_name and versions.get(ecu_name) == expected_version:
            print(f"[SUCCESS] {ecu_name} 버전이 {expected_version}로 업데이트됨")
            return True
        time.sleep(1)
    print("[WARN] 시간 초과: 버전이 기대한 값으로 바뀌지 않았습니다.")
    return False

def collect_ecu_versions():
    if not canoe_inst: return {}
    try:
        # CANoe의 시스템 변수가 특정 ECU 버전을 제공할 수 있다고 가정
        # CANoe 구성에 따라 각 ECU에 맞게 조정하세요.
        # 편의상 이전에 보았던 EDT_Cur_Ver를 사용합니다.
        # CGW, TMU 등에 대한 별도의 변수가 있는 경우 해당 변수를 사용하세요.
        ecu = canoe_inst.get_system_variable_value('OTA::Cur_version')
        versions = {
            "CGW": ecu[1],
            "TMU": ecu[2],
            "EDT": ecu[3]
        }
        print("[INFO] 수집된 ECU 버전 정보:", versions)
        return versions
    except Exception as e:
        print(f"[ERROR] ECU 버전 수집 실패: {e}")
        return {}

def report_to_server(session, versions):
    url = f"{server_base_url}/report_versions"
    payload = {
        "vehicle_id": VEHICLE_ID,
        "timestamp": time.strftime("%Y-%m-%dT%H:%M:%SZ"),
        "ecus": versions
    }
    headers = {
        'X-Vehicle-Token': VEHICLE_TOKEN
    }
    try:
        res = session.post(url, json=payload, headers=headers, timeout=5)
        print(f"[INFO] 서버 보고 완료: {res.status_code}, {res.text}")
    except Exception as e:
        print(f"[ERROR] 서버 보고 실패: {e}")


if __name__ == "__main__":
    if os.path.exists(progress_file):
        os.remove(progress_file)

    session = requests.Session()
    downloaded_sha256 = load_downloaded_sha()
    CHECK_INTERVAL = 10

    while True:
        print("\n--- OTA 업데이트 확인 주기 ---")
        latest_version_data = get_latest_version(session)
        if not latest_version_data or not latest_version_data.get('version'):
            print("[WARN] 서버에서 최신 버전을 사용할 수 없습니다.")
            time.sleep(CHECK_INTERVAL)
            continue
            
        latest_sha256 = latest_version_data['sha256']
        latest_version = latest_version_data['version']

        if latest_sha256 == downloaded_sha256:
            print(f"[INFO] 이미 최신 버전입니다. (SHA: {latest_sha256[:10]}...)")
        else:
            print(f"[INFO] 새로운 펌웨어 발견: {latest_version}")
            nonce = get_nonce(session)
            if not nonce:
                print("[ERROR] Nonce 획득 실패. 다음 주기에 재시도.")
                time.sleep(CHECK_INTERVAL)
                continue
            
            # 다운로드 시에는 하이브리드 파일 이름으로 요청해야 합니다. 
            # (또는 서버의 /latest_version이 하이브리드 파일 이름을 반환하도록 수정)
            download_url = f"{download_base_url}/{latest_version}?nonce={nonce}"
            local_filename = os.path.join(DOWNLOAD_FOLDER, latest_version)

            if download_file(session, download_url, local_filename):
                downloaded_file_sha256 = calculate_file_sha256(local_filename)
                if downloaded_file_sha256 != latest_sha256:
                    print("[ERROR] 다운로드된 파일 무결성 검증 실패! 서버-클라이언트 SHA 불일치.")
                    continue

                print("[SUCCESS] 다운로드된 파일 무결성 검증 완료.")
                decrypted_code, magic, ecu_id, new_version, code_len = process_secure_firmware(local_filename)
                
                if decrypted_code is not None: # process_secure_firmware가 성공했는지 확인
                    print("[SUCCESS] 펌웨어 보안 검증(최신성, 무결성, 인증) 완료.")
                    
                    # 출력 파일에 대한 헤더 재구성
                    # 헤더 형식은 CANoe가 업데이트 파일을 위해 기대하는 형식과 일치해야 합니다.
                    # 이 헤더는 원래 파싱된 내용과 일치해야 합니다.
                    # make_bin_file_hybrid.py가 *출력* 파일에 다른 헤더를 사용하는 경우 형식을 조정하십시오.
                    # prev.py를 기반으로 CANoe가 읽을 수 있도록 <IBBH를 기대합니다.
                    output_header = struct.pack("<IBBH", magic, ecu_id, new_version, code_len)

                    output_bin_path = os.path.join(UPDATE_FOLDER, f"firmware_update_ecu_{ecu_id}_ver_{new_version}.bin")
                    with open(output_bin_path, "wb") as out_f:
                        out_f.write(output_header)
                        out_f.write(decrypted_code)
                    print(f"[SUCCESS] 복호화된 바이너리 저장 완료 → {output_bin_path}")
                    
                    if canoe_inst:
                        canoe_inst.set_system_variable_value('OTA::OTA_flag', 1) # 새 업데이트 파일에 대해 CANoe에 알림
                        canoe_inst.set_system_variable_value('OTA::ECU_id', ecu_id)
                        canoe_inst.set_system_variable_value('OTA::ECU_ver', new_version)

                    if wait_for_approval(30):
                        # CANoe는 승인 후 OTA_update_file 폴더에서 파일을 읽을 것으로 예상됩니다.
                        # 여기서는 py_canoe를 통한 직접적인 파일 전송은 없습니다.
                        print("[INFO] 사용자 승인됨. CANoe가 파일을 읽고 업데이트를 시작할 것으로 예상됩니다.")
                        
                        if wait_for_update_complete(60):
                            if wait_until_version_updated(ecu_id, new_version):
                                print("[INFO] 최종 업데이트 성공. 새로운 버전 정보를 서버에 보고합니다.")
                                save_downloaded_sha(latest_sha256)
                                downloaded_sha256 = latest_sha256
                                time.sleep(2) # ECU가 버전 정보를 업데이트할 시간을 줍니다.
                                current_versions = collect_ecu_versions()
                                report_to_server(session, current_versions)
                            else:
                                print("[ERROR] 업데이트는 완료 신호를 보냈으나, ECU 버전이 기대한 값으로 바뀌지 않았습니다.")
                        else:
                            print("[ERROR] ECU로부터 업데이트 완료 응답을 받지 못했습니다.")
                    else:
                        print("[INFO] 사용자가 업데이트를 거절했거나 시간 초과로 프로세스를 중단합니다.")
                else:
                    print("[FATAL] 펌웨어 보안 검증 실패! 이 펌웨어는 설치할 수 없습니다.")
        
        print(f"--- 다음 확인까지 {CHECK_INTERVAL}초 대기 ---")
        time.sleep(CHECK_INTERVAL)