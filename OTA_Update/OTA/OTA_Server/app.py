# app.py

from flask import Flask, request, abort, redirect, render_template, url_for, flash, send_from_directory, jsonify, session
from werkzeug.middleware.proxy_fix import ProxyFix
import os
import glob
import json
import bcrypt
import secrets
import hashlib
from flask_wtf import CSRFProtect
from datetime import timedelta, datetime
from login_form import LoginForm # login_form.py 파일이 있다고 가정
from flask_session import Session # Flask-Session이 설치되어 있다고 가정
import logging
from logging.handlers import RotatingFileHandler
from werkzeug.utils import secure_filename
import sys
from pythonjsonlogger import jsonlogger
import time # 타임스탬프 사용을 위해 추가
import re # 파일명 파싱을 위한 정규표현식 모듈 추가

# MQTT 및 암호화 관련 라이브러리 추가
import paho.mqtt.publish as publish
import struct
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto.Random import get_random_bytes

# Google Cloud Secret Manager 관련 임포트
from google.cloud import secretmanager

# --- 중요 보안 권장 사항 ---
# 실제 운영 환경에서는 반드시 HTTPS를 적용하여 서버와 차량 간의 통신을 암호화해야 합니다.
# 이는 논문에서 언급된 '기밀성(Confidentiality)' 요구사항을 충족하기 위해 필수적입니다.
# Flask 자체 개발 서버는 프로덕션용이 아니므로, Gunicorn, Nginx 등의 WSGI 서버와 함께 사용해야 합니다.
# ---

app = Flask(__name__)
app.wsgi_app = ProxyFix(app.wsgi_app, x_for=1, x_host=1, x_prefix=1)

# TODO: 보안 강화 - app.secret_key는 환경 변수나 Google Secret Manager를 통해 관리해야 합니다.
# 코드에 하드코딩하는 것은 매우 위험합니다.
app.secret_key = 'secret_key'
app.config['WTF_CSRF_SECRET_KEY'] = app.secret_key

csrf = CSRFProtect(app)

app.permanent_session_lifetime = timedelta(minutes=10)

UPLOAD_FOLDER = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'upload')
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

USER_FILE = 'users.json'
VEHICLE_FILE = 'vehicles.json'
NONCE_FILE = 'nonces.json'
# TODO: 견고성 및 안정성 - JSON 파일 대신 PostgreSQL, MySQL 등의 데이터베이스를 도입해야 합니다.
# 이는 데이터의 지속성, 무결성, 동시성 처리에 필수적입니다.

AUDIT_LOG_FILE = 'audit.log'
audit_logger = logging.getLogger('audit')
audit_logger.setLevel(logging.INFO)

logHandler = RotatingFileHandler(AUDIT_LOG_FILE, maxBytes=5 * 1024 * 1024, backupCount=5)
formatter = jsonlogger.JsonFormatter('%(asctime)s %(levelname)s %(name)s %(message)s')
logHandler.setFormatter(formatter)
audit_logger.addHandler(logHandler)

# Nonce 만료 시간 설정 (예: 5분)
NONCE_EXPIRATION_SECONDS = 5 * 60 # 5분

# === MQTT 브로커 설정 ===
# TODO: 실제 MQTT 브로커 주소와 포트로 변경하세요!
MQTT_BROKER_HOST = "127.0.0.1" 
MQTT_BROKER_PORT = 1883 # 일반적으로 비보안 MQTT 포트 (TLS/SSL 사용 시 8883 등)

# MQTT 토픽 설정
MQTT_TOPIC_UPDATE_AVAILABLE = "ota/update/available" # 새 펌웨어 알림 토픽

# === 펌웨어 보안 처리 관련 설정값 ===
# make_bin_file_hybrid.py에서 가져옴
MAGIC = 0xDEADBEEF
# TODO: ECU_ID 및 VERSION은 파일명에서 동적으로 가져오도록 변경되었습니다.
# 따라서 DEFAULT_ECU_ID 및 DEFAULT_VERSION 변수는 더 이상 사용하지 않습니다.

