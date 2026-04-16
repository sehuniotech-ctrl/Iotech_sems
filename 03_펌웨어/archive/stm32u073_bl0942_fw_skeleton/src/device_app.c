#include "app_config.h"
#include "device_app.h"
#include "protection.h"
#include "relay_control.h"

static void device_app_update_snapshot(device_app_t *app)
{
    app->snapshot = *bl0942_get_measurement(&app->bl0942);
}

static void device_app_handle_link_cmd(device_app_t *app, link_cmd_t cmd, uint32_t tick_ms)
{
    switch (cmd)
    {
    case LINK_CMD_RELAY_ON:
    case LINK_CMD_REMOTE_RECOVERY:
        relay_control_set(&app->relay, RELAY_CMD_ON, tick_ms);
        break;

    case LINK_CMD_RELAY_OFF:
        relay_control_set(&app->relay, RELAY_CMD_OFF, tick_ms);
        break;

    case LINK_CMD_POLL:
    case LINK_CMD_NONE:
    default:
        break;
    }
}

void device_app_init(device_app_t *app)
{
    bl0942_init(&app->bl0942);
    rs485_link_init(&app->rs485);
    standby_recovery_init(&app->standby);
    relay_control_init(&app->relay);
    app->last_measurement_update_ms = 0U;
}

void device_app_process(device_app_t *app, uint32_t tick_ms)
{
    link_cmd_t cmd = rs485_link_take_cmd(&app->rs485);
    device_app_handle_link_cmd(app, cmd, tick_ms);

    if (bl0942_process_frame(&app->bl0942))
    {
        device_app_update_snapshot(app);
    }

    if ((tick_ms - app->last_measurement_update_ms) >= APP_MEASUREMENT_UPDATE_MS)
    {
        app->last_measurement_update_ms = tick_ms;

        if (protection_check_overcurrent(&app->snapshot))
        {
            relay_control_set(&app->relay, RELAY_CMD_OFF, tick_ms);
            app->relay.last_trip_reason = TRIP_REASON_OVERCURRENT;
        }
        else if (standby_recovery_should_trip(&app->standby, &app->snapshot, tick_ms))
        {
            relay_control_set(&app->relay, RELAY_CMD_OFF, tick_ms);
            app->relay.last_trip_reason = TRIP_REASON_STANDBY;
        }
        else if (app->relay.last_trip_reason == TRIP_REASON_STANDBY &&
                 standby_recovery_should_recover_by_schedule(tick_ms))
        {
            relay_control_set(&app->relay, RELAY_CMD_ON, tick_ms);
            app->relay.last_trip_reason = TRIP_REASON_NONE;
        }
    }
}

void device_app_on_bl0942_rx_byte(device_app_t *app, uint8_t data)
{
    bl0942_push_rx_byte(&app->bl0942, data);
}

void device_app_on_rs485_rx_byte(device_app_t *app, uint8_t data)
{
    rs485_link_push_rx_byte(&app->rs485, data);
}
