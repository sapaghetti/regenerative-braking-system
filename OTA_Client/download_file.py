# download_file.py
import ssl
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
import sys
import shutil

# MQTT 및 스레딩 관련 라이브러리 추가
import paho.mqtt.client as mqtt
import threading
import queue
import json # MQTT 메시지 파싱을 위해 추가

# 서버 설정
server_base_url = 'https://www.sapaghetti.shop'
latest_version_api = f'{server_base_url}/latest_version' # 이제 MQTT 알림으로 대체될 것
get_nonce_api = f'{server_base_url}/get_nonce'
download_base_url = f'{server_base_url}/ota_download'

# 다운로드 저장 폴더
DOWNLOAD_FOLDER = "OTA_download"
UPDATE_FOLDER = "OTA_update_file"
OTA_BASE_PROJECT_PATH = "C:\\Users\\Public\\Documents\\Vector\\CANoe\\canoe19_project\\ota_project"
ecu_info = [("CGW", "CGW_backup"), ("TMU", "TMU_backup"), ("EDT", "EDT_backup")]
ecu_update_forder = [("CGW", "CGW_update"), ("TMU", "TMU_update"), ("EDT", "EDT_update")]

# SHA 기록 파일
downloaded_sha_file = 'downloaded_sha.txt'

# 진행률 저장용 파일 (현재 사용되지 않음)
progress_file = 'progress.txt'

# 펌웨어 만료 시간 (초). 30일로 설정.
FIRMWARE_EXPIRATION_SECONDS = 30 * 24 * 60 * 60

# 차량 고유 ID 및 인증 토큰 (main 함수에서 sys.argv로 설정됨)
VEHICLE_ID = ""
VEHICLE_TOKEN = ""

# 롤백 시뮬레이션을 위한 플래그 (테스트용)
SIMULATE_UPDATE_FAILURE = False

os.makedirs(DOWNLOAD_FOLDER, exist_ok=True)
os.makedirs(UPDATE_FOLDER, exist_ok=True)

# canoe 연동
try:
    canoe_inst = CANoe()
    canoe_inst.open(canoe_cfg=r'C:\Users\Public\Documents\Vector\CANoe\canoe19_project\ota_project\ota_project.cfg')
    canoe_inst.start_measurement()
except Exception as e:
    print(f"[ERROR] CANoe 연결 실패: {e}. CANoe가 실행 중인지, 경로가 올바른지 확인하세요.")
    canoe_inst = None

# === MQTT 브로커 설정 ===
# TODO: 실제 MQTT 브로커 주소와 포트로 변경하세요! (app.py의 설정과 일치해야 함)
MQTT_BROKER_HOST = "www.sapaghetti.shop"
MQTT_BROKER_PORT = 8883
MQTT_UPDATE_TOPIC = "ota/update/available" # 서버가 발행할 펌웨어 업데이트 알림 토픽

# MQTT 메시지를 메인 루프로 전달하기 위한 큐
mqtt_message_queue = queue.Queue()

# [핵심 변경] 키 로딩 방식 변경
# 클라이언트는 '자신의 개인키'와 '서버의 공개키'를 가지고 있어야 합니다.
vehicle_private_key = None
server_public_key = None

def load_keys(vehicle_id):
    global vehicle_private_key, server_public_key
    try:
        # VEHICLE_ID에 따라 개인키 파일명을 동적으로 구성
        vehicle_private_key_filename = f"{vehicle_id}_private.pem"
        with open(vehicle_private_key_filename, "rb") as f:
            vehicle_private_key = RSA.import_key(f.read())
        print(f"[INFO] 차량 개인키 로드 완료: {vehicle_private_key_filename}")

        with open("public.pem", "rb") as f:
            server_public_key = RSA.import_key(f.read())
        print("[INFO] 서버 공개키 로드 완료: public.pem")

    except FileNotFoundError as e:
        print(f"[ERROR] 키 파일('{e.filename}')이 없습니다. 스크립트를 종료합니다.")
        sys.exit(1)
    except Exception as e:
        print(f"[ERROR] 키 로딩 중 오류 발생: {e}. 스크립트를 종료합니다.")
        sys.exit(1)