# 파일명에서 ECU_ID와 VERSION을 추출하기 위한 정규 표현식
# 예: firmware_ECU03_V06.bin
FILENAME_PATTERN = re.compile(r'.*ECU(\d+)_V(\d+)\.bin$')

# === Google Cloud Secret Manager 설정 ===
# TODO: 여기에 당신의 GCP 프로젝트 ID를 입력하세요!
GCP_PROJECT_ID = "thematic-grin-463106-m2"
SERVER_PRIVATE_KEY_SECRET_ID = "ota-server-private-key" # Secret Manager에 저장한 서버 개인키 비밀 이름
VEHICLE_PUBLIC_KEY_FILE = "public.pem" # 차량의 공개키 파일 경로

# 전역 변수로 키 인스턴스 저장 (최초 1회 로드 후 재사용)
_server_private_key = None
_vehicle_public_key = None

def write_audit_log(event, status="SUCCESS", **kwargs):
    # 공통 필드
    log_data = {
        "event": event,
        "status": status,
        "timestamp": datetime.now().isoformat(),
        "ip": request.remote_addr
    }
    
    # 세션 정보 (로그인 되어 있을 경우)
    if session.get('logged_in'):
        log_data["user"] = session.get('username')
        log_data["role"] = session.get('role')

    # 'filename' 대신 'file_name' 사용
    if 'filename' in kwargs:
        kwargs['file_name'] = kwargs.pop('filename')
    
    # 'message' 키 충돌 방지: 'message'를 'log_details'로 변경
    if 'message' in kwargs:
        log_data['log_details'] = kwargs.pop('message') # 'message' 키를 'log_details'로 변경하여 충돌 회피
    
    # 나머지 kwargs 필드 포함
    log_data.update(kwargs)

    audit_logger.info("Audit log entry", extra=log_data)

def load_users():
    if not os.path.exists(USER_FILE):
        initial_users = {
            "admin": {"password": bcrypt.hashpw("admin_password".encode('utf-8'), bcrypt.gensalt()).decode('utf-8'), "role": "admin"}
        }
        with open(USER_FILE, 'w') as f:
            json.dump(initial_users, f, indent=4)
    with open(USER_FILE, 'r') as f:
        return json.load(f)

def load_vehicles():
    if not os.path.exists(VEHICLE_FILE):
        sample_vehicles = {
            "Sapaghetti": {
                "token": "SECRET_API_KEY_FOR_Sapaghetti_VEHICLE"
            }
        }
        with open(VEHICLE_FILE, 'w') as f:
            json.dump(sample_vehicles, f, indent=4)
        return sample_vehicles
    with open(VEHICLE_FILE, 'r') as f:
        return json.load(f)

def load_nonces():
    if not os.path.exists(NONCE_FILE):
        return {}
    try:
        with open(NONCE_FILE, 'r') as f:
            content = f.read()
            if not content:
                return {}
            return json.loads(content)
    except json.JSONDecodeError as e:
        print(f"[ERROR] nonces.json 파일을 읽는 중 오류 발생: {e}. 파일을 초기화합니다.", file=sys.stderr)
        return {}

def save_nonces(nonces_data):
    with open(NONCE_FILE, 'w') as f:
        json.dump(nonces_data, f, indent=4)

