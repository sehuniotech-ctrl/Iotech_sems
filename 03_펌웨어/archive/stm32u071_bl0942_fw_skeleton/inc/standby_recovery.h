#ifndef STANDBY_RECOVERY_H
#define STANDBY_RECOVERY_H

#include "app_types.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint32_t low_power_start_ms;
    bool standby_trip_active;
} standby_ctx_t;

void standby_recovery_init(standby_ctx_t *ctx);
bool standby_recovery_should_trip(standby_ctx_t *ctx,
                                  const measurement_t *measurement,
                                  uint32_t tick_ms);
bool standby_recovery_should_recover_by_schedule(uint32_t tick_ms);

#endif