# === MQTT 콜백 함수 ===
def on_connect(client, userdata, flags, rc):
    """MQTT 브로커 연결 시 호출되는 콜백 함수"""
    if rc == 0:
        print(f"[MQTT] 브로커에 성공적으로 연결됨. (결과 코드: {rc})")
        client.subscribe(MQTT_UPDATE_TOPIC) # 새 펌웨어 알림 토픽 구독
        print(f"[MQTT] 토픽 '{MQTT_UPDATE_TOPIC}' 구독 시작.")
    else:
        print(f"[MQTT ERROR] 브로커 연결 실패. 결과 코드: {rc}")

def on_message(client, userdata, msg):
    """MQTT 메시지 수신 시 호출되는 콜백 함수"""
    print(f"[MQTT] 메시지 수신 - 토픽: {msg.topic}")
    try:
        # 수신된 메시지를 메인 루프에서 처리하기 위해 큐에 넣음
        mqtt_message_queue.put(msg.payload.decode('utf-8'))
    except Exception as e:
        print(f"[MQTT ERROR] 메시지 디코딩 또는 큐 삽입 실패: {e}")

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
        print(f"[ERROR] Nonce 획득 실패: {e}")
        return None

def download_file(session, url, local_filename):
    try:
        with session.get(url, stream=True, timeout=30) as r: # 다운로드 타임아웃 증가
            r.raise_for_status()
            total_size = int(r.headers.get('content-length', 0))
            downloaded_size = 0
            with open(local_filename, 'wb') as f:
                for chunk in r.iter_content(chunk_size=8192):
                    f.write(chunk)
                    downloaded_size += len(chunk)
                    progress = int((downloaded_size * 100) / total_size) if total_size else 0
                    print(f"\rDownloading... {progress}% ({downloaded_size}/{total_size} bytes)", end="")
            print("\n[INFO] 다운로드 완료!")
            return True
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] 다운로드 실패: {e}")
        return False

def calculate_file_sha256(filepath):
    sha256_hash = hashlib.sha256()
    try:
        with open(filepath, "rb") as f:
            for byte_block in iter(lambda: f.read(4096), b""):
                sha256_hash.update(byte_block)
        return sha256_hash.hexdigest()
    except FileNotFoundError:
        print(f"[ERROR] SHA256 계산을 위한 파일을 찾을 수 없습니다: {filepath}")
        return None

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
    global vehicle_private_key, server_public_key # 전역 키 사용

    if not vehicle_private_key or not server_public_key:
        print("[ERROR] 펌웨어 처리 전에 개인/공개 키가 로드되지 않았습니다.")
        return None, None, None, None, None

    try:
        with open(filepath, 'rb') as f:
            header_format = "<IIBBH32s8s"
            header_size = struct.calcsize(header_format)
            header_data = f.read(header_size)
            if len(header_data) < header_size:
                print("[ERROR] 헤더 데이터가 너무 짧습니다.")
                return None, None, None, None, None
            
            magic, timestamp, ecu_id, version, code_len, sha256_from_header, aes_nonce = struct.unpack(header_format, header_data)
            print(f"[INFO] 헤더 정보: Magic={hex(magic)}, Timestamp={timestamp}, ECU_ID={ecu_id}, Version={version}")

            current_time = int(time.time())
            if current_time - timestamp > FIRMWARE_EXPIRATION_SECONDS:
                print(f"[ERROR] 최신성 검증 실패! 펌웨어가 너무 오래되었습니다 (만료됨). 생성 시각: {timestamp}")
                return None, None, None, None, None

            encrypted_aes_key = f.read(256)
            signature = f.read(256)
            encrypted_code = f.read()
            
            if len(encrypted_aes_key) != 256 or len(signature) != 256 or len(encrypted_code) != code_len:
                print("[ERROR] 파일 구조가 손상되었거나 길이가 헤더 정보와 일치하지 않습니다.")
                return None, None, None, None, None

            cipher_rsa = PKCS1_OAEP.new(vehicle_private_key)
            decrypted_aes_key = cipher_rsa.decrypt(encrypted_aes_key)
            print("[INFO] 차량 개인키로 AES 세션 키 복호화 완료.")
            
            cipher_aes = AES.new(decrypted_aes_key, AES.MODE_CTR, nonce=aes_nonce)
            decrypted_code = cipher_aes.decrypt(encrypted_code)
            print("[INFO] AES로 펌웨어 코드 복호화 완료.")

            hash_obj_verify = SHA256.new(decrypted_code)
            if hash_obj_verify.digest() != sha256_from_header:
                print("[ERROR] SHA256 해시 검증 실패! 파일이 변조되었을 수 있습니다.")
                return None, None, None, None, None
            print("[SUCCESS] SHA256 해시 검증 성공 (무결성 확인).")

            pkcs1_15.new(server_public_key).verify(hash_obj_verify, signature)
            print("[SUCCESS] RSA 전자서명 검증 성공 (인증 확인).")

            return decrypted_code, magic, ecu_id, version, code_len

    except Exception as e:
        print(f"[ERROR] 펌웨어 처리 중 심각한 오류 발생 (검증/복호화 실패 가능성 높음): {e}")
        return None, None, None, None, None

