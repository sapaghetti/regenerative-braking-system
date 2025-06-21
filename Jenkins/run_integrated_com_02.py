import win32com.client
import time
import os
import sys

# 설정값
CFG_PATH = r"D:\can\CANoe\RBS.cfg"
MEASUREMENT_DURATION = 60  # 초

def log(msg):
    print(msg)

def run():
    try:
        # CANoe 실행
        canoe = win32com.client.Dispatch("CANoe.Application")
        canoe.Open(CFG_PATH)
        log("CANoe config loaded.")

        # 준비 대기
        timeout = 60
        start = time.time()
        while not canoe.Measurement.Running and time.time() - start < timeout:
            time.sleep(1)
        if not canoe.Measurement.Running:
            raise Exception("CANoe not ready in time.")

        # 측정 시작
        log("Starting measurement...")
        canoe.Measurement.Start()

        # 측정 시작 확인
        while not canoe.Measurement.Running:
            time.sleep(1)
        log("Measurement started.")

        # Test Configuration 실행
        test_module = canoe.TestSetup
        log("Starting vTESTstudio Test Configuration 1...")
        test_module.Run()
        log("Test Configuration started.")

        # 원하는 시간 측정
        log(f"Waiting for {MEASUREMENT_DURATION} seconds...")
        time.sleep(MEASUREMENT_DURATION)

        # 종료
        log("Stopping measurement...")
        canoe.Measurement.Stop()

        # CANoe 종료
        canoe.Quit()
        log("Automation complete.")

    except Exception as e:
        log(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    run()
