# can OTA 구현

ECU : TMU, CGW, EDT

app.py로 서버 구동, Client는 download_file.py을 실행시켜 최신 업데이트 파일 감지

TMU가 파일 감지해서 각 CGW로 승인요청을 보내 사용자 승인을 받고 각 ECU에 업데이트 메세지 전송