def wait_for_approval(timeout=30):
    if not canoe_inst: return False
    print("[INFO] 사용자 승인 대기 중...")
    start_time = time.time()
    while time.time() - start_time < timeout:
        flag = canoe_inst.get_system_variable_value('OTA::Ota_flag')
        if flag == 2:
            print("\n[INFO] 사용자 승인됨")
            return True
        elif flag == 3:
            print("\n[INFO] 사용자 거절됨")
            return False
        elif flag == 5:
            print("\n[INFO] OTA 업데이트 완료 (CANoe에서 먼저 신호 보냄)")
            return True
        time.sleep(0.1)
    print("\n[WARN] 승인 응답 없음 (시간 초과)")
    return False

def wait_for_update_complete(timeout=60, simulate_failure=False):
    if not canoe_inst: return False
    print("[INFO] OTA 업데이트 완료 대기 중...")
    start_time = time.time()
    while time.time() - start_time < timeout:
        flag = canoe_inst.get_system_variable_value('OTA::Ota_flag')
        
        if simulate_failure and (time.time() - start_time) > 3:
            if flag != 4:
                print("[WARN] 업데이트 실패 시뮬레이션: CANoe Ota_flag를 4 (업데이트 실패)로 설정.")
                canoe_inst.set_system_variable_value('OTA::Ota_flag', 4)
                time.sleep(1)
            return False

        if flag == 5:
            print("[SUCCESS] ECU로부터 업데이트 완료 신호 수신!")
            return True
        time.sleep(0.5)
    print("\n[WARN] OTA 업데이트 시간 초과")
    
    if not simulate_failure:
        if canoe_inst:
            print("[WARN] 시간 초과로 CANoe Ota_flag를 4 (업데이트 실패)로 설정.")
            canoe_inst.set_system_variable_value('OTA::Ota_flag', 4)
    return False

def wait_until_version_updated(ecu_id, expected_version, timeout=20):
    if not canoe_inst: return False
    print("[INFO] 버전 업데이트 확인 대기 중...")
    start_time = time.time()
    while time.time() - start_time < timeout:
        versions = collect_ecu_versions()
        ecu_name = {1: "CGW", 2: "TMU", 3: "EDT"}.get(ecu_id, None)
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

def report_to_server(session, versions, update_status="UNKNOWN"):
    url = f"{server_base_url}/report_versions"
    payload = {
        "vehicle_id": VEHICLE_ID,
        "timestamp": time.strftime("%Y-%m-%dT%H:%M:%SZ"),
        "ecus": versions,
        "update_status": update_status
    }
    headers = {
        'X-Vehicle-Token': VEHICLE_TOKEN
    }
    try:
        res = session.post(url, json=payload, headers=headers, timeout=5)
        print(f"[INFO] 서버 보고 완료: {res.status_code}, {res.text}")
    except Exception as e:
        print(f"[ERROR] 서버 보고 실패: {e}")

