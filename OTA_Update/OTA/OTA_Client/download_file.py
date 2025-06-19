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
import sys # sys 모듈 임포트
import shutil # 파일 복사를 위한 shutil 모듈 임포트

# 서버 설정
server_base_url = 'https://www.sapaghetti.shop'
latest_version_api = f'{server_base_url}/latest_version'
get_nonce_api = f'{server_base_url}/get_nonce' # /get_nonce는 이제 POST 요청을 받습니다.
download_base_url = f'{server_base_url}/ota_download'

# 다운로드 저장 폴더
DOWNLOAD_FOLDER = "OTA_download"
UPDATE_FOLDER = "OTA_update_file" # 복호화된 펌웨어를 위해 재추가
#OTA_BACKUP_FOLDER = "OTA_backup" # <-- 롤백 시뮬레이션을 위한 백업 폴더 추가
OTA_BASE_PROJECT_PATH = "C:\\Users\\Public\\Documents\\Vector\\CANoe\\canoe19_project\\ota_project"
ecu_info = [("CGW", "CGW_backup"), ("TMU", "TMU_backup"), ("EDT", "EDT_backup")]
ecu_update_forder = [("CGW", "CGW_update"), ("TMU", "TMU_update"), ("EDT", "EDT_update")]
#ecu_info = ["\CGW\CGW_backup", "\TMU\TMU_backup", "\EDT\EDT_backup"]

# SHA 기록 파일
downloaded_sha_file = 'downloaded_sha.txt'

# 진행률 저장용 파일
progress_file = 'progress.txt'

# 펌웨어 만료 시간 (초). 30일로 설정.
FIRMWARE_EXPIRATION_SECONDS = 30 * 24 * 60 * 60

# 차량 고유 ID 및 인증 토큰 (서버의 vehicles.json과 일치해야 함)
VEHICLE_ID = "Sapaghetti" # <-- 이 부분을 시뮬레이션할 차량 ID로 변경 (예: "VehicleB")
VEHICLE_TOKEN = "SECRET_API_KEY_FOR_Sapaghetti_VEHICLE" # TODO: 실제 환경에서는 차량 ID와 매핑된 고유 토큰 사용

# 롤백 시뮬레이션을 위한 플래그 (테스트용)
# True로 설정하면 ECU 업데이트 완료 신호 (Ota_flag = 5)를 기다리지 않고,
# 일정 시간 후 Ota_flag를 4 (업데이트 실패)로 설정하여 롤백을 트리거합니다.
SIMULATE_UPDATE_FAILURE = False # <-- 업데이트 실패 시뮬레이션 True(ON)/False(OFF)

os.makedirs(DOWNLOAD_FOLDER, exist_ok=True)
os.makedirs(UPDATE_FOLDER, exist_ok=True)
#os.makedirs(OTA_BACKUP_FOLDER, exist_ok=True) # <-- 백업 폴더 생성


# canoe 연동
try:
    canoe_inst = CANoe()
    # TODO: CANoe 경로 - 실제 CANoe 프로젝트 경로로 업데이트하세요.
    canoe_inst.open(canoe_cfg=r'C:\Users\Public\Documents\Vector\CANoe\canoe19_project\ota_project\ota_project.cfg')
    canoe_inst.start_measurement()
except Exception as e:
    print(f"[ERROR] CANoe 연결 실패: {e}. CANoe가 실행 중인지, 경로가 올바른지 확인하세요.")
    canoe_inst = None

# [핵심 변경] 키 로딩 방식 변경
# 클라이언트는 '자신의 개인키'와 '서버의 공개키'를 가지고 있어야 합니다.
try:
    # VEHICLE_ID에 따라 개인키 파일명을 동적으로 구성
    vehicle_private_key_filename = f"{VEHICLE_ID}_private.pem"
    with open(vehicle_private_key_filename, "rb") as f:
        # 이 private.pem은 펌웨어를 수신하는 '차량(ECU)의 개인키'입니다.
        vehicle_private_key = RSA.import_key(f.read())
    print(f"[INFO] 차량 개인키 로드 완료: {vehicle_private_key_filename}")

    with open("public.pem", "rb") as f:
        # 이 public.pem은 펌웨어를 보낸 '서버의 공개키'입니다. 서명 검증에 사용됩니다.
        server_public_key = RSA.import_key(f.read())
    print("[INFO] 서버 공개키 로드 완료: public.pem")

