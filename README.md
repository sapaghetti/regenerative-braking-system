# can
차량용 통신 시스템 구현 프로젝트


# **OTA 클라이언트 준비 및 실행 가이드**

이 가이드는 `download_file.py` 스크립트를 실행하여 OTA(Over-The-Air) 업데이트를 수신하고 처리하는 방법을 설명합니다. 서버는 Google Cloud VM에서 실행 중이며 정상 작동한다고 가정합니다.

**준비 컴퓨터:** `download_file.py`를 실행할 Windows 운영체제 컴퓨터

## **1. OTA 클라이언트 파일 준비 (클라이언트 컴퓨터)**

`download_file.py` 스크립트를 실행하고 OTA 기능을 사용하기 위해 필요한 파일들을 클라이언트 컴퓨터로 가져와야 합니다.

1. **클라이언트 프로젝트 디렉토리 생성**:
클라이언트 컴퓨터의 원하는 위치(예: `C:\OTA_Client`)에 새 폴더를 만듭니다.
2. **필수 파일 복사/클론**:
다음 파일들을 생성한 폴더(예: `C:\OTA_Client`)에 복사합니다.
    - **`download_file.py`**: OTA 업데이트를 진행하는 실제 클라이언트 스크립트.
    - **`[차량ID]_private.pem`**: (예: `Sapaghetti_private.pem`) - 각 차량에 고유한 개인 키 파일입니다. **이 파일은 서버 개발자로부터 받아야 합니다.**
    - **`public.pem`**: 서버의 공개 키 파일입니다. **이 파일은 서버 개발자로부터 받아야 합니다.**
    - **`isrgrootx1.pem`**: Let's Encrypt의 루트 CA(인증 기관) 인증서입니다.
        - **다운로드 방법**: 웹 브라우저를 열고 다음 링크에서 `isrgrootx1.pem` 파일을 직접 다운로드합니다: https://letsencrypt.org/certs/isrgrootx1.pem
        - 다운로드한 파일을 위에서 생성한 폴더(예: `C:\OTA_Client`)로 옮깁니다.
    
    **Git 사용 시 (권장):**
    만약 서버 개발자가 클라이언트 코드를 Git 저장소로 관리한다면, 해당 저장소를 클라이언트 컴퓨터로 `clone`합니다.
    
    - 명령 프롬프트/PowerShell에서 `C:\` 또는 원하는 상위 디렉토리로 이동 후:
        
        ```
        git clone [클라이언트_Git_저장소_URL] C:\OTA_Client
        
        ```
        
    - `[차량ID]_private.pem` 및 `public.pem` 파일은 보안상 Git에 포함되지 않을 수 있으므로, 별도로 받아 해당 디렉토리에 넣어주셔야 합니다.

## **2. `download_file.py` 클라이언트 설정 (클라이언트 컴퓨터)**

`download_file.py` 파일을 열어 몇 가지 설정을 확인하고 필요시 수정합니다.

1. **`download_file.py` 열기**:
Windows용 텍스트 에디터(메모장, VS Code 등)로 `C:\OTA_Client\download_file.py` 파일을 엽니다.
2. **MQTT 브로커 주소 설정 (`MQTT_BROKER_HOST`)**:
    - 파일 내에서 `MQTT_BROKER_HOST` 변수를 찾습니다.
        
        ```
        MQTT_BROKER_HOST = "www.sapaghetti.shop" # 이전에 설정된 도메인 이름
        MQTT_BROKER_PORT = 8883
        
        ```
        
    - **이 값이 서버 VM의 도메인 이름(`www.sapaghetti.shop`)으로 설정되어 있는지 확인합니다.** (IP 주소가 아닌 도메인 이름이어야 합니다.)
3. **CA 인증서 경로 설정 (`ca_certs`)**:
    - 파일 내에서 `mqtt_client_instance.tls_set` 부분을 찾습니다.
        
        ```
        # ...
        mqtt_client_instance.tls_set(
            ca_certs="C:\\vscodestudy\\OTA\\isrgrootx1.pem", # 이 경로를 수정합니다.
            # ...
        )
        # ...
        
        ```
        
    - `ca_certs` 경로가 **1단계에서 `isrgrootx1.pem` 파일을 복사해 둔 실제 경로**와 일치하는지 확인합니다.
        - **권장 방식 (파일이 `download_file.py`와 같은 디렉토리에 있을 경우):**
            
            ```
            # ...
            import os # <-- 파일 상단에 추가되어 있는지 확인
            # ...
            
            # 메인 함수 시작 부분 또는 전역 변수 설정 부분에서 경로를 정의
            current_script_dir = os.path.dirname(os.path.abspath(__file__))
            isrg_root_x1_path = os.path.join(current_script_dir, "isrgrootx1.pem")
            
            # ...
            mqtt_client_instance.tls_set(
                ca_certs=isrg_root_x1_path,
                # ...
            )
            # ...
            
            ```
            
        - **다른 절대 경로를 사용할 경우:** `ca_certs="C:\\Your\\Custom\\Path\\isrgrootx1.pem"` 와 같이 실제 경로로 설정합니다.
4. **CANoe 프로젝트 경로 설정 (`OTA_BASE_PROJECT_PATH`)**:
    - 파일 내에서 `OTA_BASE_PROJECT_PATH` 변수를 찾습니다.
        
        ```
        OTA_BASE_PROJECT_PATH = "C:\\Users\\Public\\Documents\\Vector\\CANoe\\canoe19_project\\ota_project"
        
        ```
        
    - 이 경로가 클라이언트 컴퓨터의 CANoe 프로젝트 경로와 **동일한지 확인**합니다. 만약 다르다면, 해당 경로를 클라이언트 컴퓨터의 실제 CANoe 프로젝트 경로에 맞게 수정해야 합니다. (`\` 대신 `\\` 또는 `/` 사용)
5. **파일 저장**: 수정한 내용을 저장합니다.

## **3. Python 라이브러리 설치 (클라이언트 컴퓨터)**

`download_file.py` 스크립트가 실행되기 위해 필요한 Python 라이브러리들을 설치합니다. 이 작업은 **Windows 명령 프롬프트 또는 PowerShell**에서 진행해야 합니다.

1. **명령 프롬프트/PowerShell 열기**:
Windows 검색창에 `cmd` 또는 `powershell`을 입력하여 실행합니다.
2. **스크립트 디렉토리로 이동**:
    
    ```
    cd C:\OTA_Client # 1단계에서 파일을 복사해 둔 폴더 경로
    
    ```
    
3. **Python 가상 환경 생성 (권장)**:
    - 시스템 Python 환경을 깨끗하게 유지하기 위해 가상 환경을 생성합니다.
        
        ```
        python -m venv venv_ota_client
        
        ```
        
4. **가상 환경 활성화**:
    
    ```
    .\venv_ota_client\Scripts\activate
    
    ```
    
    - 프롬프트가 `(venv_ota_client) C:\OTA_Client>`와 같이 `(venv_ota_client)`가 앞에 붙으면 성공입니다.
5. **필요한 라이브러리 설치**:
가상 환경이 활성화된 상태에서 다음 명령어를 실행합니다. **`download_file.py`에서 필요로 하는 모든 라이브러리입니다. 누락되지 않도록 모두 설치합니다.**
    
    ```
    pip install requests py-canoe paho-mqtt pywin32 pycryptodome
    
    ```
    
    - 설치 중 오류가 발생하지 않는지 확인합니다. 특히 `pywin32`는 Windows 환경에서만 설치 가능합니다.

## **4. OTA 클라이언트 실행 및 테스트 (클라이언트 컴퓨터)**

이제 모든 준비가 완료되었습니다.

1. **`download_file.py` 실행**:
가상 환경이 활성화된 명령 프롬프트/PowerShell에서 다음 명령어를 실행합니다. **`<차량ID>`를 실제 차량 ID (예: `Sapaghetti`)로 입력합니다.**
    
    ```
    python download_file.py Sapaghetti
    
    ```
    
    - 스크립트가 시작되면 `[MQTT] MQTT 브로커(www.sapaghetti.shop:8883) 연결 시도 중...`과 같은 메시지가 나타날 것입니다.
    - 성공적으로 연결되면 `[MQTT] 브로커에 성공적으로 연결됨. (결과 코드: 0)` 메시지가 나타나고, `-- 펌웨어 다운로드는 HTTP/HTTPS를 통해 이루어집니다 ---` 메시지 이후에는 새 펌웨어 알림을 기다리는 상태가 됩니다.
2. **OTA 업데이트 진행 (서버 개발자 요청)**:
    - 클라이언트 스크립트가 실행 중인 상태로 둡니다.
    - **서버 개발자에게 웹 인터페이스(`https://www.sapaghetti.shop/`)를 통해 새로운 펌웨어 파일을 업로드해 달라고 요청합니다.**
    - 펌웨어 업로드가 완료되면, 클라이언트 스크립트가 실행 중인 명령 프롬프트/PowerShell 창에 MQTT 알림 수신, 펌웨어 다운로드, 보안 검증, CANoe 시뮬레이션(또는 실제 CANoe 연동) 등의 과정이 순차적으로 표시될 것입니다.

