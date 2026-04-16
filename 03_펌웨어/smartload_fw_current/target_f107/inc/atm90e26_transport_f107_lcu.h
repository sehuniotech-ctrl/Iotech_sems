#ifndef ATM90E26_TRANSPORT_F107_LCU_H
#define ATM90E26_TRANSPORT_F107_LCU_H

#include <stdbool.h>
#include <stdint.h>

/* STM32F107 + LCU board transport wrapper.
 * 목표:
 * - 공통 ATM90E26 드라이버는 그대로 두고
 * - F107 보드의 SPI / GPIO / delay만 이 레이어에 연결
 *
 * 현재는 회로도 전 단계이므로 HAL 의존성을 직접 넣지 않고,
 * 아래 callback 바인딩 방식으로 뼈대를 먼저 준비한다.
 */

typedef bool (*atm90e26_f107_spi_transfer_fn)(const uint8_t *tx,
                                              uint8_t *rx,
                                              uint16_t len);
typedef void (*atm90e26_f107_gpio_write_fn)(bool level);
typedef void (*atm90e26_f107_delay_hook_fn)(uint32_t delay_ms);

typedef struct
{
    atm90e26_f107_spi_transfer_fn spi_transfer;
    atm90e26_f107_gpio_write_fn cs_write;
    atm90e26_f107_gpio_write_fn reset_write;
    atm90e26_f107_delay_hook_fn delay_ms;
    bool active_low_cs;
    bool active_low_reset;
    bool proto_assumption_enabled;
    bool bound;
} atm90e26_transport_f107_cfg_t;

/* 기본 상태 초기화 */
void atm90e26_transport_f107_init(void);

/* 회로도 확인 후 실제 SPI/GPIO/HAL 함수를 바인딩 */
void atm90e26_transport_f107_bind(const atm90e26_transport_f107_cfg_t *cfg);

/* 현재 transport가 실제 하드웨어 함수와 연결되었는지 확인 */
bool atm90e26_transport_f107_is_bound(void);

/* 선택: 하드웨어 reset 시퀀스 */
bool atm90e26_transport_f107_hw_reset(void);

/* 공통 ATM90E26 드라이버가 호출하는 함수 */
bool atm90e26_transport_f107_read_reg(uint16_t reg, uint16_t *value);
bool atm90e26_transport_f107_write_reg(uint16_t reg, uint16_t value);
void atm90e26_transport_f107_delay_ms(uint32_t delay_ms);

#endif
