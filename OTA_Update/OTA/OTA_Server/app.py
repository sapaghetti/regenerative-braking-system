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
from login_form import LoginForm
from flask_session import Session
import logging
from logging.handlers import RotatingFileHandler
from werkzeug.utils import secure_filename
import sys
from pythonjsonlogger import jsonlogger

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

# TODO: 고급 로깅 및 모니터링 - 구조화된 로깅(JSON 형식)을 도입하고 Cloud Logging과 같은 중앙 집중식 시스템과 연동을 고려하세요.

# Nonce 만료 시간 설정 (예: 5분)
NONCE_EXPIRATION_SECONDS = 5 * 60 # 5분

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

    # 충돌 방지를 위해 'filename' 대신 'file_name' 사용
    # kwargs에서 'filename'이 전달되면 'file_name'으로 변경
    if 'filename' in kwargs:
        kwargs['file_name'] = kwargs.pop('filename') # 'filename'을 'file_name'으로 변경
    
    # 추가적인 kwargs 필드 포함
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
    return render_template('upload.html')

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
        filename = secure_filename(file.filename)
        file_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
        try:
            file.save(file_path)
            write_audit_log(event="UPLOAD", status="SUCCESS", filename=filename, user=session.get('username'))
            
            return jsonify(message="Upload Complete!"), 200
        except Exception as e:
            write_audit_log(event="UPLOAD", status="CRITICAL_FAILURE", filename=filename, error=str(e), user=session.get('username'))
            return jsonify(error=f"File save failed: {str(e)}"), 500
    
    write_audit_log(event="UPLOAD", status="FAILURE", reason="Unknown", user=session.get('username'))
    return jsonify(error="File upload failed"), 500

@app.route('/upload/<filename>')
def download_file(filename):
    if not session.get('logged_in') or session.get('role') != 'admin':
        abort(403)
    try:
        write_audit_log(event="WEB_DOWNLOAD", status="SUCCESS", filename=filename, user=session.get('username'))
        return send_from_directory(app.config['UPLOAD_FOLDER'], filename, as_attachment=True)
    except FileNotFoundError:
        write_audit_log(event="WEB_DOWNLOAD", status="FAILURE", filename=filename, reason="File not found", user=session.get('username'))
        abort(404)

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
    firmware_files = glob.glob(os.path.join(app.config['UPLOAD_FOLDER'], '*.bin'))

    if not firmware_files:
        write_audit_log(event="LATEST_VERSION_CHECK", status="SUCCESS", reason="No firmware files found")
        return jsonify(version=None, mtime=None, sha256=None)

    latest_file_path = max(firmware_files, key=os.path.getmtime)
    
    latest_version_filename = os.path.basename(latest_file_path)
    latest_mtime = os.path.getmtime(latest_file_path)
    latest_sha256 = calculate_sha256(latest_file_path)

    # 'filename' 대신 'file_name' 사용
    write_audit_log(event="LATEST_VERSION_CHECK", status="SUCCESS", file_name=latest_version_filename, sha256=latest_sha256[:10])
    return jsonify(
        version=latest_version_filename,
        mtime=latest_mtime,
        sha256=latest_sha256
    )

@app.route('/ota_download/<filename>')
def ota_download_file(filename):
    client_nonce = request.args.get('nonce')
    
    token = request.headers.get('X-Vehicle-Token')
    if not token:
        write_audit_log(event="OTA_DOWNLOAD", status="FAILURE", reason="No Token", file_name=filename) # <-- 변경
        return jsonify({"error": "Authentication token is missing"}), 401
    
    nonces_data = load_nonces()
    stored_nonce_info = nonces_data.get(client_nonce)

    if not stored_nonce_info:
        write_audit_log(event="OTA_DOWNLOAD", status="FAILURE", reason="Invalid or missing nonce", nonce=client_nonce, file_name=filename) # <-- 변경
        return jsonify(error="Invalid or missing nonce"), 403

    nonce_timestamp = stored_nonce_info.get('timestamp')
    if not nonce_timestamp or (datetime.now().timestamp() - nonce_timestamp) > NONCE_EXPIRATION_SECONDS:
        del nonces_data[client_nonce]
        save_nonces(nonces_data)
        write_audit_log(event="OTA_DOWNLOAD", status="FAILURE", reason="Expired nonce", nonce=client_nonce, file_name=filename) # <-- 변경
        return jsonify(error="Expired nonce"), 403

    if stored_nonce_info.get('used'):
        write_audit_log(event="OTA_DOWNLOAD", status="FAILURE", reason="Nonce already used", nonce=client_nonce, file_name=filename) # <-- 변경
        return jsonify(error="Nonce already used"), 403
    
    requesting_vehicle_id = stored_nonce_info.get('vehicle_id')
    all_vehicles = load_vehicles()
    authenticated_vehicle_id = None
    for vid, v_info in all_vehicles.items():
        if v_info.get('token') == token:
            authenticated_vehicle_id = vid
            break

    if not authenticated_vehicle_id or requesting_vehicle_id != authenticated_vehicle_id:
        write_audit_log(event="OTA_DOWNLOAD", status="FAILURE", reason="Nonce vehicle ID mismatch or authentication failed", nonce=client_nonce, req_vid=authenticated_vehicle_id, stored_vid=requesting_vehicle_id, file_name=filename) # <-- 변경
        return jsonify(error="Nonce vehicle ID mismatch or authentication failed"), 403


    nonces_data[client_nonce]['used'] = True
    del nonces_data[client_nonce] 
    save_nonces(nonces_data)
    
    try:
        if not os.path.exists(os.path.join(app.config['UPLOAD_FOLDER'], filename)):
            write_audit_log(event="OTA_DOWNLOAD", status="FAILURE", reason="File not found on server", file_name=filename) # <-- 변경
            return jsonify(error="File not found on server"), 404

        write_audit_log(event="OTA_DOWNLOAD", status="SUCCESS", file_name=filename, vehicle_id=requesting_vehicle_id) # <-- 변경
        return send_from_directory(app.config['UPLOAD_FOLDER'], filename, as_attachment=True)
    except Exception as e:
        write_audit_log(event="OTA_DOWNLOAD", status="CRITICAL_FAILURE", file_name=filename, error=str(e), vehicle_id=requesting_vehicle_id) # <-- 변경
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

        # 새로운 'update_status' 필드를 payload에서 가져옵니다.
        # 클라이언트가 이 필드를 보낼 것이므로, 기본값은 "SUCCESS" 또는 "UNKNOWN"으로 설정합니다.
        update_status = data.get('update_status', 'SUCCESS') # <-- update_status 가져오기

        write_audit_log(event="VERSION_REPORT", status=update_status, vehicle_id=vin, reported_versions=data['ecus']) # <-- 로깅 변경
        return jsonify({"status": "saved"}), 200

    except Exception as e:
        write_audit_log(event="VERSION_REPORT", status="CRITICAL_FAILURE", error=str(e), request_data=str(request.get_json(silent=True)))
        return jsonify({"error": str(e)}), 500
#if __name__ == "__main__":
#    app.run(host='0.0.0.0', port=5000, debug=True)