def backup_current_firmware_sim(ecu_id, current_version):
    backup_bin_filename = f"previous_firmware_ecu_{ecu_id}_ver_{current_version}.bin"
    selected_ecu_tuple = ecu_info[ecu_id-1]
    main_ecu_folder = selected_ecu_tuple[0]
    backup_subfolder = selected_ecu_tuple[1]
    backup_txt_filename = f"{selected_ecu_tuple[0]}_backup_version.txt"
    full_backup_directory_for_ecu = os.path.join(OTA_BASE_PROJECT_PATH, main_ecu_folder, backup_subfolder)
    os.makedirs(full_backup_directory_for_ecu, exist_ok=True) # 백업 폴더 생성 보장
    
    backup_bin_path = os.path.join(full_backup_directory_for_ecu, backup_bin_filename)
    backup_txt_path = os.path.join(full_backup_directory_for_ecu, backup_txt_filename)
    ecu_root_forder = os.path.join(OTA_BASE_PROJECT_PATH, main_ecu_folder)
    try:
        current_firmware_bin_path = os.path.join(ecu_root_forder, f"firmware_ecu_{ecu_id}_ver{current_version}.bin")
        current_firmware_txt_path = os.path.join(ecu_root_forder, f"{main_ecu_folder}_version.txt")
        if os.path.exists(current_firmware_bin_path):
            shutil.copy(current_firmware_bin_path, backup_bin_path)
            if os.path.exists(current_firmware_txt_path): # 텍스트 파일도 존재할 경우에만 복사
                shutil.copy(current_firmware_txt_path, backup_txt_path)
            print(f"[INFO] 현재 펌웨어 (가상) 백업 완료: {backup_bin_path}")
        else:
            with open(backup_bin_path, 'wb') as f:
                f.write(b'DUMMY_PREVIOUS_FIRMWARE_CONTENT')
            print(f"[INFO] 현재 펌웨어 (더미) 백업 생성: {backup_bin_path}")
    except Exception as e:
        print(f"[ERROR] 이전 펌웨어 백업 실패: {e}")

def restore_previous_firmware_sim(ecu_id, previous_version):
    backup_bin_filename = f"previous_firmware_ecu_{ecu_id}_ver_{previous_version}.bin"
    selected_ecu_tuple = ecu_info[ecu_id-1]
    main_ecu_folder = selected_ecu_tuple[0]
    backup_subfolder = selected_ecu_tuple[1]
    full_backup_directory_for_ecu = os.path.join(OTA_BASE_PROJECT_PATH, main_ecu_folder, backup_subfolder)
    full_restore_directory_for_ecu = os.path.join(OTA_BASE_PROJECT_PATH, main_ecu_folder)
    
    backup_path = os.path.join(full_backup_directory_for_ecu, backup_bin_filename)
    restore_path = os.path.join(full_restore_directory_for_ecu, f"firmware_restored_ecu_{ecu_id}_ver_{previous_version}.bin")
    
    if os.path.exists(backup_path):
        try:
            shutil.copy(backup_path, restore_path)
            # 복원 시 TXT 파일도 복원해야 할 경우 추가
            backup_txt_filename = f"{selected_ecu_tuple[0]}_backup_version.txt"
            restore_txt_path = os.path.join(full_restore_directory_for_ecu, f"{selected_ecu_tuple[0]}_version.txt")
            backup_txt_path = os.path.join(full_backup_directory_for_ecu, backup_txt_filename)
            if os.path.exists(backup_txt_path):
                shutil.copy(backup_txt_path, restore_txt_path)

            print(f"[INFO] 이전 펌웨어 (가상) 복원 완료: {restore_path}")
            return True
        except Exception as e:
            print(f"[ERROR] 이전 펌웨어 복원 실패: {e}")
            return False
    else:
        print(f"[ERROR] 복원할 이전 펌웨어 파일이 없습니다: {backup_path}")
        return False

