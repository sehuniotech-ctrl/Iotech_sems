#include "atm90e26_transport_f107_lcu.h"

/* STM32F107 LCU board transport skeleton.
 * The common ATM90E26 driver and demo app are already prepared.
 * Once the LCU schematic is available, only this transport layer needs to be
 * connected to the real SPI/UART peripheral and GPIO pins.
 *
 * Recommended bring-up order:
 * 1. Confirm bus type: SPI or UART
 * 2. Bind peripheral handle and CS / RESET pins
 * 3. Implement SoftReset / CalStart / AdjStart writes
 * 4. Confirm SysStatus read
 * 5. Extend to URMS / IRMS / PMEAN / FREQ reads
 *
 * References:
 * - target_f107/F107_ATM90E26_BRINGUP_CHECKLIST.md
 * - inc/atm90e26_transport_f107_port_template.h
 */

void atm90e26_transport_f107_init(void)
{
    /* TODO:
     * - confirm whether ATM90E26 uses SPI or UART on the current LCU board
     * - bind the real STM32F107 peripheral handle
     * - map CS and RESET GPIOs if SPI is used
     * - bind the board delay function
     *
     * Minimum required inputs after schematic review:
     * - bus type
     * - peripheral instance
     * - CS pin
     * - RESET pin
     * - delay source
     */
}

bool atm90e26_transport_f107_read_reg(uint16_t reg, uint16_t *value)
{
    (void)reg;
    (void)value;

    /* TODO:
     * - implement one ATM90E26 register read transaction
     * - return true only when the bus transaction succeeds
     * - store the 16-bit register value in *value
     *
     * First bring-up target:
     * - SysStatus read
     * - URMS read
     * - IRMS read
     * - PMEAN read
     * - FREQ read
     *
     * SPI implementation checklist:
     * - assert CS
     * - send register read frame
     * - receive 16-bit response
     * - deassert CS
     *
     * UART implementation checklist:
     * - confirm ATM90E26 UART frame format
     * - send read request frame
     * - receive response frame
     * - validate response before returning true
     */
    return false;
}

bool atm90e26_transport_f107_write_reg(uint16_t reg, uint16_t value)
{
    (void)reg;
    (void)value;

    /* TODO:
     * - implement one ATM90E26 register write transaction
     * - demo mode startup depends on successful writes
     *
     * Minimum write targets for first bring-up:
     * - ATM90E26_REG_SOFT_RESET
     * - ATM90E26_REG_CAL_START
     * - ATM90E26_REG_ADJ_START
     */
    return false;
}

void atm90e26_transport_f107_delay_ms(uint32_t delay_ms)
{
    (void)delay_ms;

    /* TODO:
     * - bind to the blocking delay already available on the LCU board
     * - example: HAL_Delay(), RTOS delay, or existing system tick delay
     *
     * The ATM90E26 startup sequence needs real blocking delays.
     */
}
