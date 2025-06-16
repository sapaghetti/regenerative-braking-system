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
from datetime import timedelta
from login_form import LoginForm
from flask_session import Session
import logging
from datetime import datetime
from logging.handlers import RotatingFileHandler

# --- 중요 보안 권장 사항 ---
# 실제 운영 환경에서는 반드시 HTTPS를 적용하여 서버와 차량 간의 통신을 암호화해야 합니다.
# 이는 논문에서 언급된 '기밀성(Confidentiality)' 요구사항을 충족하기 위해 필수적입니다. 
# Flask 자체 개발 서버는 프로덕션용이 아니므로, Gunicorn, Nginx 등의 WSGI 서버와 함께 사용해야 합니다.
# ---

app = Flask(__name__)
app.wsgi_app = ProxyFix(app.wsgi_app, x_for=1, x_host=1, x_prefix=1)

app.secret_key = 'secret_key'
app.config['WTF_CSRF_SECRET_KEY'] = app.secret_key

csrf = CSRFProtect(app)

app.permanent_session_lifetime = timedelta(minutes=10)

UPLOAD_FOLDER = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'upload')
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

USER_FILE = 'users.json'
VEHICLE_FILE = 'vehicles.json' # ✅ 추가: 차량 인증 정보를 담을 파일

# Audit log 설정
AUDIT_LOG_FILE = 'audit.log'
audit_logger = logging.getLogger('audit')
audit_logger.setLevel(logging.INFO)
audit_handler = RotatingFileHandler(AUDIT_LOG_FILE, maxBytes=5 * 1024 * 1024, backupCount=5)
audit_formatter = logging.Formatter('%(asctime)s %(message)s', datefmt='%Y-%m-%d %H:%M:%S')
audit_handler.setFormatter(audit_formatter)
audit_logger.addHandler(audit_handler)

def write_audit_log(message):
    audit_logger.info(message)

# 최신 firmware cache
latest_version_cache = {
    'version': None,
    'mtime': None,
    'sha256': None
}

def load_users():
    with open(USER_FILE, 'r') as f:
        return json.load(f)
        
# ✅ 추가: 차량 정보 로드 함수
def load_vehicles():
    if not os.path.exists(VEHICLE_FILE):
        # 파일이 없으면 샘플 생성
        sample_vehicles = {
            "Sapaghetii": {
                "token": "SECRET_API_KEY_FOR_Sapaghetii_VEHICLE"
            }
        }
        with open(VEHICLE_FILE, 'w') as f:
            json.dump(sample_vehicles, f, indent=4)
        return sample_vehicles
    with open(VEHICLE_FILE, 'r') as f:
        return json.load(f)

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
                write_audit_log(f"[LOGIN SUCCESS] user={username}, ip={request.remote_addr}")
                return redirect(url_for('upload_form'))
        write_audit_log(f"[LOGIN FAIL] user={username}, ip={request.remote_addr}")
        return redirect(url_for('login') + '?error=1')
    return render_template('login.html', form=form)

@app.route('/logout')
def logout():
    write_audit_log(f"[LOGOUT] user={session.get('username')}, ip={request.remote_addr}")
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
        return jsonify(error="User not logged in"), 401
    if session.get('role') != 'admin':
        write_audit_log(f"[UPLOAD DENIED] user={session.get('username')}, ip={request.remote_addr}")
        return jsonify(error="Admin privileges required"), 403
    if 'file' not in request.files:
        write_audit_log(f"[UPLOAD FAIL - No file part] user={session.get('username')}, ip={request.remote_addr}")
        return jsonify(error="No file part"), 400
    file = request.files['file']
    if file.filename == '':
        write_audit_log(f"[UPLOAD FAIL - No filename] user={session.get('username')}, ip={request.remote_addr}")
        return jsonify(error="No selected file"), 400
    if file:
        file_path = os.path.join(app.config['UPLOAD_FOLDER'], file.filename)
        file.save(file_path)
        write_audit_log(f"[UPLOAD SUCCESS] file={file.filename}, user={session.get('username')}, ip={request.remote_addr}")
        latest_version_cache['version'] = file.filename
        latest_version_cache['mtime'] = os.path.getmtime(file_path)
        latest_version_cache['sha256'] = calculate_sha256(file_path)
        return jsonify(message="Upload Complete!"), 200
    write_audit_log(f"[UPLOAD FAIL - Unknown reason] user={session.get('username')}, ip={request.remote_addr}")
    return jsonify(error="File upload failed"), 500

