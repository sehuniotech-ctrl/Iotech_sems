#ifndef SMARTLOAD_TARGET_L053_H
#define SMARTLOAD_TARGET_L053_H

#include "lcu_demo_app.h"
#include <stdbool.h>
#include <stdint.h>

/* STM32L053 final-board wrapper.
 * CubeMX/HAL 초기화 이후 이 모듈에서 공통 ATM90E26 데모 코드를 호출한다.
 */
typedef struct
{
    lcu_demo_app_t app;
    bool initialized;
} smartload_target_l053_t;

/* Bind STM32L053 HAL transport to the common demo application. */
void smartload_target_l053_init(smartload_target_l053_t *target);

/* Start metering after the target transport is ready. */
bool smartload_target_l053_start(smartload_target_l053_t *target);

/* Run one periodic processing cycle on the final target. */
bool smartload_target_l053_process(smartload_target_l053_t *target, uint32_t tick_ms);

#endif