except FileNotFoundError as e:
    print(f"[ERROR] 키 파일('{vehicle_private_key_filename}' 또는 'public.pem')이 없습니다: {e}.")
    sys.exit(1) # 오류 발생 시 스크립트 종료
except Exception as e:
    print(f"[ERROR] 키 로딩 중 오류 발생: {e}")
    sys.exit(1) # 오류 발생 시 스크립트 종료

def get_latest_version(session):
    try:
        response = session.get(latest_version_api, timeout=5)
        response.raise_for_status()
        return response.json()
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Failed to get latest version: {e}")
        return None

# get_nonce 함수 수정 (POST 요청 및 헤더/바디 포함)
def get_nonce(session, vehicle_id, vehicle_token):
    headers = {
        'X-Vehicle-Token': vehicle_token,
        'Content-Type': 'application/json'
    }
    payload = {
        'vehicle_id': vehicle_id
    }
    try:
        response = session.post(get_nonce_api, json=payload, headers=headers, timeout=5)
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
            print("[INFO] OTA 업데이트 완료 (CANoe에서 먼저 신호 보냄)") # <-- 메시지 수정
            return True
        time.sleep(0.1)
    print("[WARN] 승인 응답 없음 (시간 초과)")
    return False

def wait_for_update_complete(timeout=30, simulate_failure=False): # <-- simulate_failure 인자 추가
    if not canoe_inst: return False
    print("[INFO] OTA 업데이트 완료 대기 중...")
    start_time = time.time()
    while time.time() - start_time < timeout:
        flag = canoe_inst.get_system_variable_value('OTA::Ota_flag')
        
        # 업데이트 실패 시뮬레이션 로직
        if simulate_failure and (time.time() - start_time) > 3: # 중간에 실패 시뮬레이션
            if flag != 4: # 아직 실패 플래그가 아니면 설정
                print("[WARN] 업데이트 실패 시뮬레이션: CANoe Ota_flag를 4 (업데이트 실패)로 설정.")
                canoe_inst.set_system_variable_value('OTA::Ota_flag', 4) # <-- 새로운 실패 플래그
                time.sleep(1)
            return False # 업데이트 완료 신호 없이 실패로 간주

        if flag == 5:
            print("[SUCCESS] ECU로부터 업데이트 완료 신호 수신!")
            return True
        time.sleep(0.5)
    print("[WARN] OTA 업데이트 시간 초과")
    
    if not simulate_failure: # 시뮬레이션 실패가 아닌 경우에만 플래그 설정
        if canoe_inst:
            print("[WARN] 시간 초과로 CANoe Ota_flag를 4 (업데이트 실패)로 설정.")
            canoe_inst.set_system_variable_value('OTA::Ota_flag', 4) # 시간 초과도 실패로 간주
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
        time.sleep(0.5)
    print("[WARN] 시간 초과: 버전이 기대한 값으로 바뀌지 않았습니다.")
    return False

def collect_ecu_versions():
    if not canoe_inst: return {}
    try:
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

def report_to_server(session, versions, update_status="UNKNOWN"): # <-- update_status 인자 추가
    url = f"{server_base_url}/report_versions"
    payload = {
        "vehicle_id": VEHICLE_ID,
        "timestamp": time.strftime("%Y-%m-%dT%H:%M:%SZ"),
        "ecus": versions,
        "update_status": update_status # <-- 상태 필드 추가
    }
    headers = {
        'X-Vehicle-Token': VEHICLE_TOKEN
    }
    try:
        res = session.post(url, json=payload, headers=headers, timeout=5)
        print(f"[INFO] 서버 보고 완료: {res.status_code}, {res.text}")
    except Exception as e:
        print(f"[ERROR] 서버 보고 실패: {e}")

