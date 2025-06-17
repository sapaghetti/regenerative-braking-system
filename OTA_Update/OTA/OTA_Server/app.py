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
VEHICLE_FILE = 'vehicles.json'

AUDIT_LOG_FILE = 'audit.log'
audit_logger = logging.getLogger('audit')
audit_logger.setLevel(logging.INFO)
audit_handler = RotatingFileHandler(AUDIT_LOG_FILE, maxBytes=5 * 1024 * 1024, backupCount=5)
audit_formatter = logging.Formatter('%(asctime)s %(message)s', datefmt='%Y-%m-%d %H:%M:%S')
audit_handler.setFormatter(audit_formatter)
audit_logger.addHandler(audit_handler)

def write_audit_log(message):
    audit_logger.info(message)

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
    if form.validate_on_submit(): # POST 요청 처리
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
        
        # 로그인 실패 시 (사용자 이름 없거나 비밀번호 틀림)
        write_audit_log(f"[LOGIN FAIL] user={username}, ip={request.remote_addr}")
        # 'error=1'과 함께 이전에 입력된 사용자 이름을 'prev_username' 쿼리 파라미터로 넘겨 리다이렉트
        return redirect(url_for('login', error='1', prev_username=username)) 
    
    # GET 요청 처리 (초기 페이지 로드 또는 리다이렉트 후)
    # URL 쿼리 파라미터에서 'prev_username' 값을 가져옴
    prev_username = request.args.get('prev_username')
    if prev_username:
        form.username.data = prev_username # 폼 데이터에 이전 사용자 이름을 설정하여 필드에 채워지도록 함

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
    write_audit_log(f"[UPLOAD DEBUG] Request received from IP: {request.remote_addr}, User: {session.get('username')}")

    if not session.get('logged_in'):
        write_audit_log(f"[UPLOAD DENIED] User not logged in, IP: {request.remote_addr}")
        return jsonify(error="User not logged in"), 401
    
    if session.get('role') != 'admin':
        write_audit_log(f"[UPLOAD DENIED] Admin privileges required, User: {session.get('username')}, IP: {request.remote_addr}")
        return jsonify(error="Admin privileges required"), 403
    
    if 'file' not in request.files:
        write_audit_log(f"[UPLOAD FAIL] No file part in request, User: {session.get('username')}, IP: {request.remote_addr}")
        return jsonify(error="No file part"), 400
    
    file = request.files['file']
    
    if file.filename == '':
        write_audit_log(f"[UPLOAD FAIL] No selected filename, User: {session.get('username')}, IP: {request.remote_addr}")
        return jsonify(error="No selected file"), 400
    
    if file:
        file_path = os.path.join(app.config['UPLOAD_FOLDER'], file.filename)
        write_audit_log(f"[UPLOAD DEBUG] Attempting to save file to: {file_path}")
        try:
            file.save(file_path)
            write_audit_log(f"[UPLOAD SUCCESS] File '{file.filename}' saved successfully, User: {session.get('username')}, IP: {request.remote_addr}")
            
            return jsonify(message="Upload Complete!"), 200
        except Exception as e:
            write_audit_log(f"[UPLOAD CRITICAL FAIL] Failed to save file '{file.filename}': {str(e)}, User: {session.get('username')}, IP: {request.remote_addr}")
            return jsonify(error=f"File save failed: {str(e)}"), 500
    
    write_audit_log(f"[UPLOAD FAIL] Unknown reason, User: {session.get('username')}, IP: {request.remote_addr}")
    return jsonify(error="File upload failed"), 500

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
    firmware_files = glob.glob(os.path.join(app.config['UPLOAD_FOLDER'], '*.bin'))

    if not firmware_files:
        write_audit_log(f"[LATEST VERSION] No firmware files found in upload folder, IP: {request.remote_addr}")
        return jsonify(version=None, mtime=None, sha256=None)

    latest_file_path = max(firmware_files, key=os.path.getmtime)
    
    latest_version_filename = os.path.basename(latest_file_path)
    latest_mtime = os.path.getmtime(latest_file_path)
    latest_sha256 = calculate_sha256(latest_file_path)

    write_audit_log(f"[LATEST VERSION] Found latest: {latest_version_filename}, SHA: {latest_sha256[:10]}..., IP: {request.remote_addr}")
    return jsonify(
        version=latest_version_filename,
        mtime=latest_mtime,
        sha256=latest_sha256
    )

@app.route('/ota_download/<filename>')
def ota_download_file(filename):
    client_nonce = request.args.get('nonce')
    if not client_nonce or client_nonce != session.get('ota_nonce'):
        write_audit_log(f"[OTA DOWNLOAD FAIL - Invalid nonce] file={filename}, ip={request.remote_addr}")
        return jsonify(error="Invalid or missing nonce"), 403
    session.pop('ota_nonce', None)
    try:
        if not os.path.exists(os.path.join(app.config['UPLOAD_FOLDER'], filename)):
            write_audit_log(f"[OTA DOWNLOAD FAIL - File not found] file={filename}, ip={request.remote_addr}")
            return jsonify(error="File not found on server"), 404

        write_audit_log(f"[OTA DOWNLOAD SUCCESS] file={filename}, ip={request.remote_addr}")
        return send_from_directory(app.config['UPLOAD_FOLDER'], filename, as_attachment=True)
    except Exception as e:
        write_audit_log(f"[OTA DOWNLOAD CRITICAL FAIL] Unexpected error downloading file {filename}: {str(e)}, ip={request.remote_addr}")
        return jsonify(error=f"Server error during download: {str(e)}"), 500

@csrf.exempt
@app.route('/report_versions', methods=['POST'])
def report_versions():
    try:
        token = request.headers.get('X-Vehicle-Token')
        if not token:
            write_audit_log(f"[VERSION REPORT FAIL - No Token] ip={request.remote_addr}")
            return jsonify({"error": "Authentication token is missing"}), 401

        data = request.get_json()
        if not data or 'vehicle_id' not in data or 'ecus' not in data:
            write_audit_log(f"[VERSION REPORT FAIL - Invalid Data] ip={request.remote_addr}, data={data}")
            return jsonify({"error": "Invalid data"}), 400

        vin = data['vehicle_id']
        all_vehicles = load_vehicles()

        if vin not in all_vehicles or all_vehicles[vin].get('token') != token:
            write_audit_log(f"[VERSION REPORT FAIL - Invalid Token] vin={vin}, ip={request.remote_addr}")
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

        write_audit_log(f"[VERSION REPORT SUCCESS] vin={vin}, versions={data['ecus']}, ip={request.remote_addr}")
        return jsonify({"status": "saved"}), 200

    except Exception as e:
        write_audit_log(f"[VERSION REPORT FAIL - Exception] {str(e)}, ip={request.remote_addr}")
        return jsonify({"error": str(e)}), 500

#if __name__ == "__main__":
    #app.run(host='0.0.0.0', port=5000, debug=True)