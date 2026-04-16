#include "app_config.h"
#include "bl0942.h"

static void bl0942_update_derived_values(measurement_t *m)
{
    m->apparent_power_va = m->voltage_v * m->current_a;

    if (m->apparent_power_va > 0.001f)
    {
        m->power_factor = m->active_power_w / m->apparent_power_va;
    }
    else
    {
        m->power_factor = 0.0f;
    }
}

void bl0942_init(bl0942_ctx_t *ctx)
{
    ctx->rx_len = 0U;
    ctx->frame_ready = false;
    ctx->measurement.voltage_v = 0.0f;
    ctx->measurement.current_a = 0.0f;
    ctx->measurement.frequency_hz = 0.0f;
    ctx->measurement.active_power_w = 0.0f;
    ctx->measurement.active_energy_wh = 0.0f;
    ctx->measurement.fast_current_rms_a = 0.0f;
    ctx->measurement.apparent_power_va = 0.0f;
    ctx->measurement.power_factor = 0.0f;
}

void bl0942_push_rx_byte(bl0942_ctx_t *ctx, uint8_t data)
{
    if (ctx->rx_len < APP_BL0942_RX_BUF_SIZE)
    {
        ctx->rx_buf[ctx->rx_len++] = data;
    }

    if (ctx->rx_len >= 10U)
    {
        ctx->frame_ready = true;
    }
}

bool bl0942_process_frame(bl0942_ctx_t *ctx)
{
    if (!ctx->frame_ready)
    {
        return false;
    }

    /* TODO: Replace this stub with real BL0942 frame parsing and calibration. */
    ctx->measurement.voltage_v = 220.0f;
    ctx->measurement.current_a = 0.25f;
    ctx->measurement.frequency_hz = 60.0f;
    ctx->measurement.active_power_w = 18.0f;
    ctx->measurement.active_energy_wh += 0.005f;
    ctx->measurement.fast_current_rms_a = ctx->measurement.current_a;
    bl0942_update_derived_values(&ctx->measurement);

    ctx->rx_len = 0U;
    ctx->frame_ready = false;
    return true;
}

const measurement_t *bl0942_get_measurement(const bl0942_ctx_t *ctx)
{
    return &ctx->measurement;
}
