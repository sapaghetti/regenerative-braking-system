/*
 * HcSr04.c
 *
 * HC-SR04 초음파 센서 드라이버 구현
 *
 * 이 파일은 HC-SR04 초음파 센서와 TC275 ShieldBuddy 간의 인터페이스를
 * 구현합니다. GTM TIM 모듈을 사용하여 Echo 펄스의 폭을 측정하고, 이를 통해
 * 거리를 계산합니다.
 */

#include "HcSr04.h"

#include "IfxGtm.h"
#include "IfxGtm_Tim_In.h"
#include "IfxPort.h"
#include "IfxStm.h"

/* 내부 사용 상수 */
#define HCSR04_MIN_RANGE_CM       2.0f   /* 최소 유효 측정 범위 (cm) */
#define HCSR04_MAX_RANGE_CM       400.0f /* 최대 유효 측정 범위 (cm) */
#define HCSR04_MIN_VALID_PULSE_US 1.0f   /* 최소 유효 펄스 폭 (µs) */

/*
 * 마이크로초 단위 지연 함수
 * STM 타이머를 사용하여 정확한 마이크로초 지연 구현
 */
static void delay_us(uint32 us)
{
    /* STM 타이머를 사용한 마이크로초 지연 */
    Ifx_STM *stm   = &MODULE_STM0;
    uint32   ticks = IfxStm_getTicksFromMicroseconds(stm, us);
    IfxStm_waitTicks(stm, ticks);
}

/*
 * 초음파 센서 초기화
 * - Trig 핀을 GPIO 출력으로 설정
 * - Echo 핀을 GTM TIM 입력으로 구성
 * - 펄스 길이 측정 모드 설정
 */
int HcSr04_init(HcSr04_t          *hcsr04,
                Ifx_P             *trigPort,
                uint8              trigPin,
                IfxGtm_Tim_TinMap *echoPinMapping)
{
    /* 널 포인터 체크 */
    if ((hcsr04 == NULL) || (trigPort == NULL) || (echoPinMapping == NULL))
    {
        return -1;
    }

    /* 구조체 초기화 */
    hcsr04->trigPort      = trigPort;
    hcsr04->trigPin       = trigPin;
    hcsr04->isInitialized = FALSE;

    /* Trig 핀 초기화 (출력, 초기 상태 LOW) */
    IfxPort_setPinMode(trigPort, trigPin, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinLow(trigPort, trigPin);

    /* Echo 핀을 위한 GTM TIM 초기화 */
    /* GTM 전역 활성화 */
    IfxGtm_enable(&MODULE_GTM);

    /* GTM CMU 클럭 활성화 */
    IfxGtm_Cmu_enableClocks(&MODULE_GTM,
                            IFXGTM_CMU_CLKEN_FXCLK | IFXGTM_CMU_CLKEN_CLK0);

    /* TIM 입력 구성 */
    IfxGtm_Tim_In_Config timConfig;
    IfxGtm_Tim_In_initConfig(&timConfig, &MODULE_GTM);

    /* TIM 입력 핀 및 모드 설정 */
    timConfig.filter.inputPin     = echoPinMapping;
    timConfig.filter.inputPinMode = IfxPort_InputMode_pullDown;

    /* TIM 초기화 */
    IfxGtm_Tim_In_init(&hcsr04->timDrv, &timConfig);

    /* 초기화 성공 플래그 설정 */
    hcsr04->isInitialized = TRUE;

    return 0;
}

/*
 * 거리 측정 트리거
 * HC-SR04 센서는 10µs 트리거 펄스가 필요함
 */
void HcSr04_trigger(HcSr04_t *hcsr04)
{
    /* 초기화 검사 */
    if ((hcsr04 == NULL) || (hcsr04->isInitialized == FALSE))
    {
        return;
    }

    /* 트리거 핀을 10µs 동안 HIGH로 설정하여 측정 시작 */
    IfxPort_setPinLow(hcsr04->trigPort, hcsr04->trigPin);
    delay_us(2); /* 안정화 대기 */
    IfxPort_setPinHigh(hcsr04->trigPort, hcsr04->trigPin);
    delay_us(10); /* HC-SR04 사양에 따른 10µs 트리거 펄스 */
    IfxPort_setPinLow(hcsr04->trigPort, hcsr04->trigPin);
}

/*
 * 센티미터 단위로 거리 측정값 가져오기
 * 음속은 약 340m/s이며, 거리 = (펄스 폭 × 음속) / 2
 * 간소화된 공식: 거리(cm) = 펄스 폭(µs) / 58
 */
float32 HcSr04_getDistanceCm(HcSr04_t *hcsr04)
{
    float32 duration;
    float32 distance;

    /* 초기화 검사 */
    if ((hcsr04 == NULL) || (hcsr04->isInitialized == FALSE))
    {
        return -1.0f;
    }

    /* TIM 모듈 업데이트로 최신 측정값 가져오기 */
    IfxGtm_Tim_In_update(&hcsr04->timDrv);

    /* 펄스 폭 초 단위로 가져오기 후 마이크로초로 변환 */
    duration = (float32)(IfxGtm_Tim_In_getPulseLengthTick(&hcsr04->timDrv)
                         / hcsr04->timDrv.captureClockFrequency * 1000000.0f);

    /* 펄스가 너무 짧거나 없으면 오류 */
    if (duration < HCSR04_MIN_VALID_PULSE_US)
    {
        return -1.0f;
    }

    /* 거리 계산 (거리[cm] = 펄스 폭[µs] / 58) */
    distance = duration / 58.0f;

    /* 유효 범위 확인 (HC-SR04 사양: 2cm ~ 400cm) */
    if ((distance < HCSR04_MIN_RANGE_CM) || (distance > HCSR04_MAX_RANGE_CM))
    {
        return -1.0f; /* 범위 초과시 -1 반환 */
    }

    return distance;
}

/*
 * 최대 대기 시간(ms) 내에 거리 측정
 * 트리거 송신 후 측정 결과를 기다리는 함수
 */
float32 HcSr04_measureDistanceCm(HcSr04_t *hcsr04, uint32 timeoutMs)
{
    float32  distance         = -1.0f;
    boolean  validMeasurement = FALSE;
    uint64   startTime, currentTime;
    Ifx_STM *stm = &MODULE_STM0;

    /* 초기화 검사 */
    if ((hcsr04 == NULL) || (hcsr04->isInitialized == FALSE))
    {
        return -1.0f;
    }

    /* 현재 시간 저장 */
    startTime = IfxStm_get(stm);

    /* 트리거 실행 */
    HcSr04_trigger(hcsr04);

    /* 타임아웃 내에 유효한 측정값을 기다림 */
    do
    {
        /* 거리 측정 시도 */
        distance = HcSr04_getDistanceCm(hcsr04);

        /* 유효한 측정값이면 종료 */
        if (distance >= 0.0f)
        {
            validMeasurement = TRUE;
            break;
        }

        /* 타임아웃 확인 */
        currentTime = IfxStm_get(stm);
        if (IfxStm_getTicksFromMilliseconds(stm, timeoutMs)
            < (currentTime - startTime))
        {
            break; /* 타임아웃 발생 */
        }

        /* 짧은 대기 후 재시도 */
        IfxStm_waitTicks(stm, IfxStm_getTicksFromMicroseconds(stm, 500));

    } while (!validMeasurement);

    return distance;
}
