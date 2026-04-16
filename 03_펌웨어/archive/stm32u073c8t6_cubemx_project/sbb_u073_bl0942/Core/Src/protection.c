#include "app_config.h"
#include "protection.h"

bool protection_check_overcurrent(const measurement_t *measurement)
{
    return (measurement->line_current_a >= APP_OVERCURRENT_LIMIT_A);
}

bool protection_check_overvoltage(const measurement_t *measurement)
{
    return (measurement->voltage_v >= 260.0f);
}
