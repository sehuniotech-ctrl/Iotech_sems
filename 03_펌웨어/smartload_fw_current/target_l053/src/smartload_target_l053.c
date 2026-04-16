#include "smartload_target_l053.h"

#include "atm90e26_transport_l053_hal.h"

void smartload_target_l053_init(smartload_target_l053_t *target)
{
    if (target == 0)
    {
        return;
    }

    /* CubeMX generated peripheral init must be completed before this wrapper
     * is called. After that, the target only binds HAL transport callbacks to
     * the common ATM90E26 demo app.
     */
    atm90e26_transport_l053_init();

    lcu_demo_app_init(&target->app,
                      atm90e26_transport_l053_read_reg,
                      atm90e26_transport_l053_write_reg,
                      atm90e26_transport_l053_delay_ms);

    target->initialized = true;
}

bool smartload_target_l053_start(smartload_target_l053_t *target)
{
    if ((target == 0) || !target->initialized)
    {
        return false;
    }

    /* Reuse the exact same metering startup sequence proven during the LCU
     * demo so the final-board bring-up path stays short.
     */
    return lcu_demo_app_start_meter(&target->app);
}

bool smartload_target_l053_process(smartload_target_l053_t *target, uint32_t tick_ms)
{
    if ((target == 0) || !target->initialized)
    {
        return false;
    }

    /* The final target also runs the common poll loop. Only transport and
     * hardware initialization should differ by board.
     */
    return lcu_demo_app_poll(&target->app, tick_ms);
}
