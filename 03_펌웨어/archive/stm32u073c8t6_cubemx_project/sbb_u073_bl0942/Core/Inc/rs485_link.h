#ifndef RS485_LINK_H
#define RS485_LINK_H

#include "app_config.h"
#include "app_types.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    LINK_CMD_NONE = 0,
    LINK_CMD_POLL,
    LINK_CMD_RELAY_ON,
    LINK_CMD_RELAY_OFF,
    LINK_CMD_REMOTE_RECOVERY
} link_cmd_t;

typedef struct
{
    uint8_t rx_buf[APP_RS485_RX_BUF_SIZE];
    uint16_t rx_len;
    link_cmd_t pending_cmd;
} rs485_ctx_t;

void rs485_link_init(rs485_ctx_t *ctx);
void rs485_link_push_rx_byte(rs485_ctx_t *ctx, uint8_t data);
link_cmd_t rs485_link_take_cmd(rs485_ctx_t *ctx);
uint16_t rs485_link_build_poll_response(uint8_t *tx_buf,
                                        uint16_t tx_size,
                                        const measurement_t *measurement,
                                        const relay_state_t *relay_state);

#endif
