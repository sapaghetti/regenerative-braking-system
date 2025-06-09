# can OTA 구현

ECU : TMU, CGW, EDT

app.py로 서버 구동, Client는 download_file.py을 실행시켜 최신 업데이트 파일 감지

TMU가 파일 감지해서 각 CGW로 승인요청을 보내 사용자 승인을 받고 각 ECU에 업데이트 메세지 전송

## 🚗 1️⃣ app.py — OTA 서버 (Flask 기반 웹 서버)
### 전체 역할
* 클라이언트가 다운로드 받을 OTA 업데이트 파일(.bin) 을 업로드하고 관리

* 최신 버전 정보를 제공 (/latest_version)

* 다운로드 요청 시 Nonce 기반 인증 후 파일 전송 (/ota_download/<filename>)

* 로그인 및 권한 관리 (Admin만 업로드 가능)

* IP 접근 제한

* Audit Log 기록 (로그인, 다운로드, 업로드 기록)

### 주요 구성
* 로그인 화면 =>	 /login (CSRF 적용)
* 로그아웃 => /logout
* 파일 업로드 화면	=> /
* 파일 업로드 API => /upload
* 파일 다운로드 API (Admin 전용) => /upload/<filename>
* Nonce 발급 => /get_nonce
* 최신 버전 정보 제공 => /latest_version
* OTA 다운로드 (Nonce 인증) => /ota_download/<filename>

### 보안 기능
✅ Nonce 발급 후 인증 다운로드 (재사용 불가)

✅ CSRF 보호 적용

✅ IP 제한 (허용 IP만 접근 가능)

✅ 로그인 시 bcrypt 해시 사용

✅ Audit Log 기록

✅ Session 기반 권한 관리 (Admin role만 업로드 가능)


### OTA 흐름 (서버 관점)
1️⃣ Client가 /latest_version 호출 → 최신 버전 정보 획득

2️⃣ Client가 /get_nonce 호출 → Nonce 획득

3️⃣ Client가 /ota_download/<filename>?nonce=... 로 다운로드 요청

4️⃣ 서버는 Nonce 검증 후 파일 전송

---

## 📥 2️⃣ download_file.py — OTA 클라이언트 (자동 다운로드 스크립트)
### 전체 역할
* 주기적으로 OTA 서버 /latest_version API 확인

* 서버에 새로운 버전이 있으면 다운로드 수행

* Nonce 발급 후 다운로드 시도

* SHA256 무결성 검사 후 성공/실패 기록

* 진행률 파일 (progress.txt) 기록

* 마지막으로 성공한 파일의 SHA 기록 (downloaded_sha.txt)

### 주요 구성

* /latest_version 조회                     => 서버에서 최신 버전 정보 확인
* /get_nonce 요청                          => 다운로드용 Nonce 발급 받음
* /ota_download/<filename>?nonce=... 요청  => 다운로드 시도
* SHA256 체크                              => 다운로드 후 파일 무결성 확인
* 성공 시 SHA 기록                          => downloaded_sha.txt 에 기록
* 다운로드 진행률 기록                      => progress.txt 파일로 진행 상태 표시
* 다운로드 주기                             => 기본 10초마다 체크

### 보안/신뢰성 기능
✅ Nonce 기반 다운로드 (재사용 불가)

✅ SHA256 무결성 검증

✅ 다운로드 실패 시 재시도(MAX 3회)


### OTA 흐름 (클라이언트 관점)
1️⃣ /latest_version API 호출 → 최신 SHA256 확인

2️⃣ 기존에 다운로드한 SHA와 비교

→ 이미 최신이면 skip

→ 다르면 다운로드 시도
3️⃣ /get_nonce 호출 → Nonce 발급

4️⃣ 다운로드 시도

5️⃣ 다운로드 후 SHA256 검사

6️⃣ 성공 시 SHA 기록

7️⃣ 실패 시 재시도 (최대 3회)

