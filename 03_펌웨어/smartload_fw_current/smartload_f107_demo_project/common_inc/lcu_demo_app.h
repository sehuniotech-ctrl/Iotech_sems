#ifndef LCU_DEMO_APP_H
#define LCU_DEMO_APP_H

#include "app_types.h"
#include "atm90e26.h"
#include <stdbool.h>
#include <stdint.h>

/* Demo application context for the current LCU-board-first bring-up stage.
 * It wraps the metering driver and adds just enough state for protection and
 * later Smart Load control flow verification.
 */
typedef struct
{
    atm90e26_ctx_t meter;
    measurement_t snapshot;
    relay_state_t relay;
    uint16_t last_sys_status;
    uint32_t last_update_ms;
    bool meter_ready;
} lcu_demo_app_t;

/* Initialize the demo app and bind the selected board transport. */
void lcu_demo_app_init(lcu_demo_app_t *app,
                       atm90e26_read_reg_fn read_reg,
                       atm90e26_write_reg_fn write_reg,
                       atm90e26_delay_ms_fn delay_ms);

/* Start the ATM90E26 demo measurement flow. */
bool lcu_demo_app_start_meter(lcu_demo_app_t *app);

/* Periodically update meter values and run simple protection/demo logic. */
bool lcu_demo_app_poll(lcu_demo_app_t *app, uint32_t tick_ms);

#endif