def load_server_private_key():
    """
    Google Secret Manager 또는 로컬 파일에서 서버의 개인 키를 로드합니다.
    """
    global _server_private_key
    if _server_private_key:
        return _server_private_key # 이미 로드된 키 재사용

    try:
        print("🌍 Secret Manager에서 서버 개인키를 로드 시도 중...")
        client = secretmanager.SecretManagerServiceClient()
        name = f"projects/{GCP_PROJECT_ID}/secrets/{SERVER_PRIVATE_KEY_SECRET_ID}/versions/latest"
        response = client.access_secret_version(request={"name": name})
        server_private_key_pem = response.payload.data.decode("utf-8")
        _server_private_key = RSA.import_key(server_private_key_pem)
        print("✅ 서버 개인키를 Secret Manager에서 성공적으로 로드했습니다.")
    except Exception as e:
        print(f"❌ Secret Manager에서 서버 개인키를 로드하는 데 실패했습니다: {e}")
        print("❗ 대체: 로컬 파일 'private.pem'에서 개인키를 로드합니다. 이 방식은 운영 환경에서 권장되지 않습니다.")
        if not os.path.exists("private.pem"):
            print("❌ 로컬 'private.pem' 파일도 찾을 수 없습니다. 개인키 없이는 펌웨어를 생성할 수 없습니다.")
            return None
        with open("private.pem", "rb") as f:
            _server_private_key = RSA.import_key(f.read())
        print("✅ 로컬 'private.pem'에서 개인키를 로드했습니다. (보안 경고!)")
    return _server_private_key

def load_vehicle_public_key():
    """
    로컬 파일에서 차량의 공개 키를 로드합니다.
    """
    global _vehicle_public_key
    if _vehicle_public_key:
        return _vehicle_public_key # 이미 로드된 키 재사용

    if not os.path.exists(VEHICLE_PUBLIC_KEY_FILE):
        print(f"❌ '{VEHICLE_PUBLIC_KEY_FILE}' 파일을 찾을 수 없습니다. 차량의 공개키 파일이 있어야 합니다.")
        return None
    with open(VEHICLE_PUBLIC_KEY_FILE, "rb") as f:
        _vehicle_public_key = RSA.import_key(f.read())
    print(f"✅ '{VEHICLE_PUBLIC_KEY_FILE}'에서 차량의 공개키를 로드했습니다.")
    return _vehicle_public_key

@app.route('/login', methods=['GET', 'POST'])
def login():
    form = LoginForm()
    users = load_users()
    if form.validate_on_submit():
        username = form.username.data
        password = form.password.data.encode('utf-8')
        if username in users:
            stored_hash = users[username]['password'].encode('utf-8')
            if bcrypt.checkpw(password, stored_hash):
                session.permanent = True
                session['logged_in'] = True
                session['username'] = username
                session['role'] = users[username]['role']
                write_audit_log(event="LOGIN", status="SUCCESS", user=username)
                return redirect(url_for('upload_form'))
        
        write_audit_log(event="LOGIN", status="FAILURE", user=username)
        return redirect(url_for('login', error='1', prev_username=username))    
    
    prev_username = request.args.get('prev_username')
    if prev_username:
        form.username.data = prev_username
    return render_template('login.html', form=form)

@app.route('/logout')
def logout():
    write_audit_log(event="LOGOUT", user=session.get('username'))
    session.pop('logged_in', None)
    session.pop('username', None)
    session.pop('role', None)
    return redirect(url_for('login'))

@app.route('/')
def upload_form():
    if not session.get('logged_in'):
        return redirect(url_for('login'))
    # 파일 목록을 전달하여 upload.html에 표시
    firmware_files = glob.glob(os.path.join(app.config['UPLOAD_FOLDER'], '*_secure_hybrid.bin'))
    # 파일명만 추출하여 전달
    files = [os.path.basename(f) for f in firmware_files]
    return render_template('upload.html', files=files) # files 변수를 추가하여 전달

