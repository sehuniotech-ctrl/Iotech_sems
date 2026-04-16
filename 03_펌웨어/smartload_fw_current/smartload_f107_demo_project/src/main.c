#include "board_f107_lcu.h"
#include "smartload_target_f107.h"

int main(void)
{
    board_f107_lcu_state_t board;
    smartload_target_f107_t target;

    /* 1. 보드 공통 초기화.
     * 실제 LCU 보드에 붙을 때는 여기서 clock, tick, debug, GPIO 같은
     * MCU 기본 준비가 시작된다고 보면 된다.
     */
    board_f107_lcu_init(&board);

    /* 2. 공통 ATM90E26 앱 로직에 F107 보드용 transport를 연결한다.
     * 상위 앱은 계량칩과 직접 대화하지 않고, 아래 target/transport 레이어를
     * 통해 read/write/delay를 호출한다.
     */
    smartload_target_f107_init(&target);

    /* startup 단계에서는 계량칩 soft reset과 demo mode 진입을 시도한다.
     * 아직 transport가 비어 있으면 실패할 수 있지만, main 흐름은 그대로 둔다.
     */
    (void)smartload_target_f107_start(&target);

    while (1)
    {
        /* 3. 주기 polling.
         * 실제로는 다음 순서로 진행된다.
         * - 계량칩 상태 읽기
         * - 전압/전류/전력/주파수 등 측정값 읽기
         * - snapshot 갱신
         * - 간단한 보호 로직 적용
         *
         * 현재 tick은 placeholder지만, 나중에 보드 tick으로 교체하면
         * 같은 구조로 그대로 사용할 수 있다.
         */
        (void)smartload_target_f107_process(&target, board_f107_lcu_get_tick_ms());
    }
}
