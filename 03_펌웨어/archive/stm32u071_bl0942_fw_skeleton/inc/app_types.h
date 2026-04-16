#ifndef APP_TYPES_H
#define APP_TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    RELAY_CMD_NONE = 0,
    RELAY_CMD_ON,
    RELAY_CMD_OFF
} relay_cmd_t;

typedef enum
{
    TRIP_REASON_NONE = 0,
    TRIP_REASON_STANDBY,
    TRIP_REASON_OVERCURRENT,
    TRIP_REASON_OVERVOLTAGE
} trip_reason_t;

typedef struct
{
    float voltage_v;
    float current_a;
    float frequency_hz;
    float active_power_w;
    float active_energy_wh;
    float fast_current_rms_a;
    float apparent_power_va;
    float power_factor;
} measurement_t;

typedef struct
{
    bool relay_on;
    relay_cmd_t last_cmd;
    trip_reason_t last_trip_reason;
    uint32_t last_cmd_tick_ms;
} relay_state_t;

#endif
