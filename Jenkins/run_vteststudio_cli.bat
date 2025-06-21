@echo off
REM -----------------------------------------------------------
REM vTESTstudio CLI 자동화 배치 스크립트
REM -----------------------------------------------------------

REM vTESTstudio 설치 경로를 설정하세요. (실제 경로로 수정)
SET VTESTSTUDIO_EXE="C:\Program Files\Vector vTESTstudio 10\Exec64\vTESTstudio.exe"

REM vTESTstudio 프로젝트 파일 경로를 설정하세요. (실제 경로로 수정)
SET VTESTSTUDIO_PROJECT_PATH="C:\Program Files\Vector vTESTstudio 10\Exec64t\RBS.vtsoproj"

REM 생성될 테스트 리포트 저장 폴더 및 파일 이름을 설정하세요.
REM 젠킨스 워크스페이스 내에 저장하는 것이 리포트 게시 관리에 용이합니다.
SET REPORT_DIR="D:\can\vTESTstudio\TestReports"
SET REPORT_FILE_NAME="vTESTstudio_TestReport.html"
SET FULL_REPORT_PATH="%REPORT_DIR%\%REPORT_FILE_NAME%"

REM -----------------------------------------------------------
REM 로그 출력 함수 (한글 깨짐 방지 및 디버깅 강화)
REM -----------------------------------------------------------
chcp 65001 > nul REM UTF-8로 콘솔 인코딩 설정
:log_message
echo %*
echo %* >> vteststudio_cli_automation.log
goto :eof

REM -----------------------------------------------------------
REM 스크립트 시작
REM -----------------------------------------------------------
call :log_message "Starting vTESTstudio CLI automation..."
call :log_message "vTESTstudio Executable: %VTESTSTUDIO_EXE%"
call :log_message "vTESTstudio Project: %VTESTSTUDIO_PROJECT_PATH%"
call :log_message "Report Output: %FULL_REPORT_PATH%"

REM 리포트 저장 폴더 생성 (이미 존재하면 오류 없이 넘어감)
IF NOT EXIST "%REPORT_DIR%" (
    mkdir "%REPORT_DIR%"
    IF %ERRORLEVEL% NEQ 0 (
        call :log_message "ERROR: Failed to create report directory: %REPORT_DIR%"
        exit /b 1
    )
    call :log_message "Created report directory: %REPORT_DIR%"
) ELSE (
    call :log_message "Report directory already exists: %REPORT_DIR%"
)

REM 이전 리포트 파일 삭제 (새로운 리포트 생성을 위해)
IF EXIST "%FULL_REPORT_PATH%" (
    del "%FULL_REPORT_PATH%"
    call :log_message "Deleted previous report file: %FULL_REPORT_PATH%"
)

REM -----------------------------------------------------------
REM vTESTstudio CLI 명령 실행
REM -----------------------------------------------------------
call :log_message "Executing vTESTstudio CLI command..."
REM start /wait 명령어를 사용하여 vTESTstudio 프로세스가 완전히 종료될 때까지 기다리고
REM 그 종료 코드(ERRORLEVEL)를 정확하게 받아옵니다.
start /wait "" "%VTESTSTUDIO_EXE%" ^
    /open "%VTESTSTUDIO_PROJECT_PATH%" ^
    /run_all ^
    /report "%FULL_REPORT_PATH%" ^
    /quit

REM 명령어 실행 결과를 확인
IF %ERRORLEVEL% NEQ 0 (
    call :log_message "Error: vTESTstudio CLI command failed with errorlevel %ERRORLEVEL%."
    REM vTESTstudio 자체에서 오류가 발생했으므로, 빌드를 실패시킵니다.
    exit /b %ERRORLEVEL%
) ELSE (
    call :log_message "vTESTstudio CLI command executed successfully (errorlevel 0)."
)

REM 리포트 파일이 실제로 생성되었는지 확인
IF NOT EXIST "%FULL_REPORT_PATH%" (
    call :log_message "ERROR: Test report file was not generated: %FULL_REPORT_PATH%"
    exit /b 1
) ELSE (
    call :log_message "Test report file generated successfully: %FULL_REPORT_PATH%"
)

call :log_message "vTESTstudio CLI automation completed."

exit /b 0