@app.route('/upload', methods=['POST'])
def upload_file():
    if not session.get('logged_in'):
        write_audit_log(event="UPLOAD", status="FAILURE", reason="User not logged in")
        return jsonify(error="User not logged in"), 401
    
    if session.get('role') != 'admin':
        write_audit_log(event="UPLOAD", status="FAILURE", reason="Admin privileges required", user=session.get('username'))
        return jsonify(error="Admin privileges required"), 403
    
    if 'file' not in request.files:
        write_audit_log(event="UPLOAD", status="FAILURE", reason="No file part", user=session.get('username'))
        return jsonify(error="No file part"), 400
    
    file = request.files['file']
    
    if file.filename == '':
        write_audit_log(event="UPLOAD", status="FAILURE", reason="No selected file", user=session.get('username'))
        return jsonify(error="No selected file"), 400
    
    if file:
        original_filename = secure_filename(file.filename)
        original_filepath = os.path.join(app.config['UPLOAD_FOLDER'], original_filename)
        
        try:
            # 1. 원본 펌웨어 파일 저장 (임시로)
            file.save(original_filepath)
            print(f"[INFO] 원본 펌웨어 파일 저장 완료: {original_filepath}")

            # 2. 파일명에서 ECU_ID와 VERSION 추출
            match = FILENAME_PATTERN.match(original_filename)
            if not match:
                os.remove(original_filepath) # 유효하지 않은 파일명 삭제
                write_audit_log(event="UPLOAD", status="FAILURE", reason="Invalid filename format (ECU ID, Version not found)", filename=original_filename, user=session.get('username'))
                return jsonify(error="파일명 형식이 올바르지 않습니다. 'firmware_ECU<ID>_V<VERSION>.bin' 형식이어야 합니다."), 400
            
            extracted_ecu_id = int(match.group(1)) # 첫 번째 캡처 그룹 (ECU ID)
            extracted_version = int(match.group(2)) # 두 번째 캡처 그룹 (Version)
            
            print(f"[INFO] 파일명에서 추출된 ECU ID: {extracted_ecu_id}, 버전: {extracted_version}")

            # 3. 보안 펌웨어 생성 로직 시작 (make_bin_file_hybrid.py 로직 통합)
            server_private_key = load_server_private_key()
            vehicle_public_key = load_vehicle_public_key()

            if not server_private_key or not vehicle_public_key:
                # 키 로딩 실패 시 에러 처리
                os.remove(original_filepath) # 원본 파일 삭제
                write_audit_log(event="UPLOAD", status="FAILURE", reason="Key loading failed for secure firmware creation", user=session.get('username'))
                return jsonify(error="보안 펌웨어 생성 실패: 키 문제"), 500

            # 원본 펌웨어 코드 읽기
            with open(original_filepath, "rb") as f_orig:
                CODE = f_orig.read()
            CODE_LEN = len(CODE)
            
            # === 펌웨어 해시 (무결성 검증용) ===
            hash_obj = SHA256.new(CODE)
            firmware_hash = hash_obj.digest() # 32 bytes

            # === 전자 서명 (인증) ===
            signer = pkcs1_15.new(server_private_key)
            signature = signer.sign(hash_obj) # 256 bytes (RSA2048)

            # === AES 암호화 (기밀성) ===
            aes_key = get_random_bytes(16)  # 128bit 임시 세션 키
            aes_nonce = get_random_bytes(8)  # CTR용 nonce
            cipher_aes = AES.new(aes_key, AES.MODE_CTR, nonce=aes_nonce)
            encrypted_code = cipher_aes.encrypt(CODE)

            # === AES 키를 차량의 공개키로 암호화 ===
            cipher_rsa = PKCS1_OAEP.new(vehicle_public_key)
            encrypted_aes_key = cipher_rsa.encrypt(aes_key) # 256 bytes (RSA-2048)

            # === 헤더 구성 ===
            # ECU_ID와 VERSION을 파일명에서 추출한 값으로 사용
            header = struct.pack("<IIBBH32s8s256s256s",
                                 MAGIC,
                                 int(time.time()), # 현재 타임스탬프
                                 extracted_ecu_id, # <-- 파일명에서 추출한 ECU ID 사용
                                 extracted_version, # <-- 파일명에서 추출한 버전 사용
                                 CODE_LEN,
                                 firmware_hash,
                                 aes_nonce,
                                 encrypted_aes_key,
                                 signature)
            
            secure_filename_output = original_filename.replace('.bin', '_secure_hybrid.bin') # 새 파일명
            # 만약 .bin 확장자가 없으면 그냥 _secure_hybrid.bin 추가
            if not secure_filename_output.endswith('_secure_hybrid.bin'):
                secure_filename_output += '_secure_hybrid.bin'

            secure_filepath = os.path.join(app.config['UPLOAD_FOLDER'], secure_filename_output)

            # === 최종 보안 바이너리 파일 생성 ===
            with open(secure_filepath, "wb") as f_secure:
                f_secure.write(header)
                f_secure.write(encrypted_code)
            print(f"[INFO] 보안 펌웨어 파일 생성 완료: {secure_filepath}")
            os.remove(original_filepath) # 원본 파일 삭제 (보안상 권장)

            # 4. MQTT 알림 발행
            try:
                mqtt_payload = {
                    "filename": secure_filename_output,
                    "sha256": calculate_sha256(secure_filepath), # 새로 생성된 파일의 SHA256
                    "ecu_id": extracted_ecu_id, # <-- 파일명에서 추출한 ECU ID 사용
                    "version": extracted_version, # <-- 파일명에서 추출한 버전 사용
                    "timestamp": int(time.time()),
                    "download_url": f"{request.url_root.replace('http://', 'https://')}ota_download/{secure_filename_output}"
                }
                
                # publish.single 함수 사용 (인증 없는 단순 발행)
                # 실제 환경에서는 MQTT 클라이언트 인스턴스를 유지하고
                # publish.single 대신 client.publish()를 사용하는 것이 더 좋습니다.
                publish.single(
                    MQTT_TOPIC_UPDATE_AVAILABLE,
                    json.dumps(mqtt_payload),
                    hostname=MQTT_BROKER_HOST,
                    port=MQTT_BROKER_PORT,
                    qos=1,       # 메시지 전달 신뢰성 (At least once)
                    retain=True  # 이 메시지를 브로커가 유지하도록 설정
                )
                
                # 로그 메시지를 'log_details'로 전달하여 충돌 회피
                write_audit_log(event="UPLOAD", status="SUCCESS", filename=secure_filename_output, user=session.get('username'), log_details="Secure firmware created and MQTT notification sent.", ecu_id=extracted_ecu_id, version=extracted_version)
                
                # 웹 응답은 성공 메시지로 반환
                return jsonify(message=f"보안 펌웨어 '{secure_filename_output}' 생성 및 MQTT 알림 전송 완료! (ECU ID: {extracted_ecu_id}, 버전: {extracted_version})"), 200

            except Exception as mqtt_e:
                write_audit_log(event="UPLOAD", status="FAILURE", filename=secure_filename_output, reason=f"MQTT notification failed: {str(mqtt_e)}", user=session.get('username'))
                return jsonify(error=f"파일은 업로드 및 보안 처리되었으나, MQTT 알림 전송에 실패했습니다: {str(mqtt_e)}"), 500

        except Exception as e:
            # 파일 저장 또는 보안 처리 중 오류 발생
            if os.path.exists(original_filepath):
                os.remove(original_filepath) # 원본 파일이 남아있다면 삭제
            write_audit_log(event="UPLOAD", status="CRITICAL_FAILURE", filename=original_filename, error=str(e), user=session.get('username'))
            return jsonify(error=f"파일 업로드 및 보안 처리 실패: {str(e)}"), 500
    
    write_audit_log(event="UPLOAD", status="FAILURE", reason="Unknown", user=session.get('username'))
    return jsonify(error="파일 업로드 실패"), 500

