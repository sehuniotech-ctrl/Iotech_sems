#ifndef ATM90E26_H
#define ATM90E26_H

#include "app_types.h"
#include "atm90e26_transport.h"
#include <stdbool.h>
#include <stdint.h>

/* ATM90E26 driver context.
 * The driver itself does not depend on a specific MCU or HAL. Instead, board-
 * specific transport callbacks are injected here so the same code can run on
 * the LCU board now and the STM32L053 board later.
 */
typedef struct
{
    atm90e26_read_reg_fn read_reg;
    atm90e26_write_reg_fn write_reg;
    atm90e26_delay_ms_fn delay_ms;
    measurement_t measurement;
    bool initialized;
} atm90e26_ctx_t;

/* Bind the board-specific transport functions and clear cached measurements. */
void atm90e26_init(atm90e26_ctx_t *ctx,
                   atm90e26_read_reg_fn read_reg,
                   atm90e26_write_reg_fn write_reg,
                   atm90e26_delay_ms_fn delay_ms);

/* Put the chip into a known state before starting measurement. */
bool atm90e26_soft_reset(atm90e26_ctx_t *ctx);

/* Start the chip in demo mode so bring-up can proceed before full calibration
 * data and checksum flow are finalized.
 */
bool atm90e26_startup_demo_mode(atm90e26_ctx_t *ctx);

/* Read the core measurement set used by Smart Load demo and control logic. */
bool atm90e26_read_all_measurements(atm90e26_ctx_t *ctx);

/* Read status bits such as checksum error or voltage sag warning. */
bool atm90e26_read_sys_status(atm90e26_ctx_t *ctx, uint16_t *status);

/* Return the last cached measurement snapshot. */
const measurement_t *atm90e26_get_measurement(const atm90e26_ctx_t *ctx);

#endif