# 롤백 시뮬레이션 관련 함수 추가
def backup_current_firmware_sim(ecu_id, current_version):
    backup_bin_filename = f"previous_firmware_ecu_{ecu_id}_ver_{current_version}.bin"
    selected_ecu_tuple = ecu_info[ecu_id-1]
    main_ecu_folder = selected_ecu_tuple[0]
    backup_subfolder = selected_ecu_tuple[1]
    backup_txt_filename = f"{selected_ecu_tuple[0]}_backup_version.txt"
    full_backup_directory_for_ecu = os.path.join(OTA_BASE_PROJECT_PATH, main_ecu_folder, backup_subfolder)
    
    backup_bin_path = os.path.join(full_backup_directory_for_ecu, backup_bin_filename)
    backup_txt_path = os.path.join(full_backup_directory_for_ecu, backup_txt_filename)
    #backup_path = os.path.join(OTA_BACKUP_FOLDER, backup_filename)
    ecu_root_forder = os.path.join(OTA_BASE_PROJECT_PATH, main_ecu_folder)#edtversion 경로
    try:
        current_firmware_bin_path = os.path.join(ecu_root_forder, f"firmware_ecu_{ecu_id}_ver{current_version}.bin")
        current_firmware_txt_path = os.path.join(ecu_root_forder, f"{main_ecu_folder}_version.txt")
        if os.path.exists(current_firmware_bin_path):
            shutil.copy(current_firmware_bin_path, backup_bin_path)
            shutil.copy(current_firmware_txt_path, backup_txt_path)
            print(f"[INFO] 현재 펌웨어 (가상) 백업 완료: {backup_bin_path}")
        else:
            with open(backup_bin_path, 'wb') as f:
                f.write(b'DUMMY_PREVIOUS_FIRMWARE_CONTENT')
            print(f"[INFO] 현재 펌웨어 (더미) 백업 생성: {backup_bin_path}")
    except Exception as e:
        print(f"[ERROR] 이전 펌웨어 백업 실패: {e}")

def restore_previous_firmware_sim(ecu_id, previous_version):
    backup_filename = f"previous_firmware_ecu_{ecu_id}_ver_{previous_version}.bin"
    selected_ecu_tuple = ecu_info[ecu_id-1]
    main_ecu_folder = selected_ecu_tuple[0]
    backup_subfolder = selected_ecu_tuple[1]
    full_backup_directory_for_ecu = os.path.join(OTA_BASE_PROJECT_PATH, main_ecu_folder, backup_subfolder)
    full_restore_directory_for_ecu = os.path.join(OTA_BASE_PROJECT_PATH, main_ecu_folder)
    
    backup_path = os.path.join(full_backup_directory_for_ecu, backup_filename)
    restore_path = os.path.join(full_restore_directory_for_ecu, f"firmware_restored_ecu_{ecu_id}_ver_{previous_version}.bin")
    #backup_path = os.path.join(OTA_BACKUP_FOLDER, backup_filename)
    #restore_path = os.path.join(UPDATE_FOLDER, f"firmware_restored_ecu_{ecu_id}_ver_{previous_version}.bin") # 복원된 파일명은 다르게
    
    if os.path.exists(backup_path):
        try:
            shutil.copy(backup_path, restore_path)
            print(f"[INFO] 이전 펌웨어 (가상) 복원 완료: {restore_path}")
            return True
        except Exception as e:
            print(f"[ERROR] 이전 펌웨어 복원 실패: {e}")
            return False
    else:
        print(f"[ERROR] 복원할 이전 펌웨어 파일이 없습니다: {backup_path}")
        return False