ALLOWED_IPS = ['127.0.0.1', '192.168.0.100', '192.168.0.101', '112.218.95.58']
@app.before_request
def limit_remote_addr():
    if request.path.startswith('/static'):
        return
    client_ip = request.remote_addr
    if client_ip not in ALLOWED_IPS and not client_ip.startswith('127.0.0.1'):
        write_audit_log(event="IP_BLOCKED", status="FAILURE", ip=client_ip, reason="Forbidden IP")
        abort(403, "Access denied for your IP address.")

def calculate_sha256(filepath):
    sha256_hash = hashlib.sha256()
    with open(filepath, "rb") as f:
        for byte_block in iter(lambda: f.read(4096), b""):
            sha256_hash.update(byte_block)
    return sha256_hash.hexdigest()

@app.route('/latest_version')
def latest_version():
    # 이제 make_bin_file_hybrid.py에 의해 생성된 _secure_hybrid.bin 파일을 찾도록 수정
    firmware_files = glob.glob(os.path.join(app.config['UPLOAD_FOLDER'], '*_secure_hybrid.bin'))

    if not firmware_files:
        write_audit_log(event="LATEST_VERSION_CHECK", status="SUCCESS", reason="No secure firmware files found")
        return jsonify(version=None, mtime=None, sha256=None)

    latest_file_path = max(firmware_files, key=os.path.getmtime)
    
    latest_version_filename = os.path.basename(latest_file_path)
    latest_mtime = os.path.getmtime(latest_file_path)
    latest_sha256 = calculate_sha256(latest_file_path)

    # 파일명에서 ECU_ID와 VERSION을 추출하여 응답에 포함 (선택 사항, 클라이언트가 필요하다면)
    match = FILENAME_PATTERN.match(latest_version_filename)
    ecu_id_found = None
    version_found = None
    if match:
        ecu_id_found = int(match.group(1))
        version_found = int(match.group(2))

    write_audit_log(event="LATEST_VERSION_CHECK", status="SUCCESS", file_name=latest_version_filename, sha256=latest_sha256[:10], ecu_id=ecu_id_found, version=version_found)
    return jsonify(
        version=latest_version_filename,
        mtime=latest_mtime,
        sha256=latest_sha256,
        ecu_id=ecu_id_found, # 응답에 ECU ID 추가
        fw_version=version_found # 응답에 펌웨어 버전 추가 (key 이름은 fw_version으로 변경)
    )

