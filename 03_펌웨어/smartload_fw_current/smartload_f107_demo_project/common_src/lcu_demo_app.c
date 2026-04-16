#include "lcu_demo_app.h"

#include "app_config.h"

/* Copy the latest chip measurements into an application snapshot so later
 * logic can work on a stable set of values for one polling cycle.
 */
static void lcu_demo_snapshot(lcu_demo_app_t *app)
{
    const measurement_t *m = atm90e26_get_measurement(&app->meter);
    if (m != 0)
    {
        app->snapshot = *m;
    }
}

void lcu_demo_app_init(lcu_demo_app_t *app,
                       atm90e26_read_reg_fn read_reg,
                       atm90e26_write_reg_fn write_reg,
                       atm90e26_delay_ms_fn delay_ms)
{
    if (app == 0)
    {
        return;
    }

    /* 이 함수는 "공통 앱 상태"를 초기화한다.
     * 중요한 점은, 여기서는 SPI/UART/HAL 같은 보드 세부사항을 모른다는 점이다.
     * 보드별 read/write/delay 함수만 주입받고 나머지 로직은 공통으로 재사용한다.
     */
    atm90e26_init(&app->meter, read_reg, write_reg, delay_ms);
    app->snapshot.voltage_v = 0.0f;
    app->snapshot.line_current_a = 0.0f;
    app->snapshot.neutral_current_a = 0.0f;
    app->snapshot.frequency_hz = 0.0f;
    app->snapshot.active_power_w = 0.0f;
    app->snapshot.reactive_power_var = 0.0f;
    app->snapshot.apparent_power_va = 0.0f;
    app->snapshot.power_factor = 0.0f;
    app->snapshot.phase_angle_deg = 0.0f;
    app->snapshot.active_energy_wh = 0.0f;
    app->snapshot.reactive_energy_varh = 0.0f;
    app->relay.relay_on = true;
    app->relay.last_cmd = RELAY_CMD_NONE;
    app->relay.last_trip_reason = TRIP_REASON_NONE;
    app->relay.last_cmd_tick_ms = 0U;
    app->last_sys_status = 0U;
    app->last_update_ms = 0U;
    app->meter_ready = false;
}

bool lcu_demo_app_start_meter(lcu_demo_app_t *app)
{
    if (app == 0)
    {
        return false;
    }

    /* bring-up 초기에는 calibration보다 "통신이 되는지" 확인이 먼저다.
     * 그래서 demo mode startup을 먼저 사용한다.
     * 이 단계가 성공하면:
     * - write_reg 경로가 살아 있고
     * - delay 함수가 연결되어 있고
     * - 이후 read_reg 검증으로 넘어갈 수 있다.
     */
    app->meter_ready = atm90e26_startup_demo_mode(&app->meter);
    return app->meter_ready;
}

bool lcu_demo_app_poll(lcu_demo_app_t *app, uint32_t tick_ms)
{
    if ((app == 0) || !app->meter_ready)
    {
        return false;
    }

    /* polling 주기를 고정해 두면 bring-up 때 로그 비교가 쉬워진다.
     * 나중에 RS-485 주기 polling과도 같은 구조로 이어가기 좋다.
     */
    if ((tick_ms - app->last_update_ms) < APP_MEASUREMENT_UPDATE_MS)
    {
        return true;
    }

    app->last_update_ms = tick_ms;

    /* 상태 레지스터를 먼저 읽는 이유:
     * 측정값과 함께 warning/checksum 관련 상태도 같은 주기 안에서 보기 위해서다.
     */
    if (!atm90e26_read_sys_status(&app->meter, &app->last_sys_status))
    {
        return false;
    }

    if (!atm90e26_read_all_measurements(&app->meter))
    {
        return false;
    }

    lcu_demo_snapshot(app);

    /* 여기 보호 로직은 제품 최종판이라기보다 bring-up/데모용 확인 포인트다.
     * 즉, 측정값이 정상적으로 올라오면 relay/trip 상태도 같이 검증할 수 있다.
     */
    if (app->snapshot.line_current_a >= APP_OVERCURRENT_LIMIT_A)
    {
        app->relay.relay_on = false;
        app->relay.last_cmd = RELAY_CMD_OFF;
        app->relay.last_trip_reason = TRIP_REASON_OVERCURRENT;
        app->relay.last_cmd_tick_ms = tick_ms;
    }
    else if (app->snapshot.voltage_v >= APP_OVERVOLTAGE_LIMIT_V)
    {
        app->relay.relay_on = false;
        app->relay.last_cmd = RELAY_CMD_OFF;
        app->relay.last_trip_reason = TRIP_REASON_OVERVOLTAGE;
        app->relay.last_cmd_tick_ms = tick_ms;
    }
    else if (app->snapshot.active_power_w <= APP_STANDBY_POWER_LIMIT_W)
    {
        app->relay.last_trip_reason = TRIP_REASON_STANDBY;
    }

    return true;
}
