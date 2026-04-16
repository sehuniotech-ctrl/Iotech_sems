#ifndef BOARD_F107_LCU_H
#define BOARD_F107_LCU_H

#include <stdbool.h>
#include <stdint.h>

/* Minimal board support layer for the current STM32F107 LCU demo target.
 * Keep board setup separate from the ATM90E26 demo logic so the application
 * flow is easy to read from main.c.
 */
typedef struct
{
    bool clocks_ready;
    bool debug_ready;
    bool tick_ready;
} board_f107_lcu_state_t;

/* Prepare basic board state needed before the demo target starts.
 * 실제 LCU 프로젝트에 붙일 때는 clock, debug, gpio, tick 초기화 진입점이 된다.
 */
void board_f107_lcu_init(board_f107_lcu_state_t *state);

/* Return a monotonically increasing millisecond tick for polling logic.
 * This is a placeholder until the real LCU tick source is connected.
 */
uint32_t board_f107_lcu_get_tick_ms(void);

#endif