@app.route('/get_nonce', methods=['POST'])
@csrf.exempt
def get_nonce():
    token = request.headers.get('X-Vehicle-Token')
    if not token:
        write_audit_log(event="GET_NONCE", status="FAILURE", reason="No Token", vehicle_id="N/A")
        return jsonify({"error": "Authentication token is missing"}), 401

    data = request.get_json()
    if not data or 'vehicle_id' not in data:
        write_audit_log(event="GET_NONCE", status="FAILURE", reason="Invalid data", request_data=str(data), token_present=True)
        return jsonify({"error": "Invalid data, vehicle_id required"}), 400
    
    vehicle_id = data['vehicle_id']
    all_vehicles = load_vehicles()

    if vehicle_id not in all_vehicles or all_vehicles[vehicle_id].get('token') != token:
        write_audit_log(event="GET_NONCE", status="FAILURE", reason="Authentication failed", vehicle_id=vehicle_id)
        return jsonify({"error": "Authentication failed"}), 403

    nonce = secrets.token_hex(16)
    nonces_data = load_nonces()
    
    nonces_data[nonce] = {
        "timestamp": datetime.now().timestamp(),
        "vehicle_id": vehicle_id,
        "used": False
    }
    save_nonces(nonces_data)
    
    write_audit_log(event="GET_NONCE", status="SUCCESS", nonce=nonce[:8] + "...", vehicle_id=vehicle_id)
    return jsonify({'nonce': nonce})

