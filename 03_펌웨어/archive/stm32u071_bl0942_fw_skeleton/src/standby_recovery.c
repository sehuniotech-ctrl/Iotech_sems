#include "app_config.h"
#include "standby_recovery.h"

void standby_recovery_init(standby_ctx_t *ctx)
{
    ctx->low_power_start_ms = 0U;
    ctx->standby_trip_active = false;
}

bool standby_recovery_should_trip(standby_ctx_t *ctx,
                                  const measurement_t *measurement,
                                  uint32_t tick_ms)
{
    if (measurement->active_power_w < APP_STANDBY_POWER_LIMIT_W)
    {
        if (ctx->low_power_start_ms == 0U)
        {
            ctx->low_power_start_ms = tick_ms;
        }

        if ((tick_ms - ctx->low_power_start_ms) >= APP_STANDBY_DELAY_MS)
        {
            ctx->standby_trip_active = true;
            return true;
        }
    }
    else
    {
        ctx->low_power_start_ms = 0U;
        ctx->standby_trip_active = false;
    }

    return false;
}

bool standby_recovery_should_recover_by_schedule(uint32_t tick_ms)
{
    /* TODO: Replace with RTC/time table logic from DCU schedule policy. */
    (void)tick_ms;
    return false;
}
