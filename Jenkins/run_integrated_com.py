import win32com.client
import time
import os

# --- 설정 값 ---
# CANoe 설정 파일 경로 (본인의 경로로 수정하세요!)
CANOE_CONFIG_PATH = r"D:\can\CANoe\RBS.cfg"

# vTESTstudio 프로젝트 파일 경로 (본인의 경로로 수정하세요!)
VTESTSTUDIO_PROJECT_PATH = r"D:\can\vTESTstudio\RBS.vtsoproj"

# 실행할 vTESTstudio Test Configuration의 이름 (정확한 이름을 입력하세요!)
# 예: "MyDrivingTestConfig" 또는 "DiagnosticTestSequence"
TEST_CONFIGURATION_NAME = "RBSTestConfiguration" # 실제 Test Configuration 이름으로 수

# 젠킨스 로그 및 스크립트 자체 로그 파일
LOG_FILE = "integrated_automation.log"

def log_message(message):
    """콘솔과 로그 파일에 메시지를 출력합니다."""
    print(message)
    with open(LOG_FILE, "a", encoding="utf-8") as f:
        f.write(message + "\n")

def run_integrated_automation():
    canoe = None
    vteststudio = None
    try:
        log_message("--- Starting Integrated CANoe & vTESTstudio Test Automation ---")

        # 1. CANoe COM 객체 생성 및 콘피그 로드
        log_message("Creating CANoe COM object...")
        canoe = win32com.client.Dispatch("CANoe.Application")
        log_message(f"CANoe Version: {canoe.Version.Major}.{canoe.Version.Minor}.{canoe.Version.Build}")
        
        log_message(f"Loading CANoe configuration file: {CANOE_CONFIG_PATH}")
        canoe.Open(CANOE_CONFIG_PATH)
        log_message("CANoe configuration file loaded successfully.")

        # CANoe가 완전히 로드되고 준비될 때까지 대기
        start_time = time.time()
        timeout = 120 # 2분 타임아웃
        while not canoe.Measurement.Ready and (time.time() - start_time < timeout):
            log_message("Waiting for CANoe to become ready...")
            time.sleep(5)
        if not canoe.Measurement.Ready:
            raise Exception("CANoe did not become ready in time. Check if config loaded correctly or if it requires user interaction.")
        log_message("CANoe appears to be ready for measurement.")

        # 2. CANoe 측정 시작 (시뮬레이션 환경 활성화)
        log_message("Starting CANoe measurement...")
        canoe.Measurement.Start()
        
        # CANoe 측정이 시작될 때까지 충분히 대기
        start_time = time.time()
        timeout = 60 # 1분 타임아웃
        while not canoe.Measurement.Running and (time.time() - start_time < timeout):
            log_message("Waiting for CANoe measurement to start...")
            time.sleep(2)
        if not canoe.Measurement.Running:
            raise Exception("CANoe measurement did not start in time. Check CANoe configuration or if it requires user interaction.")
        log_message("CANoe measurement started successfully.")
        
        # vTESTstudio가 CANoe에 연결할 수 있도록 잠시 대기
        log_message("Giving CANoe some time for stabilization before vTESTstudio connection...")
        time.sleep(10) 

        # 3. vTESTstudio COM 객체 생성 및 프로젝트 로드
        log_message("Creating vTESTstudio COM object...")
        vteststudio = win32com.client.Dispatch("vTESTstudio.Application")
        log_message(f"vTESTstudio Version: {vteststudio.Version.Major}.{vteststudio.Version.Minor}.{vteststudio.Version.Build}")

        log_message(f"Opening vTESTstudio project: {VTESTSTUDIO_PROJECT_PATH}")
        vteststudio.Open(VTESTSTUDIO_PROJECT_PATH)
        log_message("vTESTstudio project opened successfully.")
        
        # vTESTstudio 프로젝트 로드 후 CANoe에 연결될 때까지 대기
        log_message("Waiting for vTESTstudio to connect to CANoe...")
        time.sleep(10) # 연결 시간 대기 (환경에 따라 조절)

        # 4. vTESTstudio Test Configuration 실행
        log_message(f"Attempting to run Test Configuration: '{TEST_CONFIGURATION_NAME}'")

        test_config_found = False
        # TestEnvironments 컬렉션을 탐색하여 Test Configuration을 찾고 실행
        for te in vteststudio.TestEnvironments:
            # log_message(f"  Checking Test Environment: {te.Name}") # 디버깅용
            for tc in te.TestConfigurations:
                # log_message(f"    Found Test Configuration: {tc.Name}") # 디버깅용
                if tc.Name == TEST_CONFIGURATION_NAME:
                    log_message(f"  Found target Test Configuration: '{TEST_CONFIGURATION_NAME}'. Running it...")
                    tc.Run() # Test Configuration 실행 메서드
                    test_config_found = True
                    break # Test Configuration을 찾았으므로 내부 루프 종료
            if test_config_found:
                break # Test Configuration을 찾았으므로 외부 루프 종료

        if not test_config_found:
            raise Exception(f"Error: Test Configuration '{TEST_CONFIGURATION_NAME}' not found in the project. Check name or path.")

        log_message("vTESTstudio Test Configuration started. Waiting for completion...")

        # 테스트가 완료될 때까지 대기 (vTESTstudio Test Execution Engine 상태 확인)
        start_time = time.time()
        test_timeout = 900 # 15분 타임아웃 (충분히 길게 설정)
        while vteststudio.TestExecution.IsRunning and (time.time() - start_time < test_timeout):
            log_message("vTESTstudio Test Configuration is still running. Waiting...")
            time.sleep(10) # 10초마다 확인
        
        if vteststudio.TestExecution.IsRunning:
            log_message(f"Error: vTESTstudio Test Configuration did not complete within {test_timeout} seconds. Forcibly stopping...")
            vteststudio.TestExecution.Stop() # 강제 중지
            raise Exception("vTESTstudio Test Configuration timed out.")
        else:
            log_message("vTESTstudio Test Configuration completed.")

    except Exception as e:
        log_message(f"An error occurred during automation: {e}")
        # 오류 발생 시 젠킨스 빌드를 실패 상태로 만들도록 예외 다시 발생
        raise
    finally:
        # 5. vTESTstudio 종료 (오류 발생 시에도 반드시 종료 시도)
        if vteststudio:
            try:
                log_message("Closing vTESTstudio project...")
                vteststudio.Close()
                log_message("Terminating vTESTstudio application...")
                vteststudio.Quit()
                log_message("vTESTstudio terminated.")
            except Exception as ex:
                log_message(f"Failed to gracefully close/quit vTESTstudio: {ex}")
        
        # 6. CANoe 측정 중지 및 CANoe 종료 (오류 발생 시에도 반드시 종료 시도)
        if canoe:
            try:
                if canoe.Measurement.Running:
                    log_message("Stopping CANoe measurement...")
                    canoe.Measurement.Stop()
                    # 측정이 완전히 멈출 때까지 대기
                    stop_timeout = 30
                    stop_start_time = time.time()
                    while canoe.Measurement.Running and (time.time() - stop_start_time < stop_timeout):
                        log_message("Waiting for CANoe measurement to stop...")
                        time.sleep(1)
                    if canoe.Measurement.Running:
                        log_message("Warning: CANoe measurement did not stop gracefully.")

                log_message("Closing CANoe configuration...")
                canoe.Close()
                log_message("Terminating CANoe application...")
                canoe.Quit()
                log_message("CANoe terminated.")
            except Exception as ex:
                log_message(f"Failed to gracefully stop/close/quit CANoe: {ex}")
        
        log_message("--- Integrated CANoe & vTESTstudio Test Automation Finished ---")

if __name__ == "__main__":
    if os.path.exists(LOG_FILE):
        os.remove(LOG_FILE) # 이전 로그 파일 삭제
    try:
        run_integrated_automation()
        exit(0) # 성공 시 0 반환
    except Exception as e:
        log_message(f"Script failed with unhandled exception: {e}")
        exit(1) # 실패 시 1 반환