@app.route('/ota_download/<filename>')
def ota_download_file(filename):
    client_nonce = request.args.get('nonce')
    
    token = request.headers.get('X-Vehicle-Token')
    if not token:
        write_audit_log(event="OTA_DOWNLOAD", status="FAILURE", reason="No Token", file_name=filename)
        return jsonify({"error": "Authentication token is missing"}), 401
    
    nonces_data = load_nonces()
    stored_nonce_info = nonces_data.get(client_nonce)

    if not stored_nonce_info:
        write_audit_log(event="OTA_DOWNLOAD", status="FAILURE", reason="Invalid or missing nonce", nonce=client_nonce, file_name=filename)
        return jsonify(error="Invalid or missing nonce"), 403

    nonce_timestamp = stored_nonce_info.get('timestamp')
    if not nonce_timestamp or (datetime.now().timestamp() - nonce_timestamp) > NONCE_EXPIRATION_SECONDS:
        del nonces_data[client_nonce]
        save_nonces(nonces_data)
        write_audit_log(event="OTA_DOWNLOAD", status="FAILURE", reason="Expired nonce", nonce=client_nonce, file_name=filename)
        return jsonify(error="Expired nonce"), 403

    if stored_nonce_info.get('used'):
        write_audit_log(event="OTA_DOWNLOAD", status="FAILURE", reason="Nonce already used", nonce=client_nonce, file_name=filename)
        return jsonify(error="Nonce already used"), 403
    
    requesting_vehicle_id = stored_nonce_info.get('vehicle_id')
    all_vehicles = load_vehicles()
    authenticated_vehicle_id = None
    for vid, v_info in all_vehicles.items():
        if v_info.get('token') == token:
            authenticated_vehicle_id = vid
            break

    if not authenticated_vehicle_id or requesting_vehicle_id != authenticated_vehicle_id:
        write_audit_log(event="OTA_DOWNLOAD", status="FAILURE", reason="Nonce vehicle ID mismatch or authentication failed", nonce=client_nonce, req_vid=authenticated_vehicle_id, stored_vid=requesting_vehicle_id, file_name=filename)
        return jsonify(error="Nonce vehicle ID mismatch or authentication failed"), 403


    nonces_data[client_nonce]['used'] = True
    del nonces_data[client_nonce] 
    save_nonces(nonces_data)
    
    try:
        if not os.path.exists(os.path.join(app.config['UPLOAD_FOLDER'], filename)):
            write_audit_log(event="OTA_DOWNLOAD", status="FAILURE", reason="File not found on server", file_name=filename)
            return jsonify(error="File not found on server"), 404

        write_audit_log(event="OTA_DOWNLOAD", status="SUCCESS", file_name=filename, vehicle_id=requesting_vehicle_id)
        return send_from_directory(app.config['UPLOAD_FOLDER'], filename, as_attachment=True)
    except Exception as e:
        write_audit_log(event="OTA_DOWNLOAD", status="CRITICAL_FAILURE", file_name=filename, error=str(e), vehicle_id=requesting_vehicle_id)
        return jsonify(error=f"Server error during download: {str(e)}"), 500

@csrf.exempt
@app.route('/report_versions', methods=['POST'])
def report_versions():
    try:
        token = request.headers.get('X-Vehicle-Token')
        if not token:
            write_audit_log(event="VERSION_REPORT", status="FAILURE", reason="Authentication token is missing")
            return jsonify({"error": "Authentication token is missing"}), 401

        data = request.get_json()
        if not data or 'vehicle_id' not in data or 'ecus' not in data:
            write_audit_log(event="VERSION_REPORT", status="FAILURE", reason="Invalid data", request_data=str(data), token_present=True)
            return jsonify({"error": "Invalid data"}), 400

        vin = data['vehicle_id']
        all_vehicles = load_vehicles()

        if vin not in all_vehicles or all_vehicles[vin].get('token') != token:
            write_audit_log(event="VERSION_REPORT", status="FAILURE", reason="Authentication failed", vehicle_id=vin)
            return jsonify({"error": "Authentication failed"}), 403

        version_file = 'versions.json'
        if os.path.exists(version_file):
            with open(version_file, 'r') as f:
                all_versions = json.load(f)
        else:
            all_versions = {}
        all_versions[vin] = data['ecus']
        with open(version_file, 'w') as f:
            json.dump(all_versions, f, indent=4)

        update_status = data.get('update_status', 'SUCCESS')
        write_audit_log(event="VERSION_REPORT", status=update_status, vehicle_id=vin, reported_versions=data['ecus'])
        return jsonify({"status": "saved"}), 200

    except Exception as e:
        write_audit_log(event="VERSION_REPORT", status="CRITICAL_FAILURE", error=str(e), request_data=str(request.get_json(silent=True)))
        return jsonify({"error": str(e)}), 500

# if __name__ == "__main__":
#     app.run(host='0.0.0.0', port=5000, debug=True)
