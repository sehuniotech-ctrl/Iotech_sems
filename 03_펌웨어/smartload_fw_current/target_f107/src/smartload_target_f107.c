#include "smartload_target_f107.h"

#include "atm90e26_transport_f107_lcu.h"

void smartload_target_f107_init(smartload_target_f107_t *target)
{
    if (target == 0)
    {
        return;
    }

    /* transport는 "공통 계량칩 로직"과 "실제 F107 보드"를 연결하는 어댑터다.
     * 회로도만 확인되면 이 아래 transport 파일에 핀/버스를 붙이면 된다.
     */
    atm90e26_transport_f107_init();

    /* 공통 demo app에 보드 전용 함수 포인터를 주입한다.
     * 이후 상위 로직은 아래 함수들을 통해서만 계량칩에 접근한다.
     * - read_reg
     * - write_reg
     * - delay_ms
     */
    lcu_demo_app_init(&target->app,
                      atm90e26_transport_f107_read_reg,
                      atm90e26_transport_f107_write_reg,
                      atm90e26_transport_f107_delay_ms);

    target->initialized = true;
}

bool smartload_target_f107_start(smartload_target_f107_t *target)
{
    if ((target == 0) || !target->initialized)
    {
        return false;
    }

    /* 1차 bring-up 목표는 아주 단순하다.
     * - 계량칩 write가 되는지
     * - 계량칩 read가 되는지
     *
     * 그래서 먼저 demo mode startup만 통과시키는 구조다.
     */
    return lcu_demo_app_start_meter(&target->app);
}

bool smartload_target_f107_process(smartload_target_f107_t *target, uint32_t tick_ms)
{
    if ((target == 0) || !target->initialized)
    {
        return false;
    }

    /* 실제 측정/보호 동작은 공통 app에 들어 있다.
     * target 레이어는 "이 보드에서 그 app를 돌릴 수 있게 연결해 주는 역할"만 한다.
     */
    return lcu_demo_app_poll(&target->app, tick_ms);
}