## **5. 문제 해결 시**

- **`ModuleNotFoundError` 발생**: `3. Python 라이브러리 설치` 단계를 다시 꼼꼼하게 확인합니다. 특히 가상 환경 활성화를 잊지 않았는지, `pip install` 명령이 오류 없이 완료되었는지 확인합니다.
- **`[MQTT ERROR] MQTT 브로커 연결에 실패했습니다: ...` 오류 발생**:
    - **`IP address mismatch`**: `2. 클라이언트 설정` 단계에서 `MQTT_BROKER_HOST`가 `www.sapaghetti.shop`으로 정확히 설정되었는지 확인합니다.
    - **`certificate verify failed`**: `2. 클라이언트 설정` 단계에서 `ca_certs` 경로가 `isrgrootx1.pem` 파일의 정확한 경로를 가리키는지, 그리고 `isrgrootx1.pem` 파일이 실제로 해당 경로에 존재하는지 다시 확인합니다.
    - **`Connection refused` 또는 연결 지연**: 클라이언트 컴퓨터가 서버 VM에 네트워크로 접근할 수 없는 경우입니다. 클라이언트 컴퓨터의 인터넷 연결, VM의 공인 IP 주소, 그리고 Google Cloud 방화벽 규칙(TCP 8883 및 443 포트 허용)을 다시 확인해야 합니다.
    - **Telnet 테스트**: 명령 프롬프트/PowerShell에서 `telnet www.sapaghetti.shop 8883`을 입력하여 `Connected to ...` 메시지가 나오는지 확인합니다. 나오지 않는다면 네트워크 연결에 문제가 있는 것입니다.
- **CANoe 관련 오류**: `CANoe 연결 실패` 또는 `get_system_variable_value` 오류 등은 `1. 필수 프로그램 확인`의 CANoe 설치 및 프로젝트 경로 설정을 다시 확인해야 합니다. CANoe가 실행 중인지도 확인합니다.

