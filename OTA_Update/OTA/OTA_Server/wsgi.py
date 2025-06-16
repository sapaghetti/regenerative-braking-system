# /var/www/my_ota_app/can/OTA_Update/OTA/OTA_Server/wsgi.py

import sys
import os

# Flask 앱의 실제 경로 (OTA_Server 디렉토리)를 sys.path에 추가
sys.path.insert(0, '/var/www/my_ota_app/can/OTA_Update/OTA/OTA_Server')

from app import app as application

# 이 부분은 Gunicorn이 호출하지 않으므로 없어도 되지만, 로컬 테스트용으로 둘 수 있습니다.
if __name__ == "__main__":
    application.run(debug=True)
