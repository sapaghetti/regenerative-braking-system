import win32com.client
import time
import os

# --- 설정 값 ---
# CANoe 설정 파일 경로 (본인의 경로로 수정하세요!)
# 이 설정 파일(.cfg) 내에 Test Automation 관련 설정과 vTESTstudio Test Configuration에 대한 참조가 포함되어 있어야 합니다.
CANOE_CONFIG_PATH = r"D:\can\CANoe\RBS.cfg"

# 실행할 vTESTstudio Test Configuration의 이름 (정확한 이름을 입력하세요!)
# CANoe Test Automation 인터페이스에서 보이는 이름과 동일해야 합니다.
TEST_CONFIGURATION_NAME = "Test Configuration 1"

# 젠킨스 로그 및 스크립트 자체 로그 파일
LOG_FILE = "canoe_vteststudio_integrated_automation.log"

def log_message(message):
    """콘솔과 로그 파일에 메시지를 출력합니다."""
    print(message)
    with open(LOG_FILE, "a", encoding="utf-8") as f:
        f.write(message + "\n")

def run_integrated_automation():
    canoe = None
    try:
        log_message("--- Starting CANoe Automation with Integrated vTESTstudio Test Configuration ---")

        # 1. CANoe COM 객체 생성 및 콘피그 로드
        log_message("Creating CANoe COM object...")
        canoe = win32com.client.Dispatch("CANoe.Application")
        log_message(f"CANoe Version: {canoe.Version.Major}.{canoe.Version.Minor}.{canoe.Version.Build}")
        
        log_message(f"Loading CANoe configuration file: {CANOE_CONFIG_PATH}")
        canoe.Open(CANOE_CONFIG_PATH)
        log_message("CANoe configuration file loaded successfully.")

        # CANoe 설정 로드 후 충분한 대기 시간을 줍니다.
        log_message("Giving CANoe sufficient time to fully initialize after config load...")
        time.sleep(20) # 20초 대기 (환경 및 콘피그 복잡성에 따라 조절 필요)

        # 2. CANoe 측정 시작 (시뮬레이션 환경 활성화)
        log_message("Starting CANoe measurement...")
        canoe.Measurement.Start()
        
        # CANoe 측정이 시작될 때까지 충분히 대기 (Running 속성은 대부분의 버전에서 존재)
        start_time = time.time()
        timeout = 60 # 1분 타임아웃
        while not canoe.Measurement.Running and (time.time() - start_time < timeout):
            log_message("Waiting for CANoe measurement to start...")
            time.sleep(2)
        if not canoe.Measurement.Running:
            raise Exception("CANoe measurement did not start in time. Check CANoe configuration or if it requires user interaction.")
        log_message("CANoe measurement started successfully.")
        
        # 3. CANoe Test Automation 객체를 통해 vTESTstudio Test Configuration 실행
        # 이 부분이 중요합니다. CANoe의 TestAutomation 인터페이스를 사용합니다.
        log_message(f"Accessing CANoe Test Automation interface to run Test Configuration: '{TEST_CONFIGURATION_NAME}'")

        # CANoe.Application.TestEnvironments는 Test Units/Configurations를 관리하는 COM 객체를 반환합니다.
        # 정확한 접근 경로는 CANoe 버전에 따라 다를 수 있으므로, CANoe COM API 문서를 참조해야 합니다.
        # Test Automation 객체를 직접 가져오는 방식이 더 안정적일 수 있습니다.
        
        # CANoe.Application.TestAutomation 객체 접근
        test_automation = canoe.Application.TestAutomation
        
        if test_automation is None:
            raise Exception("CANoe Test Automation object is not available. Ensure 'Test Automation' feature is enabled in CANoe and COM API is accessible.")
        
        log_message("CANoe Test Automation object accessed.")
        
        # Test Automation 객체를 통해 Test Configuration 실행
        # Test Automation 객체의 RunTestConfiguration 메서드를 사용합니다.
        # 이 메서드는 일반적으로 Test Configuration의 이름을 인자로 받습니다.
        
        test_config_found = False
        try:
            # vTESTstudio Test Configuration을 CANoe Test Automation에서 로드하는 방식에 따라 다름
            # TestAutomation.TestConfigurations 컬렉션을 통해 접근할 수 있습니다.
            # 하지만 CANoe 19.0.155에서는 TestAutomation.RunTestConfiguration이 직접 이름을 받을 수도 있습니다.
            
            # 먼저 TestConfigurations 컬렉션을 반복하여 찾기 시도
            log_message("Searching for Test Configuration within CANoe Test Automation...")
            for tc_item in test_automation.TestConfigurations:
                if tc_item.Name == TEST_CONFIGURATION_NAME:
                    log_message(f"Found Test Configuration '{TEST_CONFIGURATION_NAME}'. Running it via CANoe Test Automation...")
                    tc_item.Run() # CANoe Test Automation 객체 내의 Test Configuration Item 실행
                    test_config_found = True
                    break
            
            if not test_config_found:
                 # 만약 컬렉션에서 찾지 못하면, RunTestConfiguration 메서드가 이름을 직접 받을 수 있는지 시도
                 log_message(f"Test Configuration '{TEST_CONFIGURATION_NAME}' not found in collection. Attempting direct execution via RunTestConfiguration method (if supported)...")
                 test_automation.RunTestConfiguration(TEST_CONFIGURATION_NAME)
                 test_config_found = True # 에러가 발생하지 않았다면 성공으로 간주
                 
        except Exception as e_run_tc:
            log_message(f"Could not run Test Configuration '{TEST_CONFIGURATION_NAME}' directly or via collection: {e_run_tc}")
            # 이 오류가 발생하면 test_config_found는 False로 유지
        
        if not test_config_found:
            raise Exception(f"Error: Test Configuration '{TEST_CONFIGURATION_NAME}' could not be started via CANoe Test Automation. Check name, CANoe config setup, or COM API documentation.")

        log_message("vTESTstudio Test Configuration started via CANoe. Waiting for completion...")

        # Test Automation 실행 완료 대기
        # canoe.Measurement.Running이 아닌, Test Automation의 실행 상태를 확인해야 합니다.
        # CANoe.Application.TestAutomation.IsRunning (또는 유사한 속성)을 사용합니다.
        
        start_time = time.time()
        test_timeout = 900 # 15분 타임아웃
        
        # CANoe Test Automation의 IsRunning 속성 확인
        while test_automation.IsRunning and (time.time() - start_time < test_timeout):
            log_message("Test Configuration is still running via CANoe Test Automation. Waiting...")
            time.sleep(10) # 10초마다 확인
        
        if test_automation.IsRunning:
            log_message(f"Error: Test Configuration did not complete within {test_timeout} seconds. Forcibly stopping Test Automation...")
            test_automation.Stop() # Test Automation 강제 중지
            raise Exception("CANoe Test Automation for vTESTstudio timed out.")
        else:
            log_message("vTESTstudio Test Configuration completed via CANoe Test Automation.")

    except Exception as e:
        log_message(f"An error occurred during automation: {e}")
        # 오류 발생 시 젠킨스 빌드를 실패 상태로 만들도록 예외 다시 발생
        raise
    finally:
        # 4. CANoe 측정 중지 및 CANoe 종료 (오류 발생 시에도 반드시 종료 시도)
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