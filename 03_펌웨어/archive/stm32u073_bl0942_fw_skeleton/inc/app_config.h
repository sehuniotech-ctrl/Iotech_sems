#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#define APP_BL0942_RX_BUF_SIZE            64U
#define APP_RS485_RX_BUF_SIZE             128U

#define APP_OVERCURRENT_LIMIT_A           16.0f
#define APP_STANDBY_POWER_LIMIT_W         3.0f
#define APP_STANDBY_DELAY_MS              (30UL * 60UL * 1000UL)
#define APP_RECOVERY_RETRY_LIMIT          2U

#define APP_POLL_RESPONSE_TIMEOUT_MS      1000UL
#define APP_MEASUREMENT_UPDATE_MS         1000UL

#endif
