from flask import Flask, request, abort, redirect, render_template, url_for, flash, send_from_directory, jsonify, session
import os
import glob
import json
import bcrypt
import secrets  # Nonce 생성용
import hashlib
from flask_wtf import CSRFProtect
from datetime import timedelta
from login_form import LoginForm
from flask_session import Session
import logging
from datetime import datetime
from logging.handlers import RotatingFileHandler  # ✅ 추가

app = Flask(__name__)
app.secret_key = 'secret_key'
app.config['WTF_CSRF_SECRET_KEY'] = app.secret_key

# CSRF 보호 적용!
csrf = CSRFProtect(app)

# 세션 설정: 유효 시간을 30분으로 설정
app.permanent_session_lifetime = timedelta(minutes=10)

UPLOAD_FOLDER = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'upload')
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

USER_FILE = 'users.json'

# ✅ Audit log 설정 (rotation 적용!)
AUDIT_LOG_FILE = 'audit.log'
audit_logger = logging.getLogger('audit')
audit_logger.setLevel(logging.INFO)

# ✅ RotatingFileHandler 사용 → maxBytes = 5MB, backupCount = 5
audit_handler = RotatingFileHandler(AUDIT_LOG_FILE, maxBytes=5 * 1024 * 1024, backupCount=5)

audit_formatter = logging.Formatter('%(asctime)s %(message)s', datefmt='%Y-%m-%d %H:%M:%S')
audit_handler.setFormatter(audit_formatter)
audit_logger.addHandler(audit_handler)

def write_audit_log(message):
    audit_logger.info(message)

# ✅ 최신 firmware cache
latest_version_cache = {
    'version': None,
    'mtime': None,
    'sha256': None
}

# 사용자 정보 로드
def load_users():
    with open(USER_FILE, 'r') as f:
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

        # ✅ 최신 version cache 업데이트
        latest_version_cache['version'] = file.filename
        latest_version_cache['mtime'] = os.path.getmtime(file_path)
        latest_version_cache['sha256'] = calculate_sha256(file_path)

        return jsonify(message="Upload Complete!"), 200

    write_audit_log(f"[UPLOAD FAIL - Unknown reason] user={session.get('username')}, ip={request.remote_addr}")
    return jsonify(error="File upload failed"), 500

@app.route('/upload/<filename>')
def download_file(filename):
    if not session.get('logged_in'):
        return jsonify(error="User not logged in"), 401

    if session.get('role') != 'admin':
        write_audit_log(f"[DOWNLOAD DENIED] user={session.get('username')}, ip={request.remote_addr}")
        return jsonify(error="Admin privileges required"), 403

    client_nonce = request.args.get('nonce')

    if not client_nonce or client_nonce != session.get('ota_nonce'):
        write_audit_log(f"[DOWNLOAD FAIL - Invalid nonce] file={filename}, user={session.get('username')}, ip={request.remote_addr}")
        return jsonify(error="Invalid or missing nonce"), 403

    session.pop('ota_nonce', None)

    try:
        write_audit_log(f"[DOWNLOAD SUCCESS] file={filename}, user={session.get('username')}, ip={request.remote_addr}")
        return send_from_directory(app.config['UPLOAD_FOLDER'], filename, as_attachment=True)
    except FileNotFoundError:
        write_audit_log(f"[DOWNLOAD FAIL - FileNotFound] file={filename}, user={session.get('username')}, ip={request.remote_addr}")
        return redirect(url_for('upload_form'))

@app.route('/get_nonce', methods=['GET'])
def get_nonce():
    nonce = secrets.token_hex(16)
    session['ota_nonce'] = nonce
    return jsonify({'nonce': nonce})

ALLOWED_IPS = [
    '127.0.0.1',
    '192.168.0.100',
    '192.168.0.101'
]

@app.before_request
def limit_remote_addr():
    client_ip = request.remote_addr
    if client_ip not in ALLOWED_IPS:
        print(f"[IP BLOCKED] 접근 시도 IP: {client_ip}")
        abort(403)
    else:
        print(f"[IP ALLOWED] 접근 허용 IP: {client_ip}")

def calculate_sha256(filepath):
    sha256_hash = hashlib.sha256()
    with open(filepath, "rb") as f:
        for byte_block in iter(lambda: f.read(4096), b""):
            sha256_hash.update(byte_block)
    return sha256_hash.hexdigest()

@app.route('/latest_version')
def latest_version():
    if latest_version_cache['version'] is None:
        # ✅ 최초 호출 시 cache 초기화
        firmware_files = glob.glob(os.path.join(UPLOAD_FOLDER, 'comfortable_mode_*.bin'))
        if not firmware_files:
            return jsonify(version=None, mtime=None, sha256=None)

        latest_file = max(firmware_files, key=os.path.getmtime)
        latest_version_cache['version'] = os.path.basename(latest_file)
        latest_version_cache['mtime'] = os.path.getmtime(latest_file)
        latest_version_cache['sha256'] = calculate_sha256(latest_file)

    print(f"[API] Latest version (cached) -> {latest_version_cache['version']} (mtime: {latest_version_cache['mtime']}, sha256: {latest_version_cache['sha256']})")

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

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)
