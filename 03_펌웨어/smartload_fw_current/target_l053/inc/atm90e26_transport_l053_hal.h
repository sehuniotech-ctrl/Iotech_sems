#ifndef ATM90E26_TRANSPORT_L053_HAL_H
#define ATM90E26_TRANSPORT_L053_HAL_H

#include <stdbool.h>
#include <stdint.h>

/* STM32L053 HAL 기반 transport wrapper.
 * 실제 보드 도착 후 SPI/UART 연결이 확정되면 이 파일의 구현을 마무리한다.
 */

/* Prepare HAL handles and GPIO mapping used by the ATM90E26 transport. */
void atm90e26_transport_l053_init(void);

/* Read one ATM90E26 register through the final STM32L053 board interface. */
bool atm90e26_transport_l053_read_reg(uint16_t reg, uint16_t *value);

/* Write one ATM90E26 register through the final STM32L053 board interface. */
bool atm90e26_transport_l053_write_reg(uint16_t reg, uint16_t value);

/* Delay wrapper so common driver code can stay HAL-independent. */
void atm90e26_transport_l053_delay_ms(uint32_t delay_ms);

#endif
