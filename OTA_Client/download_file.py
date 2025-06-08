# download_file.py

import requests
import time
import os
import hashlib

# 서버 설정
server_base_url = 'http://127.0.0.1:5000'
latest_version_api = f'{server_base_url}/latest_version'
get_nonce_api = f'{server_base_url}/get_nonce'
download_base_url = f'{server_base_url}/ota_download'

# 다운로드 저장 폴더
DOWNLOAD_FOLDER = "OTA_download"

# SHA 기록 파일
downloaded_sha_file = 'downloaded_sha.txt'

# 진행률 저장용 파일
progress_file = 'progress.txt'

# 다운로드 폴더가 없으면 생성
os.makedirs(DOWNLOAD_FOLDER, exist_ok=True)

def get_latest_version(session):
    try:
        response = session.get(latest_version_api, timeout=5)
        response.raise_for_status()
        data = response.json()
        latest_version = data.get('version')
        latest_mtime = data.get('mtime')
        latest_sha256 = data.get('sha256')
        print(f"[INFO] Latest version from server: {latest_version} (mtime: {latest_mtime}, sha256: {latest_sha256})")
        return latest_version, latest_mtime, latest_sha256
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Failed to get latest version: {e}")
        return None, None, None

def get_nonce(session):
    try:
        response = session.get(get_nonce_api, timeout=5)
        response.raise_for_status()
        data = response.json()
        nonce = data.get('nonce')
        print(f"[INFO] Got nonce: {nonce}")
        return nonce
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Failed to get nonce: {e}")
        return None

def download_file(session, url, local_filename):
    try:
        with session.get(url, stream=True, timeout=10) as r:
            r.raise_for_status()
            total_size = int(r.headers.get('content-length', 0))
            print(f"[INFO] Total size: {total_size} bytes")

            downloaded_size = 0

            with open(local_filename, 'wb') as f:
                for chunk in r.iter_content(chunk_size=8192):
                    if chunk:
                        f.write(chunk)
                        downloaded_size += len(chunk)

                        progress = int((downloaded_size * 100) / total_size) if total_size else 0
                        if progress > 100:
                            progress = 100

                        print(f"Downloading... {progress}% ({downloaded_size}/{total_size} bytes)")

                        with open(progress_file, 'w') as pf:
                            pf.write(str(progress))

                        time.sleep(0.05)

        print("[INFO] Download complete!")
        with open(progress_file, 'w') as pf:
            pf.write('100')

    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Download failed: {e}")
        with open(progress_file, 'w') as pf:
            pf.write('ERROR')

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

if __name__ == "__main__":
    # 시작 전에 progress.txt 초기화
    if os.path.exists(progress_file):
        os.remove(progress_file)

    # 세션 객체 만들기
    session = requests.Session()

    # 기록된 SHA 읽기
    downloaded_sha256 = load_downloaded_sha()

    # 주기 (초 단위), 예: 10초마다 체크
    CHECK_INTERVAL = 10

    # 재시도 횟수 설정
    MAX_RETRY = 3

    while True:
        print("[INFO] Checking for OTA update...")

        latest_version, latest_mtime, latest_sha256 = get_latest_version(session)

        if latest_version:
            # SHA256 비교
            if latest_sha256 == downloaded_sha256:
                print(f"[INFO] Already up-to-date with SHA256: {latest_sha256}")
            else:
                retry_count = 0
                success = False

                while retry_count < MAX_RETRY and not success:
                    print(f"[INFO] Attempting download... (try {retry_count + 1}/{MAX_RETRY})")

                    nonce = get_nonce(session)

                    if nonce:
                        download_url = f"{download_base_url}/{latest_version}?nonce={nonce}"
                        local_filename = os.path.join(DOWNLOAD_FOLDER, latest_version)

                        print(f"[INFO] Download URL: {download_url}")
                        print(f"[INFO] Local filename: {local_filename}")

                        download_file(session, download_url, local_filename)

                        # ✅ 무결성 체크 (Integrity Check)
                        downloaded_file_sha256 = calculate_file_sha256(local_filename)

                        if downloaded_file_sha256 == latest_sha256:
                            print(f"[INTEGRITY CHECK] SUCCESS! SHA256 matches: {downloaded_file_sha256}")
                            save_downloaded_sha(latest_sha256)
                            downloaded_sha256 = latest_sha256
                            success = True
                        else:
                            print(f"[INTEGRITY CHECK] FAILED! Downloaded SHA256: {downloaded_file_sha256}, Expected SHA256: {latest_sha256}")
                            print("[WARNING] Integrity check failed — Retrying...")
                            retry_count += 1
                            time.sleep(2)  # 재시도 전 약간 대기
                    else:
                        print("[ERROR] Could not get nonce. Skipping this cycle.")
                        break

                if not success:
                    print("[ERROR] Failed to download valid file after maximum retries. Skipping this cycle.")
        else:
            print("[WARN] No latest version available.")

        print(f"[INFO] Waiting {CHECK_INTERVAL} seconds for next check...\n")
        time.sleep(CHECK_INTERVAL)