# [추가된 함수 1] 롤백 후 정리 작업을 수행하는 함수
def perform_cleanup_and_finalize(ecu_id, next_version):
    """
    업데이트 폴더의 파일을 정리하고, 완료되면 CANoe 플래그를 9로 설정합니다.
    """
    selected_ecu_tuple = ecu_update_forder[ecu_id-1]
    main_ecu_folder = selected_ecu_tuple[0]
    update_subfolder = selected_ecu_tuple[1]
    full_update_directory_for_ecu = os.path.join(OTA_BASE_PROJECT_PATH, main_ecu_folder, update_subfolder)
    if not canoe_inst:
        print("[WARN] CANoe 인스턴스가 없어 정리 작업을 건너뜁니다.")
        return False
    
    print(f"[INFO] '{full_update_directory_for_ecu}' 폴더의 내용을 정리합니다...")
    files_deleted = False
    try:
        for filename in os.listdir(full_update_directory_for_ecu):
            file_path = os.path.join(full_update_directory_for_ecu, filename)
            if os.path.isfile(file_path) or os.path.islink(file_path):
                os.unlink(file_path)
                print(f"[INFO] 삭제 완료: {file_path}")
                files_deleted = True
        
        if not files_deleted:
            print(f"[INFO] '{full_update_directory_for_ecu}' 폴더에 삭제할 파일이 없습니다.")
        
        # 정리 작업 후 플래그를 9로 설정
        canoe_inst.set_system_variable_value('OTA::Ota_flag', 9)
        print("[SUCCESS] 정리 작업 완료. Ota_flag를 9로 설정했습니다.")
        return True

    except Exception as e:
        print(f"[ERROR] 파일 정리 또는 플래그 설정 중 오류 발생: {e}")
        return False

# [추가된 함수 2] 롤백 완료 후 정리 신호를 기다리는 함수
def wait_for_rollback_cleanup(ecu_id, next_version, timeout=60):
    """
    롤백 완료(flag=7) 후, 정리 시작 신호(flag=8)를 기다립니다.
    신호를 받으면 정리를 수행하고 flag를 9로 바꿉니다.
    """
    if not canoe_inst: return False
    
    print("[INFO] 롤백 완료 (flag=7). 정리 시작 신호(flag=8) 대기 중...")
    start_time = time.time()
    
    while time.time() - start_time < timeout:
        flag = canoe_inst.get_system_variable_value('OTA::Ota_flag')
        
        if flag == 8:
            print("[INFO] 정리 시작 신호 (flag=8) 수신. 정리 작업을 시작합니다.")
            return perform_cleanup_and_finalize(ecu_id, next_version) # 정리 함수 호출
        
        # 대기 중임을 알리기 위해 주기적으로 메시지 출력 (선택 사항)
        if int(time.time() - start_time) % 5 == 0:
            print(f"\r[INFO] 정리 신호(flag=8) 대기 중... ({int(time.time() - start_time)}s)", end="")
            time.sleep(1) # CPU 사용량을 줄이기 위해 짧은 대기
        time.sleep(0.5)
    print("\n[WARN] 정리 신호 대기 시간 초과.")
    return False