# [추가된 함수 1] 롤백 후 정리 작업을 수행하는 함수 (기존 app.py에서 가져옴)
def perform_cleanup_and_finalize(ecu_id, next_version):
    """
    업데이트 폴더의 파일을 정리하고, 완료되면 CANoe 플래그를 9로 설정합니다.
    """
    selected_ecu_tuple = ecu_update_forder[ecu_id-1]
    main_ecu_folder = selected_ecu_tuple[0]
    update_subfolder = selected_ecu_tuple[1]
    full_update_directory_for_ecu = os.path.join(OTA_BASE_PROJECT_PATH, main_ecu_folder, update_subfolder)
    os.makedirs(full_update_directory_for_ecu, exist_ok=True) # 폴더 생성 보장

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
            print("\n[INFO] 정리 시작 신호 (flag=8) 수신. 정리 작업을 시작합니다.")
            return perform_cleanup_and_finalize(ecu_id, next_version) # 정리 함수 호출
        
        if int(time.time() - start_time) % 5 == 0:
            print(f"\r[INFO] 정리 신호(flag=8) 대기 중... ({int(time.time() - start_time)}s)", end="")
            time.sleep(1)
        time.sleep(0.5) # CPU 사용량을 줄이기 위해 짧은 대기
    print("\n[WARN] 정리 신호 대기 시간 초과.")
    return False

# === 펌웨어 다운로드 및 처리 함수 (기존 main 루프의 핵심 로직을 함수로 분리) ===
def download_and_process_firmware(session, update_info):
    """
    MQTT 알림을 통해 받은 정보로 펌웨어를 다운로드하고 처리합니다.
    """
    global downloaded_sha256 # SHA256 값 업데이트를 위해 전역 변수 사용

    latest_version_name = update_info['filename']
    latest_sha256 = update_info['sha256']
    ecu_id_from_mqtt = update_info.get('ecu_id', 0) # 기본값 설정
    new_version_from_mqtt = update_info.get('version', 0) # 기본값 설정
    download_url_from_mqtt = update_info['download_url']

    # 로드된 SHA 값과 MQTT 알림의 SHA 값을 비교하여 중복 처리
    current_downloaded_sha = get_current_downloaded_sha()
    if latest_sha256 == current_downloaded_sha:
        print(f"[INFO] 이미 최신 버전입니다. (SHA: {latest_sha256[:10]}...) MQTT 알림은 중복 알림일 수 있습니다.")
        return # 이미 최신 버전이면 여기서 종료

    print(f"[INFO] 새로운 펌웨어 발견 (MQTT 알림): {latest_version_name}")

    # Nonce 획득 (기존 로직 유지)
    nonce = get_nonce(session, VEHICLE_ID, VEHICLE_TOKEN)
    if not nonce:
        print("[ERROR] Nonce 획득 실패. 업데이트를 진행할 수 없습니다.")
        return

    download_url = f"{download_url_from_mqtt}?nonce={nonce}"

    session.headers.update({'X-Vehicle-Token': VEHICLE_TOKEN})
    local_filename = os.path.join(DOWNLOAD_FOLDER, latest_version_name)

    if download_file(session, download_url, local_filename):
        downloaded_file_sha256 = calculate_file_sha256(local_filename)
        if downloaded_file_sha256 != latest_sha256:
            print("[ERROR] 다운로드된 파일 무결성 검증 실패! 서버-클라이언트 SHA 불일치.")
            del session.headers['X-Vehicle-Token']
            if canoe_inst:
                canoe_inst.set_system_variable_value('OTA::Ota_flag', 4)
                current_versions_at_fail = collect_ecu_versions()
                report_to_server(session, current_versions_at_fail, update_status="DOWNLOAD_INTEGRITY_FAILED")
            return

        print("[SUCCESS] 다운로드된 파일 무결성 검증 완료.")
        decrypted_code, magic, ecu_id, new_version, code_len = process_secure_firmware(local_filename)
        
        # MQTT 알림의 ECU ID/버전과 실제 펌웨어의 ECU ID/버전 일치 여부 확인 (선택 사항)
        # 이 부분이 중요합니다. 서버에서 보낸 정보와 실제 펌웨어 내용이 일치해야 합니다.
        if ecu_id_from_mqtt != 0 and new_version_from_mqtt != 0: # 0은 기본값 (없음을 의미)
            if ecu_id != ecu_id_from_mqtt or new_version != new_version_from_mqtt:
                print(f"[WARN] MQTT 알림의 ECU ID/버전 ({ecu_id_from_mqtt}/{new_version_from_mqtt})과 펌웨어 내부 정보 ({ecu_id}/{new_version}) 불일치!")
                # 이 경우, 보안 정책에 따라 업데이트를 중단할지, 경고 후 진행할지 결정해야 합니다.
                # 여기서는 경고 후 진행하도록 하겠습니다.

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
                
                if wait_for_update_complete(60, simulate_failure=SIMULATE_UPDATE_FAILURE): 
                    if wait_until_version_updated(ecu_id, new_version):
                        print("[INFO] 최종 업데이트 성공. 새로운 버전 정보를 서버에 보고합니다.")
                        save_downloaded_sha(latest_sha256)
                        downloaded_sha256 = latest_sha256 # 전역 변수 업데이트
                        time.sleep(2)
                        current_versions = collect_ecu_versions()
                        report_to_server(session, current_versions, update_status="SUCCESS") 
                    else:
                        print("[ERROR] 업데이트는 완료 신호를 보냈으나, ECU 버전이 기대한 값으로 바뀌지 않았습니다. 롤백 시도.")
                        if canoe_inst:
                            canoe_inst.set_system_variable_value('OTA::Ota_flag', 6)
                        print("[INFO] 롤백 시뮬레이션 시작...")
                        time.sleep(1)
                        if restore_previous_firmware_sim(ecu_id, current_version_for_backup): 
                            print("[INFO] 롤백 시뮬레이션 성공. CANoe Ota_flag를 7 (롤백 완료)로 설정.")
                            if canoe_inst:
                                canoe_inst.set_system_variable_value('OTA::Ota_flag', 7)
                            
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
                        canoe_inst.set_system_variable_value('OTA::Ota_flag', 6)
                    print("[INFO] 롤백 시뮬레이션 시작...")
                    if restore_previous_firmware_sim(ecu_id, current_version_for_backup):
                        print("[INFO] 롤백 시뮬레이션 성공. CANoe Ota_flag를 7 (롤백 완료)로 설정.")
                        if canoe_inst:
                            canoe_inst.set_system_variable_value('OTA::Ota_flag', 7)
                        
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

