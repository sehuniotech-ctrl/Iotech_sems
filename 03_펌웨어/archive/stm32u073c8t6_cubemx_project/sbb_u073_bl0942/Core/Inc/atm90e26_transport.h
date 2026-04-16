#ifndef ATM90E26_TRANSPORT_H
#define ATM90E26_TRANSPORT_H

#include <stdbool.h>
#include <stdint.h>

typedef bool (*atm90e26_read_reg_fn)(uint16_t reg, uint16_t *value);
typedef bool (*atm90e26_write_reg_fn)(uint16_t reg, uint16_t value);

#endif
