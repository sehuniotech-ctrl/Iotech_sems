#ifndef ATM90E26_TRANSPORT_F107_LCU_H
#define ATM90E26_TRANSPORT_F107_LCU_H

#include <stdbool.h>
#include <stdint.h>

/* STM32F107 + LCU board transport wrapper.
 * The actual bus type on the LCU board must be confirmed first. After that,
 * only these functions need to be connected to the board SPI/UART and GPIOs.
 * 즉, 보드 수령 후 가장 먼저 구현할 파일이 이 파일이다.
 */
void atm90e26_transport_f107_init(void);
bool atm90e26_transport_f107_read_reg(uint16_t reg, uint16_t *value);
bool atm90e26_transport_f107_write_reg(uint16_t reg, uint16_t value);
void atm90e26_transport_f107_delay_ms(uint32_t delay_ms);

#endif
