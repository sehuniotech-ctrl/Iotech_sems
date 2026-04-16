#include "atm90e26_transport_l053_hal.h"

/* This file is intentionally left as a HAL skeleton.
 * Once the STM32L053 board and CubeMX project are ready, replace these stubs
 * with SPI/UART transactions that match the real ATM90E26 wiring.
 */

void atm90e26_transport_l053_init(void)
{
    /* TODO:
     * - bind SPI1 or UART handle created by CubeMX
     * - configure CS / RESET GPIO handles
     * - decide whether the final board uses SPI or UART path
     */
}

bool atm90e26_transport_l053_read_reg(uint16_t reg, uint16_t *value)
{
    (void)reg;
    (void)value;

    /* TODO:
     * - implement ATM90E26 register read sequence with HAL SPI/UART API
     * - return true only when transaction succeeds and value is valid
     * - if SPI is used, handle CS assert/deassert and frame format here
     */
    return false;
}

bool atm90e26_transport_l053_write_reg(uint16_t reg, uint16_t value)
{
    (void)reg;
    (void)value;

    /* TODO:
     * - implement ATM90E26 register write sequence with HAL SPI/UART API
     * - reuse the same register addressing rule as the demo board path
     */
    return false;
}

void atm90e26_transport_l053_delay_ms(uint32_t delay_ms)
{
    (void)delay_ms;

    /* TODO:
     * - replace with HAL_Delay(delay_ms) after CubeMX project is generated
     */
}
