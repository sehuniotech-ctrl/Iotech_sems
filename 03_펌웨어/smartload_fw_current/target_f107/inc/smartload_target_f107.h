#ifndef SMARTLOAD_TARGET_F107_H
#define SMARTLOAD_TARGET_F107_H

#include "lcu_demo_app.h"
#include <stdbool.h>
#include <stdint.h>

/* STM32F107 + LCU board demo target.
 * This layer connects the common ATM90E26 demo logic to the current LCU board.
 * When the real LCU board interface is confirmed, only the transport file
 * should need board-specific changes.
 *
 * 구조상 main.c는 이 target만 호출하고, 실제 계량/보호 로직은 공통 app에 둔다.
 */
typedef struct
{
    lcu_demo_app_t app;
    bool initialized;
} smartload_target_f107_t;

/* Bind the F107 transport layer to the common demo application. */
void smartload_target_f107_init(smartload_target_f107_t *target);

/* Start the ATM90E26 demo sequence on the current target board. */
bool smartload_target_f107_start(smartload_target_f107_t *target);

/* Periodically poll measurements and run demo protection logic. */
bool smartload_target_f107_process(smartload_target_f107_t *target, uint32_t tick_ms);

#endif
