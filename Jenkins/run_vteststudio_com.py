import win32com.client
import time
import os
import sys

# === 설정 ===
VTEST_PROJECT_PATH = r"D:\can\vTESTstudio\RBS.vtsoproj"
CANOE_CFG_PATH = r"D:\can\CANoe\RBS.cfg"
REPORT_DIR = r"D:\can\vTESTstudio\TestReports"
MEASUREMENT_DURATION = 60  # 측정 시간 (초)

def log(msg):
    print(msg)

def run():
    try:
        log("▶ Launching vTESTstudio...")
        vtest = win32com.client.Dispatch("vTESTstudio.Application")
        project = vtest.OpenProject(VTEST_PROJECT_PATH)
        log("✅ Project opened.")

        log("▶ Building vTESTstudio project...")
        project.Build()
        time.sleep(2)  # 빌드 시간 여유

        if not project.BuildSucceeded:
            log("❌ Build failed.")
            sys.exit(1)
        log("✅ Build successful.")

        # COM 통해 CANoe 실행
        log("▶ Launching CANoe...")
        canoe = win32com.client.Dispatch("CANoe.Application")
        canoe.Open(CANOE_CFG_PATH)
        log("✅ CANoe config loaded.")

        # 준비 대기
        timeout = 60
        start = time.time()
        log("▶ Waiting for CANoe to become ready...")
        while not canoe.Measurement.Running and time.time() - start < timeout:
            if not canoe.Measurement.Running:
                break
            time.sleep(1)
        log("✅ CANoe is ready.")

        # 측정 시작
        log("▶ Starting measurement...")
        canoe.Measurement.Start()

        # 실제 시작 확인
        wait_time = 0
        while not canoe.Measurement.Running and wait_time < 10:
            time.sleep(1)
            wait_time += 1

        if not canoe.Measurement.Running:
            log("❌ Measurement did not start.")
            sys.exit(1)
        log("✅ Measurement started.")

        log(f"⏱ Measuring for {MEASUREMENT_DURATION} seconds...")
        time.sleep(MEASUREMENT_DURATION)

        log("⏹ Stopping measurement...")
        canoe.Measurement.Stop()
        time.sleep(2)

        log("✅ Measurement stopped.")

        # 리포트 저장 위치 확인 또는 복사 (CANoe 설정에서 설정돼 있어야 함)
        if os.path.exists(REPORT_DIR):
            log(f"📁 Report directory exists: {REPORT_DIR}")
        else:
            log("⚠️ Report directory not found. Please check CANoe report generation settings.")

        log("🚪 Quitting CANoe...")
        canoe.Quit()

        log("✅ Script completed successfully.")

    except Exception as e:
        log(f"❌ Error occurred: {e}")
        sys.exit(1)

if __name__ == "__main__":
    run()
