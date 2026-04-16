#include "atm90e26_transport_f107_lcu.h"

#include <string.h>

/* STM32F107 LCU board transport skeleton.
 * 회로도 전 단계에서 미리 해둘 수 있는 최대한의 뼈대 코드:
 * - SPI transfer callback 바인딩
 * - CS / RESET 제어 callback 바인딩
 * - delay callback 바인딩
 * - read/write 공통 흐름 정리
 *
 * 주의:
 * ATM90E26의 실제 SPI frame 포맷은 회로도/데이터시트 재확인 후 최종 확정해야 한다.
 * 그래서 현재 frame packing은 "임시 가정" 기반이며,
 * proto_assumption_enabled=true 일 때만 실제 transfer를 시도한다.
 */

static atm90e26_transport_f107_cfg_t g_f107_transport;

static bool atm90e26_transport_f107_ready_for_io(void)
{
    return g_f107_transport.bound &&
           (g_f107_transport.spi_transfer != 0) &&
           (g_f107_transport.cs_write != 0);
}

static void atm90e26_transport_f107_set_cs(bool asserted)
{
    bool physical_level = asserted;

    if (g_f107_transport.active_low_cs)
    {
        physical_level = !asserted;
    }

    g_f107_transport.cs_write(physical_level);
}

static void atm90e26_transport_f107_set_reset(bool asserted)
{
    bool physical_level = asserted;

    if ((g_f107_transport.reset_write == 0))
    {
        return;
    }

    if (g_f107_transport.active_low_reset)
    {
        physical_level = !asserted;
    }

    g_f107_transport.reset_write(physical_level);
}

static bool atm90e26_transport_f107_spi_xfer(const uint8_t *tx, uint8_t *rx, uint16_t len)
{
    if (!atm90e26_transport_f107_ready_for_io())
    {
        return false;
    }

    atm90e26_transport_f107_set_cs(true);
    const bool ok = g_f107_transport.spi_transfer(tx, rx, len);
    atm90e26_transport_f107_set_cs(false);
    return ok;
}

static void atm90e26_transport_f107_build_read_frame(uint16_t reg, uint8_t tx[4])
{
    /* TODO after final ATM90E26 SPI timing review:
     * The exact read opcode/address format must be confirmed against the
     * datasheet and the actual LCU schematic.
     *
     * Current placeholder assumption:
     * - 16-bit command/address field
     * - 16-bit dummy field to clock out response
     */
    tx[0] = (uint8_t)((reg >> 8) & 0x7FU);
    tx[1] = (uint8_t)(reg & 0xFFU);
    tx[2] = 0x00U;
    tx[3] = 0x00U;
}

static void atm90e26_transport_f107_build_write_frame(uint16_t reg, uint16_t value, uint8_t tx[4])
{
    /* TODO after final ATM90E26 SPI timing review:
     * The write bit/opcode position must be confirmed against the datasheet.
     *
     * Current placeholder assumption:
     * - MSB bit7 marks write
     * - remaining bits carry register address
     * - last 2 bytes carry value MSB first
     */
    tx[0] = (uint8_t)(((reg >> 8) & 0x7FU) | 0x80U);
    tx[1] = (uint8_t)(reg & 0xFFU);
    tx[2] = (uint8_t)(value >> 8);
    tx[3] = (uint8_t)(value & 0xFFU);
}

void atm90e26_transport_f107_init(void)
{
    memset(&g_f107_transport, 0, sizeof(g_f107_transport));

    /* 기본값:
     * - CS active low
     * - RESET active low로 가정
     * - 회로도 전까지는 proto_assumption_enabled를 false로 두어
     *   실수로 잘못된 frame을 실제 보드에 보내지 않게 한다.
     */
    g_f107_transport.active_low_cs = true;
    g_f107_transport.active_low_reset = true;
    g_f107_transport.proto_assumption_enabled = false;
    g_f107_transport.bound = false;
}

void atm90e26_transport_f107_bind(const atm90e26_transport_f107_cfg_t *cfg)
{
    if (cfg == 0)
    {
        return;
    }

    g_f107_transport = *cfg;
    g_f107_transport.bound = true;

    /* 유휴 상태에서 CS는 비활성으로 둔다. */
    if (g_f107_transport.cs_write != 0)
    {
        atm90e26_transport_f107_set_cs(false);
    }

    /* RESET도 비활성 상태로 둔다. */
    if (g_f107_transport.reset_write != 0)
    {
        atm90e26_transport_f107_set_reset(false);
    }
}

bool atm90e26_transport_f107_is_bound(void)
{
    return g_f107_transport.bound;
}

bool atm90e26_transport_f107_hw_reset(void)
{
    if ((g_f107_transport.reset_write == 0) || (g_f107_transport.delay_ms == 0))
    {
        return false;
    }

    /* 일반적인 active-low reset 시퀀스 기준 */
    atm90e26_transport_f107_set_reset(true);
    g_f107_transport.delay_ms(10U);
    atm90e26_transport_f107_set_reset(false);
    g_f107_transport.delay_ms(10U);
    return true;
}

bool atm90e26_transport_f107_read_reg(uint16_t reg, uint16_t *value)
{
    uint8_t tx[4];
    uint8_t rx[4] = {0U, 0U, 0U, 0U};

    if (value == 0)
    {
        return false;
    }

    if (!g_f107_transport.proto_assumption_enabled)
    {
        /* 회로도/데이터시트 기준 frame format이 확정되기 전에는
         * 잘못된 bus transaction을 막기 위해 false를 반환한다.
         */
        return false;
    }

    atm90e26_transport_f107_build_read_frame(reg, tx);
    if (!atm90e26_transport_f107_spi_xfer(tx, rx, sizeof(tx)))
    {
        return false;
    }

    /* 현재는 마지막 2바이트를 register value로 가정 */
    *value = ((uint16_t)rx[2] << 8) | (uint16_t)rx[3];
    return true;
}

bool atm90e26_transport_f107_write_reg(uint16_t reg, uint16_t value)
{
    uint8_t tx[4];
    uint8_t rx[4] = {0U, 0U, 0U, 0U};

    if (!g_f107_transport.proto_assumption_enabled)
    {
        return false;
    }

    atm90e26_transport_f107_build_write_frame(reg, value, tx);
    return atm90e26_transport_f107_spi_xfer(tx, rx, sizeof(tx));
}

void atm90e26_transport_f107_delay_ms(uint32_t delay_ms)
{
    if (g_f107_transport.delay_ms != 0)
    {
        g_f107_transport.delay_ms(delay_ms);
    }
}
