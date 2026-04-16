#include "atm90e26.h"

/* Placeholder register addresses for structure only.
 * Replace with actual ATM90E26 register map when HW interface is fixed.
 */
#define ATM90E26_REG_URMS        0x0001U
#define ATM90E26_REG_IRMS        0x0002U
#define ATM90E26_REG_IRMSN       0x0003U
#define ATM90E26_REG_PMEAN       0x0004U
#define ATM90E26_REG_QMEAN       0x0005U
#define ATM90E26_REG_SMEAN       0x0006U
#define ATM90E26_REG_POWERF      0x0007U
#define ATM90E26_REG_FREQ        0x0008U
#define ATM90E26_REG_PANGLE      0x0009U

static float atm90e26_scale_u16(uint16_t raw, float scale)
{
    return ((float)raw) * scale;
}

void atm90e26_init(atm90e26_ctx_t *ctx,
                   atm90e26_read_reg_fn read_reg,
                   atm90e26_write_reg_fn write_reg)
{
    ctx->read_reg = read_reg;
    ctx->write_reg = write_reg;
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

bool atm90e26_configure(atm90e26_ctx_t *ctx)
{
    if ((ctx->read_reg == 0) || (ctx->write_reg == 0))
    {
        return false;
    }

    /* TODO: Apply ATM90E26 initialization sequence and calibration registers. */
    ctx->initialized = true;
    return true;
}

bool atm90e26_read_measurement(atm90e26_ctx_t *ctx)
{
    uint16_t raw = 0U;

    if (!ctx->initialized || (ctx->read_reg == 0))
    {
        return false;
    }

    if (ctx->read_reg(ATM90E26_REG_URMS, &raw))
    {
        ctx->measurement.voltage_v = atm90e26_scale_u16(raw, 0.01f);
    }
    if (ctx->read_reg(ATM90E26_REG_IRMS, &raw))
    {
        ctx->measurement.line_current_a = atm90e26_scale_u16(raw, 0.001f);
    }
    if (ctx->read_reg(ATM90E26_REG_IRMSN, &raw))
    {
        ctx->measurement.neutral_current_a = atm90e26_scale_u16(raw, 0.001f);
    }
    if (ctx->read_reg(ATM90E26_REG_PMEAN, &raw))
    {
        ctx->measurement.active_power_w = atm90e26_scale_u16(raw, 0.1f);
    }
    if (ctx->read_reg(ATM90E26_REG_QMEAN, &raw))
    {
        ctx->measurement.reactive_power_var = atm90e26_scale_u16(raw, 0.1f);
    }
    if (ctx->read_reg(ATM90E26_REG_SMEAN, &raw))
    {
        ctx->measurement.apparent_power_va = atm90e26_scale_u16(raw, 0.1f);
    }
    if (ctx->read_reg(ATM90E26_REG_POWERF, &raw))
    {
        ctx->measurement.power_factor = atm90e26_scale_u16(raw, 0.001f);
    }
    if (ctx->read_reg(ATM90E26_REG_FREQ, &raw))
    {
        ctx->measurement.frequency_hz = atm90e26_scale_u16(raw, 0.01f);
    }
    if (ctx->read_reg(ATM90E26_REG_PANGLE, &raw))
    {
        ctx->measurement.phase_angle_deg = atm90e26_scale_u16(raw, 0.1f);
    }

    return true;
}

const measurement_t *atm90e26_get_measurement(const atm90e26_ctx_t *ctx)
{
    return &ctx->measurement;
}
