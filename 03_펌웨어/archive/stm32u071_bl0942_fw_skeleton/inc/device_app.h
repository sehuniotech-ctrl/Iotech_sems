#ifndef DEVICE_APP_H
#define DEVICE_APP_H

#include "app_types.h"
#include "bl0942.h"
#include "rs485_link.h"
#include "standby_recovery.h"

typedef struct
{
    bl0942_ctx_t bl0942;
    rs485_ctx_t rs485;
    standby_ctx_t standby;
    relay_state_t relay;
    measurement_t snapshot;
    uint32_t last_measurement_update_ms;
} device_app_t;

void device_app_init(device_app_t *app);
void device_app_process(device_app_t *app, uint32_t tick_ms);
void device_app_on_bl0942_rx_byte(device_app_t *app, uint8_t data);
void device_app_on_rs485_rx_byte(device_app_t *app, uint8_t data);

#endif
