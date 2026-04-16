#include "relay_control.h"

void relay_control_init(relay_state_t *state)
{
    state->relay_on = true;
    state->last_cmd = RELAY_CMD_NONE;
    state->last_trip_reason = TRIP_REASON_NONE;
    state->last_cmd_tick_ms = 0U;
}

void relay_control_set(relay_state_t *state, relay_cmd_t cmd, uint32_t tick_ms)
{
    state->last_cmd = cmd;
    state->last_cmd_tick_ms = tick_ms;

    if (cmd == RELAY_CMD_ON)
    {
        state->relay_on = true;
    }
    else if (cmd == RELAY_CMD_OFF)
    {
        state->relay_on = false;
    }

    /* TODO: Drive real relay GPIO here. */
}

bool relay_control_is_on(const relay_state_t *state)
{
    return state->relay_on;
}
