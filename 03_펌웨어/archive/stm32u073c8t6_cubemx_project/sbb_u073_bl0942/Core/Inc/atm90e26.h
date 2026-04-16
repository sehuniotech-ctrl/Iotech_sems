#ifndef ATM90E26_H
#define ATM90E26_H

#include "app_types.h"
#include "atm90e26_transport.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    atm90e26_read_reg_fn read_reg;
    atm90e26_write_reg_fn write_reg;
    measurement_t measurement;
    bool initialized;
} atm90e26_ctx_t;

void atm90e26_init(atm90e26_ctx_t *ctx,
                   atm90e26_read_reg_fn read_reg,
                   atm90e26_write_reg_fn write_reg);
bool atm90e26_configure(atm90e26_ctx_t *ctx);
bool atm90e26_read_measurement(atm90e26_ctx_t *ctx);
const measurement_t *atm90e26_get_measurement(const atm90e26_ctx_t *ctx);

#endif
