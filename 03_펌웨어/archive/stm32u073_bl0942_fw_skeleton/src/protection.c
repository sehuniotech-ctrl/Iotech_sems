#include "app_config.h"
#include "protection.h"

bool protection_check_overcurrent(const measurement_t *measurement)
{
    return (measurement->fast_current_rms_a >= APP_OVERCURRENT_LIMIT_A);
}
