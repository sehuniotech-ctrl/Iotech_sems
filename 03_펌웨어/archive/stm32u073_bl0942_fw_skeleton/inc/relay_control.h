#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include "app_types.h"

void relay_control_init(relay_state_t *state);
void relay_control_set(relay_state_t *state, relay_cmd_t cmd, uint32_t tick_ms);
bool relay_control_is_on(const relay_state_t *state);

#endif
