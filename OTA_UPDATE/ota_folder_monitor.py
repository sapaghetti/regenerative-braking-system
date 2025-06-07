import time
import os
import subprocess
import requests
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

watch_folder = "upload"
# 감시할 확장자 (.bin)
target_extension = ".bin"

# download_file.py 경로
download_script = "download_file.py"

python_exe = "C:\Program Files\Python313\python.exe"

flag_folder = "OTA_flag" 
flag_filename = "ota_complete.flag"
flag_path = os.path.join(flag_folder, flag_filename)
os.makedirs(flag_folder, exist_ok=True)

class OTAHandler(FileSystemEventHandler):
    def on_created(self, event):
        if not event.is_directory:
            filename = os.path.basename(event.src_path)
            if filename.endswith(target_extension):
                print(f"[OTA 감지] 새 파일 업로드됨: {filename}")

                # try-except 사용 → 실패 시 flag 생성 안 하게 함
                try:
                    print("→ download_file.py 실행 중...")
                    subprocess.run([python_exe, download_script], check=True)
                    print("→ download_file.py 완료!")

                    # 성공 후 flag 파일 생성
                    flag_path = os.path.join(flag_folder, flag_filename)
                    with open(flag_path, "w") as f:
                        f.write(filename)
                    print(f"→ ota_complete.flag 생성 완료(버전 기록됨): {filename}")

                except subprocess.CalledProcessError as e:
                    print("!!! download_file.py 실행 실패!")
                    print(f"에러 내용: {e}")
                    # 실패 시 flag 생성 안 함

if __name__ == "__main__":
    print(f"OTA 폴더 감시 시작: {watch_folder}")

    # 🚩 flag 저장할 폴더가 없으면 생성 (자동으로)
    if not os.path.exists(flag_folder):
        os.makedirs(flag_folder)
        print(f"flag 폴더 생성됨: {flag_folder}")

    event_handler = OTAHandler()
    observer = Observer()
    observer.schedule(event_handler, path=watch_folder, recursive=False)
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()

    observer.join()
