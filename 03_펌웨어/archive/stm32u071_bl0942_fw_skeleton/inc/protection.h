#ifndef PROTECTION_H
#define PROTECTION_H

#include "app_types.h"
#include <stdbool.h>

bool protection_check_overcurrent(const measurement_t *measurement);

#endif