if __name__ == "__main__":
    if len(sys.argv) > 1:
        VEHICLE_ID = sys.argv[1]
    else:
        print("[ERROR] 차량 ID가 제공되지 않았습니다. 사용법: python download_file.py <VEHICLE_ID>")
        sys.exit(1)
    
    if VEHICLE_ID == "Sapaghetti":
        VEHICLE_TOKEN = "SECRET_API_KEY_FOR_Sapaghetti_VEHICLE"
    elif VEHICLE_ID == "VehicleB": # 예시: 새로운 차량 ID 추가
        VEHICLE_TOKEN = "ANOTHER_SECRET_API_KEY_FOR_VEHICLEB"
    else:
        print(f"[ERROR] 알 수 없는 차량 ID: {VEHICLE_ID}. 유효한 차량 ID를 제공하세요.")
        sys.exit(1)


    if os.path.exists(progress_file):
        os.remove(progress_file)

    session = requests.Session()
    downloaded_sha256 = load_downloaded_sha()
    CHECK_INTERVAL = 10

    while True:
        # 이전의 범용적인 flag=8 확인 로직은 삭제되었습니다.

        print(f"\n--- OTA 업데이트 확인 주기 (차량 ID: {VEHICLE_ID}) ---")
        latest_version_data = get_latest_version(session)
        if not latest_version_data or not latest_version_data.get('version'):
            print("[WARN] 서버에서 최신 버전을 사용할 수 없습니다.")
            time.sleep(CHECK_INTERVAL)
            continue
            
        latest_sha256 = latest_version_data['sha256']
        latest_version_name = latest_version_data['version'] # 파일명 그대로 가져옴

        if latest_sha256 == downloaded_sha256:
            print(f"[INFO] 이미 최신 버전입니다. (SHA: {latest_sha256[:10]}...)")
        else:
            print(f"[INFO] 새로운 펌웨어 발견: {latest_version_name}")
            
            # Nonce 획득
            nonce = get_nonce(session, VEHICLE_ID, VEHICLE_TOKEN)
            if not nonce:
                print("[ERROR] Nonce 획득 실패. 다음 주기에 재시도.")
                time.sleep(CHECK_INTERVAL)
                continue
            
            download_url = f"{download_base_url}/{latest_version_name}?nonce={nonce}"
            local_filename = os.path.join(DOWNLOAD_FOLDER, latest_version_name)
            
            session.headers.update({'X-Vehicle-Token': VEHICLE_TOKEN})

            if download_file(session, download_url, local_filename):
                downloaded_file_sha256 = calculate_file_sha256(local_filename)
                if downloaded_file_sha256 != latest_sha256:
                    print("[ERROR] 다운로드된 파일 무결성 검증 실패! 서버-클라이언트 SHA 불일치.")
                    del session.headers['X-Vehicle-Token']
                    if canoe_inst:
                        canoe_inst.set_system_variable_value('OTA::Ota_flag', 4) # 업데이트 실패 플래그
                        current_versions_at_fail = collect_ecu_versions()
                        report_to_server(session, current_versions_at_fail, update_status="DOWNLOAD_INTEGRITY_FAILED")
                    continue

                print("[SUCCESS] 다운로드된 파일 무결성 검증 완료.")
                decrypted_code, magic, ecu_id, new_version, code_len = process_secure_firmware(local_filename)
                
                if decrypted_code is not None:
                    print("[SUCCESS] 펌웨어 보안 검증(최신성, 무결성, 인증) 완료.")
                    
                    output_header = struct.pack("<IBBH", magic, ecu_id, new_version, code_len)
                    output_bin_path = os.path.join(UPDATE_FOLDER, f"firmware_update_ecu_{ecu_id}_ver_{new_version}.bin")
                    with open(output_bin_path, "wb") as out_f:
                        out_f.write(output_header)
                        out_f.write(decrypted_code)
                    print(f"[SUCCESS] 복호화된 바이너리 저장 완료 → {output_bin_path}")
                    
                    current_ecu_versions = collect_ecu_versions()
                    ecu_name_for_backup = {1: "CGW", 2: "TMU", 3: "EDT"}.get(ecu_id, None)
                    if ecu_name_for_backup:
                        current_version_for_backup = current_ecu_versions.get(ecu_name_for_backup, "UNKNOWN")
                        backup_current_firmware_sim(ecu_id, current_version_for_backup)

                    if canoe_inst:
                        canoe_inst.set_system_variable_value('OTA::Ota_flag', 1) 
                        canoe_inst.set_system_variable_value('OTA::ECU_id', ecu_id)
                        canoe_inst.set_system_variable_value('OTA::ECU_ver', new_version)

                    if wait_for_approval(30):
                        print("[INFO] CANoe가 파일을 읽고 업데이트를 시작할 예정")
                        
                        if wait_for_update_complete(30, simulate_failure=SIMULATE_UPDATE_FAILURE): 
                            if wait_until_version_updated(ecu_id, new_version):
                                print("[INFO] 최종 업데이트 성공. 새로운 버전 정보를 서버에 보고합니다.")
                                save_downloaded_sha(latest_sha256)
                                downloaded_sha256 = latest_sha256
                                time.sleep(2)
                                current_versions = collect_ecu_versions()
                                report_to_server(session, current_versions, update_status="SUCCESS") 
                            else:
                                print("[ERROR] 업데이트는 완료 신호를 보냈으나, ECU 버전이 기대한 값으로 바뀌지 않았습니다. 롤백 시도.")
                                if canoe_inst:
                                    canoe_inst.set_system_variable_value('OTA::Ota_flag', 6) # 롤백 시작 플래그
                                print("[INFO] 롤백 시뮬레이션 시작...")
                                time.sleep(1)
                                if restore_previous_firmware_sim(ecu_id, current_version_for_backup): 
                                    print("[INFO] 롤백 시뮬레이션 성공. CANoe Ota_flag를 7 (롤백 완료)로 설정.")
                                    if canoe_inst:
                                        canoe_inst.set_system_variable_value('OTA::Ota_flag', 7) # 롤백 완료 플래그
                                    
                                    # [수정된 부분 1] 롤백 후 정리 대기 및 실행
                                    wait_for_rollback_cleanup(ecu_id, new_version)

                                    current_versions_after_rollback = collect_ecu_versions()
                                    report_to_server(session, current_versions_after_rollback, update_status="ROLLBACK_COMPLETED") 
                                else:
                                    print("[FATAL] 롤백 시뮬레이션 실패.")
                                    current_versions_after_rollback_fail = collect_ecu_versions()
                                    report_to_server(session, current_versions_after_rollback_fail, update_status="ROLLBACK_FAILED") 
                        else:
                            print("[ERROR] ECU로부터 업데이트 완료 응답을 받지 못했습니다. (업데이트 실패 또는 시간 초과). 롤백 시도.")
                            time.sleep(1)
                            if canoe_inst:
                                canoe_inst.set_system_variable_value('OTA::Ota_flag', 6) # 롤백 시작 플래그
                            print("[INFO] 롤백 시뮬레이션 시작...")
                            if restore_previous_firmware_sim(ecu_id, current_version_for_backup):
                                print("[INFO] 롤백 시뮬레이션 성공. CANoe Ota_flag를 7 (롤백 완료)로 설정.")
                                if canoe_inst:
                                    canoe_inst.set_system_variable_value('OTA::Ota_flag', 7) # 롤백 완료 플래그
                                
                                # [수정된 부분 2] 롤백 후 정리 대기 및 실행
                                wait_for_rollback_cleanup(ecu_id, new_version)
                                
                                current_versions_after_rollback = collect_ecu_versions()
                                report_to_server(session, current_versions_after_rollback, update_status="ROLLBACK_COMPLETED")
                            else:
                                print("[FATAL] 롤백 시뮬레이션 실패.")
                                current_versions_after_rollback_fail = collect_ecu_versions()
                                report_to_server(session, current_versions_after_rollback_fail, update_status="ROLLBACK_FAILED")

                    else:
                        print("[INFO] 사용자가 업데이트를 거절했거나 시간 초과로 프로세스를 중단합니다.")
                        if canoe_inst:
                            canoe_inst.set_system_variable_value('OTA::Ota_flag', 3) 
                        current_versions_rejected = collect_ecu_versions()
                        report_to_server(session, current_versions_rejected, update_status="USER_REJECTED")
                else:
                    print("[FATAL] 펌웨어 보안 검증 실패! 이 펌웨어는 설치할 수 없습니다.")
                    if canoe_inst:
                        canoe_inst.set_system_variable_value('OTA::Ota_flag', 4) 
                    current_versions_sec_fail = collect_ecu_versions()
                    report_to_server(session, current_versions_sec_fail, update_status="SECURITY_VALIDATION_FAILED")
            
            if 'X-Vehicle-Token' in session.headers:
                del session.headers['X-Vehicle-Token']
        
        print(f"--- 다음 확인까지 {CHECK_INTERVAL}초 대기 ---")
        time.sleep(CHECK_INTERVAL)