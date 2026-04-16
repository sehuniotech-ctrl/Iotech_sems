#include "board_f107_lcu.h"

void board_f107_lcu_init(board_f107_lcu_state_t *state)
{
    if (state == 0)
    {
        return;
    }

    /* 이 함수는 보드 종속 초기화를 한 군데로 모으기 위한 자리다.
     * 데모 단계에서는 실제 HAL 코드가 아직 없으므로 준비 완료 플래그만 세운다.
     *
     * TODO:
     * - Connect the real STM32F107 clock configuration.
     * - Confirm SWD/JTAG debug connection on the LCU board.
     * - Connect the actual system tick source.
     */
    state->clocks_ready = true;
    state->debug_ready = true;
    state->tick_ready = true;
}

uint32_t board_f107_lcu_get_tick_ms(void)
{
    static uint32_t fake_tick_ms = 0U;

    /* Placeholder tick so the demo flow has a complete call chain.
     * Replace this with the real board time base when the LCU board is in hand.
     * 현재는 100ms씩 증가시키며 polling 흐름만 검증한다.
     */
    fake_tick_ms += 100U;
    return fake_tick_ms;
}
