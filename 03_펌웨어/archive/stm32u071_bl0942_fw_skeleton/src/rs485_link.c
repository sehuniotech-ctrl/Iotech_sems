#include "app_config.h"
#include "rs485_link.h"

void rs485_link_init(rs485_ctx_t *ctx)
{
    ctx->rx_len = 0U;
    ctx->pending_cmd = LINK_CMD_NONE;
}

void rs485_link_push_rx_byte(rs485_ctx_t *ctx, uint8_t data)
{
    if (ctx->rx_len < APP_RS485_RX_BUF_SIZE)
    {
        ctx->rx_buf[ctx->rx_len++] = data;
    }

    /* TODO: Replace this stub with real protocol parsing. */
    if (data == 'P')
    {
        ctx->pending_cmd = LINK_CMD_POLL;
    }
    else if (data == '1')
    {
        ctx->pending_cmd = LINK_CMD_RELAY_ON;
    }
    else if (data == '0')
    {
        ctx->pending_cmd = LINK_CMD_RELAY_OFF;
    }
    else if (data == 'R')
    {
        ctx->pending_cmd = LINK_CMD_REMOTE_RECOVERY;
    }
}

link_cmd_t rs485_link_take_cmd(rs485_ctx_t *ctx)
{
    link_cmd_t cmd = ctx->pending_cmd;
    ctx->pending_cmd = LINK_CMD_NONE;
    ctx->rx_len = 0U;
    return cmd;
}

uint16_t rs485_link_build_poll_response(uint8_t *tx_buf,
                                        uint16_t tx_size,
                                        const measurement_t *measurement,
                                        const relay_state_t *relay_state)
{
    (void)measurement;
    (void)relay_state;

    if (tx_size < 8U)
    {
        return 0U;
    }

    /* TODO: Fill with actual protocol frame. */
    tx_buf[0] = 'O';
    tx_buf[1] = 'K';
    tx_buf[2] = ',';
    tx_buf[3] = relay_state->relay_on ? '1' : '0';
    tx_buf[4] = '\r';
    tx_buf[5] = '\n';
    return 6U;
}
