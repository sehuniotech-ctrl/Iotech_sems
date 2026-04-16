#ifndef BL0942_H
#define BL0942_H

#include "app_config.h"
#include "app_types.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t rx_buf[APP_BL0942_RX_BUF_SIZE];
    uint16_t rx_len;
    measurement_t measurement;
    bool frame_ready;
} bl0942_ctx_t;

void bl0942_init(bl0942_ctx_t *ctx);
void bl0942_push_rx_byte(bl0942_ctx_t *ctx, uint8_t data);
bool bl0942_process_frame(bl0942_ctx_t *ctx);
const measurement_t *bl0942_get_measurement(const bl0942_ctx_t *ctx);

#endif