# 이 함수는 관리자용 다운로드이므로 그대로 둠
@app.route('/upload/<filename>')
def download_file(filename):
    if not session.get('logged_in') or session.get('role') != 'admin':
        abort(403)
    try:
        return send_from_directory(app.config['UPLOAD_FOLDER'], filename, as_attachment=True)
    except FileNotFoundError:
        abort(404)

@app.route('/get_nonce', methods=['GET'])
def get_nonce():
    nonce = secrets.token_hex(16)
    session['ota_nonce'] = nonce
    return jsonify({'nonce': nonce})

ALLOWED_IPS = ['127.0.0.1', '192.168.0.100', '192.168.0.101', '112.218.95.58']
@app.before_request
def limit_remote_addr():
    if request.path.startswith('/static'):
        return
    client_ip = request.remote_addr
    # 로컬 개발 환경을 위해 로컬호스트는 항상 허용
    if client_ip not in ALLOWED_IPS and not client_ip.startswith('127.0.0.1'):
        write_audit_log(f"[IP BLOCKED] Forbidden IP: {client_ip}")
        abort(403, "Access denied for your IP address.")

def calculate_sha256(filepath):
    sha256_hash = hashlib.sha256()
    with open(filepath, "rb") as f:
        for byte_block in iter(lambda: f.read(4096), b""):
            sha256_hash.update(byte_block)
    return sha256_hash.hexdigest()

@app.route('/latest_version')
def latest_version():
    if latest_version_cache['version'] is None:
        firmware_files = glob.glob(os.path.join(UPLOAD_FOLDER, '*.bin')) # 모든 .bin 파일 대상
        if not firmware_files:
            return jsonify(version=None, mtime=None, sha256=None)
        latest_file = max(firmware_files, key=os.path.getmtime)
        latest_version_cache['version'] = os.path.basename(latest_file)
        latest_version_cache['mtime'] = os.path.getmtime(latest_file)
        latest_version_cache['sha256'] = calculate_sha256(latest_file)
    return jsonify(
        version=latest_version_cache['version'],
        mtime=latest_version_cache['mtime'],
        sha256=latest_version_cache['sha256']
    )

@app.route('/ota_download/<filename>')
def ota_download_file(filename):
    client_nonce = request.args.get('nonce')
    if not client_nonce or client_nonce != session.get('ota_nonce'):
        write_audit_log(f"[OTA DOWNLOAD FAIL - Invalid nonce] file={filename}, ip={request.remote_addr}")
        return jsonify(error="Invalid or missing nonce"), 403
    session.pop('ota_nonce', None)
    try:
        write_audit_log(f"[OTA DOWNLOAD SUCCESS] file={filename}, ip={request.remote_addr}")
        return send_from_directory(app.config['UPLOAD_FOLDER'], filename, as_attachment=True)
    except FileNotFoundError:
        write_audit_log(f"[OTA DOWNLOAD FAIL - FileNotFound] file={filename}, ip={request.remote_addr}")
        return jsonify(error="File not found"), 404

@csrf.exempt
@app.route('/report_versions', methods=['POST'])
def report_versions():
    # ✅ 변경: 차량 인증 로직 추가
    # 논문 요구사항: OEM은 차량의 진위 여부를 확인해야 함 (인증) 
    try:
        # 1. 헤더에서 인증 토큰 가져오기
        token = request.headers.get('X-Vehicle-Token')
        if not token:
            write_audit_log(f"[VERSION REPORT FAIL - No Token] ip={request.remote_addr}")
            return jsonify({"error": "Authentication token is missing"}), 401

        data = request.get_json()
        if not data or 'vehicle_id' not in data or 'ecus' not in data:
            return jsonify({"error": "Invalid data"}), 400

        vin = data['vehicle_id']
        all_vehicles = load_vehicles()

        # 2. 토큰 검증
        if vin not in all_vehicles or all_vehicles[vin].get('token') != token:
            write_audit_log(f"[VERSION REPORT FAIL - Invalid Token] vin={vin}, ip={request.remote_addr}")
            return jsonify({"error": "Authentication failed"}), 403
        
        # --- 인증 성공 후 로직 ---
        version_file = 'versions.json'
        if os.path.exists(version_file):
            with open(version_file, 'r') as f:
                all_versions = json.load(f)
        else:
            all_versions = {}
        all_versions[vin] = data['ecus']
        with open(version_file, 'w') as f:
            json.dump(all_versions, f, indent=4)

        write_audit_log(f"[VERSION REPORT SUCCESS] vin={vin}, versions={data['ecus']}")
        return jsonify({"status": "saved"}), 200

    except Exception as e:
        write_audit_log(f"[VERSION REPORT FAIL] {str(e)}")
        return jsonify({"error": str(e)}), 500

#if __name__ == "__main__":
    #app.run(host='0.0.0.0', port=5000, debug=True)