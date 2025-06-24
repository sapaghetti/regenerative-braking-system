/*
 * HcSr04.h
 *
 * HC-SR04 초음파 센서 드라이버 헤더 파일
 *
 * 이 드라이버는 TC275 ShieldBuddy 보드에서 HC-SR04 초음파 센서를 사용하기 위한
 * 인터페이스를 제공합니다. GTM TIM 모듈을 사용하여 에코 펄스 폭을 측정합니다.
 *
 * 참고 사항:
 * - Trig 핀은 일반 GPIO로 제어
 * - Echo 핀은 GTM TIM 입력으로 연결
 */

#ifndef HCSR04_H
#define HCSR04_H

#include "IfxGtm_Tim_In.h"
#include "Ifx_Types.h"

/**
 * HC-SR04 센서 구성 구조체
 */
typedef struct
{
    Ifx_P        *trigPort;      /* Trig 핀 포트 (예: &MODULE_P10) */
    uint8         trigPin;       /* Trig 핀 번호 (예: 5 for P10.5) */
    IfxGtm_Tim_In timDrv;        /* TIM 드라이버 (Echo 펄스 측정용) */
    boolean       isInitialized; /* 초기화 상태 플래그 */
} HcSr04_t;

/**
 * HC-SR04 초음파 센서 초기화
 *
 * @param hcsr04 센서 구조체 포인터
 * @param trigPort Trig 핀의 포트 (예: &MODULE_P10)
 * @param trigPin Trig 핀 번호 (예: 5 for P10.5)
 * @param echoPinMapping Echo 핀의 TIM 맵핑 (예: &IfxGtm_TIM0_7_TIN7_P02_7_IN)
 * @return 성공 시 0, 실패 시 음수 값
 */
int HcSr04_init(HcSr04_t          *hcsr04,
                Ifx_P             *trigPort,
                uint8              trigPin,
                IfxGtm_Tim_TinMap *echoPinMapping);

/**
 * 거리 측정 트리거
 * 10us 펄스를 생성하여 측정 시작
 *
 * @param hcsr04 센서 구조체 포인터
 */
void HcSr04_trigger(HcSr04_t *hcsr04);

/**
 * 센티미터 단위로 거리 측정값 가져오기
 * 거리(cm) = 펄스 폭(µs) / 58
 *
 * @param hcsr04 센서 구조체 포인터
 * @return 거리(cm), 오류 또는 범위 외(2cm~400cm)일 경우 -1.0
 */
float32 HcSr04_getDistanceCm(HcSr04_t *hcsr04);

/**
 * 최대 대기 시간(ms) 내에 거리 측정
 * 트리거 후 측정 완료까지 대기하는 함수
 *
 * @param hcsr04 센서 구조체 포인터
 * @param timeoutMs 타임아웃 시간(ms)
 * @return 거리(cm), 타임아웃 또는 오류 시 -1.0
 */
float32 HcSr04_measureDistanceCm(HcSr04_t *hcsr04, uint32 timeoutMs);

#endif /* HCSR04_H */
