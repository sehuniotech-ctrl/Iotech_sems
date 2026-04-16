#include "atm90e26.h"

#include "atm90e26_regs.h"

/* ATM90E26 레지스터는 항목마다 스케일이 다르다.
 * 예를 들어 전압/전류/주파수/전력은 raw 16-bit 값을 그대로 쓰지 않고
 * 각 레지스터 규칙에 맞게 나눠서 engineering unit으로 바꿔야 한다.
 *
 * 아래 helper들은 그 변환을 담당한다.
 */
static float atm90e26_u16_to_voltage(uint16_t raw)
{
    return ((float)raw) / 100.0f;
}

static float atm90e26_u16_to_current(uint16_t raw)
{
    return ((float)raw) / 1000.0f;
}

static float atm90e26_u16_to_frequency(uint16_t raw)
{
    return ((float)raw) / 100.0f;
}

static float atm90e26_s16_to_signed_scale(uint16_t raw, float divisor)
{
    return ((float)((int16_t)raw)) / divisor;
}

static float atm90e26_u16_to_positive_scale(uint16_t raw, float divisor)
{
    return ((float)raw) / divisor;
}

void atm90e26_init(atm90e26_ctx_t *ctx,
                   atm90e26_read_reg_fn read_reg,
                   atm90e26_write_reg_fn write_reg,
                   atm90e26_delay_ms_fn delay_ms)
{
    if (ctx == 0)
    {
        return;
    }

    /* ctx는 공통 ATM90E26 드라이버의 실행 상태다.
     * 여기서 보드별 함수 포인터와 측정값 저장 공간을 초기화한다.
     */
    ctx->read_reg = read_reg;
    ctx->write_reg = write_reg;
    ctx->delay_ms = delay_ms;
    ctx->initialized = false;
    ctx->measurement.voltage_v = 0.0f;
    ctx->measurement.line_current_a = 0.0f;
    ctx->measurement.neutral_current_a = 0.0f;
    ctx->measurement.frequency_hz = 0.0f;
    ctx->measurement.active_power_w = 0.0f;
    ctx->measurement.reactive_power_var = 0.0f;
    ctx->measurement.apparent_power_va = 0.0f;
    ctx->measurement.power_factor = 0.0f;
    ctx->measurement.phase_angle_deg = 0.0f;
    ctx->measurement.active_energy_wh = 0.0f;
    ctx->measurement.reactive_energy_varh = 0.0f;
}

bool atm90e26_soft_reset(atm90e26_ctx_t *ctx)
{
    if ((ctx == 0) || (ctx->write_reg == 0))
    {
        return false;
    }

    /* soft reset은 bring-up에서 가장 먼저 확인할 write 경로다.
     * 이게 성공하면 최소한 계량칩 write transaction은 붙었다고 볼 수 있다.
     */
    if (!ctx->write_reg(ATM90E26_REG_SOFT_RESET, ATM90E26_SOFT_RESET_KEY))
    {
        return false;
    }

    if (ctx->delay_ms != 0)
    {
        ctx->delay_ms(10U);
    }

    return true;
}

bool atm90e26_startup_demo_mode(atm90e26_ctx_t *ctx)
{
    if ((ctx == 0) || (ctx->write_reg == 0))
    {
        return false;
    }

    if (!atm90e26_soft_reset(ctx))
    {
        return false;
    }

    /* demo mode startup은 1차 bring-up용 진입 루틴이다.
     * 정식 calibration을 모두 준비하기 전에 아래 3단계만으로 통신을 먼저 확인한다.
     * - soft reset
     * - CalStart = 0x5678
     * - AdjStart = 0x5678
     *
     * 즉, "보드와 계량칩이 대화 가능한지"를 가장 짧게 검증하는 경로다.
     */
    if (!ctx->write_reg(ATM90E26_REG_CAL_START, ATM90E26_STARTUP_BYPASS_KEY))
    {
        return false;
    }

    if (!ctx->write_reg(ATM90E26_REG_ADJ_START, ATM90E26_STARTUP_BYPASS_KEY))
    {
        return false;
    }

    if (ctx->delay_ms != 0)
    {
        ctx->delay_ms(20U);
    }

    ctx->initialized = true;
    return true;
}

bool atm90e26_read_sys_status(atm90e26_ctx_t *ctx, uint16_t *status)
{
    if ((ctx == 0) || (ctx->read_reg == 0) || (status == 0))
    {
        return false;
    }

    return ctx->read_reg(ATM90E26_REG_SYS_STATUS, status);
}

bool atm90e26_read_all_measurements(atm90e26_ctx_t *ctx)
{
    uint16_t raw = 0U;

    if ((ctx == 0) || !ctx->initialized || (ctx->read_reg == 0))
    {
        return false;
    }

    /* 각 레지스터 read를 일부러 독립적으로 처리한다.
     * 이유는 bring-up 때 특정 레지스터 하나가 실패하더라도
     * 나머지 측정값이 어디까지 읽히는지 계속 보기 위해서다.
     */
    if (ctx->read_reg(ATM90E26_REG_URMS, &raw))
    {
        ctx->measurement.voltage_v = atm90e26_u16_to_voltage(raw);
    }

    if (ctx->read_reg(ATM90E26_REG_IRMS, &raw))
    {
        ctx->measurement.line_current_a = atm90e26_u16_to_current(raw);
    }

    if (ctx->read_reg(ATM90E26_REG_IRMS2, &raw))
    {
        ctx->measurement.neutral_current_a = atm90e26_u16_to_current(raw);
    }

    if (ctx->read_reg(ATM90E26_REG_PMEAN, &raw))
    {
        ctx->measurement.active_power_w = atm90e26_s16_to_signed_scale(raw, 1000.0f);
    }

    if (ctx->read_reg(ATM90E26_REG_QMEAN, &raw))
    {
        ctx->measurement.reactive_power_var = atm90e26_s16_to_signed_scale(raw, 1000.0f);
    }

    if (ctx->read_reg(ATM90E26_REG_SMEAN, &raw))
    {
        ctx->measurement.apparent_power_va = atm90e26_u16_to_positive_scale(raw, 1000.0f);
    }

    if (ctx->read_reg(ATM90E26_REG_POWERF, &raw))
    {
        ctx->measurement.power_factor = atm90e26_s16_to_signed_scale(raw, 1000.0f);
    }

    if (ctx->read_reg(ATM90E26_REG_FREQ, &raw))
    {
        ctx->measurement.frequency_hz = atm90e26_u16_to_frequency(raw);
    }

    if (ctx->read_reg(ATM90E26_REG_PANGLE, &raw))
    {
        ctx->measurement.phase_angle_deg = atm90e26_s16_to_signed_scale(raw, 10.0f);
    }

    /* 에너지 레지스터는 누적 성격이라, 데모에서는 단순히 계속 더해가며
     * 값이 변하는지만 먼저 확인한다.
     */
    if (ctx->read_reg(ATM90E26_REG_AP_ENERGY, &raw))
    {
        ctx->measurement.active_energy_wh += (float)raw;
    }

    if (ctx->read_reg(ATM90E26_REG_RP_ENERGY, &raw))
    {
        ctx->measurement.reactive_energy_varh += (float)raw;
    }

    return true;
}

const measurement_t *atm90e26_get_measurement(const atm90e26_ctx_t *ctx)
{
    return (ctx == 0) ? 0 : &ctx->measurement;
}
