import win32com.client
import time
import os

# --- 설정 값 ---
# CANoe 콘피그 파일 경로 (본인의 경로로 수정하세요!)
CANOE_CONFIG_PATH = r"D:\can\CANoe\RBS.cfg" # 예시 경로, 실제 경로로 수정
# 테스트 측정 시간 (초)
MEASUREMENT_DURATION_SECONDS = 20 # 3분 (180초)로 설정, 필요에 따라 조정 가능
# 젠킨스 로그에 표시할 메시지
LOG_FILE = "canoe_automation.log"

# 열고 싶은 .vtestreport 파일 경로
REPORT_FILE_PATH = r"D:\can\CANoe\Report_Test_Configuration_1.vtestreport"

def log_message(message):
    """콘솔과 로그 파일에 메시지를 출력합니다."""
    print(message)
    with open(LOG_FILE, "a") as f:
        f.write(message + "\n")

def wait_for_canoe_ready(canoe_app, timeout=120):
    """
    CANoe 애플리케이션이 완전히 로드되고 준비될 때까지 기다립니다.
    """
    start_time = time.time()
    log_message("Waiting for CANoe to load and become ready...")
    while time.time() - start_time < timeout:
        try:
            # CANoe의 Measurement.Running 상태가 False (측정 중이 아님)인 것을 확인.
            # System.State는 COM API 노출 여부가 버전마다 다를 수 있으므로 Measurement.Running이 더 일반적.
            if not canoe_app.Measurement.Running:
                log_message("CANoe appears to be ready.")
                return True
        except Exception as e:
            # 아직 CANoe 객체가 완전히 초기화되지 않았거나 오류 발생 시
            log_message(f"Error occurred while checking CANoe status: {e}. Retrying...")
        time.sleep(2) # 2초마다 상태 확인
    log_message("Error: CANoe did not become ready within the specified time.")
    return False

def wait_for_measurement_start(canoe_app, timeout=60):
    """CANoe 측정이 시작될 때까지 기다립니다."""
    start_time = time.time()
    log_message("Waiting for CANoe measurement to start...")
    while time.time() - start_time < timeout:
        try:
            if canoe_app.Measurement.Running:
                log_message("CANoe measurement started successfully.")
                return True
        except Exception as e:
            log_message(f"Error occurred while checking measurement start: {e}. Retrying...")
        time.sleep(1) # 1초마다 상태 확인
    log_message("Error: CANoe measurement did not start within the specified time.")
    return False

def open_report_file(file_path):
    """지정된 파일을 기본 연결 프로그램으로 엽니다."""
    try:
        log_message(f"Attempting to open report file: {file_path}")
        # os.startfile은 Windows 전용이며, 파일의 기본 연결 프로그램을 사용합니다.
        os.startfile(file_path) 
        log_message("Report file opened successfully (assuming default association).")
        return True
    except AttributeError:
        # os.startfile이 없는 경우 (예: Linux)
        log_message("os.startfile is not available (non-Windows OS?). Trying subprocess.Popen...")
        try:
            # Linux/macOS 등에서는 'xdg-open' 또는 'open' 명령어를 사용
            if os.name == 'posix': # Unix-like system
                subprocess.Popen(['xdg-open', file_path]) # Linux
            elif os.name == 'mac': # macOS
                subprocess.Popen(['open', file_path]) # macOS
            else:
                raise Exception("Unsupported OS for opening files automatically.")
            log_message("Report file opened successfully via subprocess.")
            return True
        except Exception as e:
            log_message(f"Could not open report file using subprocess: {e}")
            return False
    except FileNotFoundError:
        log_message(f"Error: Report file not found at '{file_path}'. Please check the path and if the file was generated.")
        return False
    except Exception as e:
        log_message(f"An unexpected error occurred while opening the report file: {e}")
        return False

def run_canoe_automation():
    canoe = None
    try:
        log_message("Creating CANoe COM object...")
        canoe = win32com.client.Dispatch("CANoe.Application")
        log_message("CANoe COM object created successfully.")

        log_message(f"CANoe Version: {canoe.Version.Major}.{canoe.Version.Minor}.{canoe.Version.Build}")

        log_message(f"Loading CANoe configuration file: {CANOE_CONFIG_PATH}")
        canoe.Open(CANOE_CONFIG_PATH)
        log_message("CANoe configuration file loaded successfully.")

        # CANoe가 콘피그 로드 후 완전히 준비될 때까지 대기
        if not wait_for_canoe_ready(canoe, timeout=120): # 타임아웃을 넉넉하게 설정
            raise Exception("CANoe did not enter the ready state after loading the configuration.")

        log_message("Starting CANoe measurement...")
        canoe.Measurement.Start()

        # CANoe 측정이 실제로 시작될 때까지 대기
        if not wait_for_measurement_start(canoe, timeout=60): # 타임아웃을 넉넉하게 설정
            raise Exception("CANoe measurement did not start.")

        log_message(f"Measurement in progress for {MEASUREMENT_DURATION_SECONDS} seconds...")
        time.sleep(MEASUREMENT_DURATION_SECONDS) # 지정된 시간만큼 측정

        log_message("Stopping CANoe measurement...")
        canoe.Measurement.Stop()

        # 측정이 완전히 중지될 때까지 대기
        start_time = time.time()
        log_message("Waiting for CANoe measurement to fully stop...")
        while canoe.Measurement.Running and (time.time() - start_time < 30): # 30초 대기
            time.sleep(1)
        if not canoe.Measurement.Running:
            log_message("CANoe measurement stopped successfully.")
        else:
            log_message("Warning: CANoe measurement did not stop within the specified time.")
        
        log_message("CANoe automation script completed successfully.")

    except Exception as e:
        log_message(f"Error during CANoe automation: {e}")
        # 오류 발생 시 CANoe가 열려있다면 강제 종료 시도
        if canoe:
            try:
                log_message("Terminating CANoe due to an error.")
                canoe.Quit()
            except Exception as ex:
                log_message(f"Failed to forcibly terminate CANoe: {ex}")
        # 젠킨스 빌드 실패를 위해 exit 코드 반환
        exit(1)
    finally:
        # CANoe 종료 로직 (오류 발생 여부와 상관없이 항상 실행)
        if canoe:
            try:
                log_message("Closing CANoe configuration...")
                canoe.Close() # 콘피그 닫기
                log_message("Terminating CANoe application...")
                canoe.Quit() # CANoe 애플리케이션 종료
                log_message("CANoe terminated.")
            except Exception as ex:
                log_message(f"Failed to gracefully close/quit CANoe: {ex}")
        
        # COM 객체 참조 해제 (메모리 누수 방지)
        if 'canoe' in locals() and canoe is not None:
            canoe = None # 중요: COM 객체 참조 해제

        # 측정 종료 후 리포트 파일 열기 (성공 여부와 상관없이 시도)
        if not open_report_file(REPORT_FILE_PATH):
            log_message("Could not automatically open the report file. Please check the path and file association.")
        
        log_message("--- CANoe Automation Script Finished ---")


if __name__ == "__main__":
    # 이전 로그 파일 삭제
    if os.path.exists(LOG_FILE):
        os.remove(LOG_FILE)
    
    run_canoe_automation()
    # 스크립트 성공 시 0 반환 (젠킨스 빌드 성공)
    exit(0)