# 현재 다운로드된 SHA256을 가져오는 함수 (load_downloaded_sha와 동일)
def get_current_downloaded_sha():
    """
    현재 클라이언트에 저장된 downloaded_sha.txt 파일의 SHA256 값을 반환합니다.
    """
    return load_downloaded_sha()

if __name__ == "__main__":
    if len(sys.argv) > 1:
        VEHICLE_ID = sys.argv[1]
    else:
        print("[ERROR] 차량 ID가 제공되지 않았습니다. 사용법: python download_file.py <VEHICLE_ID>")
        sys.exit(1)
    
    if VEHICLE_ID == "Sapaghetti":
        VEHICLE_TOKEN = "SECRET_API_KEY_FOR_Sapaghetti_VEHICLE"
    elif VEHICLE_ID == "VehicleB":
        VEHICLE_TOKEN = "ANOTHER_SECRET_API_KEY_FOR_VEHICLEB"
    else:
        print(f"[ERROR] 알 수 없는 차량 ID: {VEHICLE_ID}. 유효한 차량 ID를 제공하세요.")
        sys.exit(1)
    
    # 키 로딩은 차량 ID가 확정된 후에 수행
    load_keys(VEHICLE_ID)

    if os.path.exists(progress_file):
        os.remove(progress_file)

    session = requests.Session()
    # downloaded_sha256은 이제 download_and_process_firmware 함수 내부에서 업데이트됩니다.
    # 초기 값 로드는 유지합니다.
    downloaded_sha256 = load_downloaded_sha()

    # === MQTT 클라이언트 설정 및 연결 ===
    mqtt_client_instance = mqtt.Client()
    mqtt_client_instance.on_connect = on_connect
    mqtt_client_instance.on_message = on_message

    try:
        print(f"[MQTT] MQTT 브로커({MQTT_BROKER_HOST}:{MQTT_BROKER_PORT}) 연결 시도 중...")
        mqtt_client_instance.tls_set(
            ca_certs="C:\\vscodestudy\\OTA\\isrgrootx1.pem", # <--- 사용자 로컬 PC의 Let's Encrypt CA 인증서 경로!
            #ca_certs="C:\\vscodestudy\\OTA\\client_ca_chain.pem",
            certfile=None, # 상호 TLS를 사용하지 않는다면 None
            keyfile=None,  # 상호 TLS를 사용하지 않는다면 None
            tls_version=ssl.PROTOCOL_TLSv1_2,
            cert_reqs=ssl.CERT_REQUIRED
        )
        mqtt_client_instance.connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT, 60)
        mqtt_client_instance.loop_start() # 백그라운드 스레드에서 MQTT 메시지 처리 시작
    except Exception as e:
        print(f"[MQTT ERROR] MQTT 브로커 연결에 실패했습니다: {e}. OTA 알림이 작동하지 않을 수 있습니다.")
        # MQTT 연결 실패 시 대체 로직 (예: 기존 HTTP 폴링 유지)을 여기에 추가할 수 있습니다.
        # 이 예시에서는 MQTT 연결 실패 시 업데이트 알림을 받지 못하게 됩니다.

    print(f"\n--- OTA 클라이언트 시작 (차량 ID: {VEHICLE_ID}) ---")
    print(f"--- MQTT 토픽 '{MQTT_UPDATE_TOPIC}'에서 업데이트 알림 대기 중 ---")
    print(f"--- 펌웨어 다운로드는 HTTP/HTTPS를 통해 이루어집니다 ---")

    while True:
        # MQTT 메시지 큐 확인 및 처리
        try:
            # 큐에서 메시지를 non-blocking 방식으로 가져옴 (0.1초 대기)
            mqtt_payload_str = mqtt_message_queue.get(timeout=0.1) 
            print(f"[INFO] 큐에서 MQTT 메시지 가져옴: {mqtt_payload_str[:50]}...")
            update_info = json.loads(mqtt_payload_str)
            
            # 펌웨어 다운로드 및 처리 함수 호출
            download_and_process_firmware(session, update_info)

        except queue.Empty:
            # 큐가 비어있으면 (새 MQTT 메시지가 없으면) 계속 진행
            pass
        except json.JSONDecodeError as e:
            print(f"[MQTT ERROR] 수신된 메시지 파싱 오류: {e}. 메시지 내용: {mqtt_payload_str}")
        except Exception as e:
            print(f"[ERROR] MQTT 메시지 처리 중 알 수 없는 오류 발생: {e}")

        # CANoe Ota_flag = 8 감지 로직은 메인 루프 하단에 유지
        if canoe_inst:
            current_ota_flag = canoe_inst.get_system_variable_value('OTA::Ota_flag')
            if current_ota_flag == 8:
                print("[INFO] 메인 루프에서 CANoe의 OTA::Ota_flag=8 신호 감지. 파일 정리 시작.")
                
                # `download_and_process_firmware` 함수에서 생성된 파일 경로를
                # 전역 변수나 다른 방식으로 관리하지 않으므로, UPDATE_FOLDER 전체를 정리하는 로직을 사용합니다.
                # 이는 `perform_cleanup_and_finalize` 함수가 특정 ECU ID/버전에 종속되지 않고
                # UPDATE_FOLDER의 모든 파일을 정리하도록 설계되어 있기 때문입니다.
                # (만약 각 시나리오별로 생성된 특정 파일만 삭제하고 싶다면, 해당 파일 경로를 유지해야 함)
                
                print(f"[WARN] OTA::Ota_flag=8 정리 로직이 일반적인 UPDATE_FOLDER 전체 정리를 수행합니다. 특정 파일 정리 필요 시 로직 수정 요망.")
                # perform_cleanup_and_finalize 함수는 ecu_id, next_version 인자를 받지만,
                # 이 위치에서는 특정 업데이트와 직접 연결되지 않으므로 더미 값을 전달합니다.
                # perform_cleanup_and_finalize는 UPDATE_FOLDER 전체를 정리하도록 되어 있습니다.
                perform_cleanup_and_finalize(1, 0) # 더미 ECU ID와 버전

                canoe_inst.set_system_variable_value('OTA::Ota_flag', 0)
                print("[INFO] OTA::Ota_flag를 0으로 리셋했습니다.")
        
        # 주기적인 대기 (메시지가 없으면 대기)
        time.sleep(0.5)
