#include "atm90e26_regs.h"
#include "lcu_demo_app.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
    bool soft_reset_done;
    bool demo_mode_started;
    uint32_t read_cycle;
} host_demo_meter_state_t;

static host_demo_meter_state_t g_host_demo_meter;

static void host_demo_print_snapshot(const lcu_demo_app_t *app, uint32_t tick_ms)
{
    printf("[tick=%lu ms] V=%.2fV I=%.3fA P=%.3fW Q=%.3fvar S=%.3fVA PF=%.3f F=%.2fHz Relay=%s Trip=%d\n",
           (unsigned long)tick_ms,
           app->snapshot.voltage_v,
           app->snapshot.line_current_a,
           app->snapshot.active_power_w,
           app->snapshot.reactive_power_var,
           app->snapshot.apparent_power_va,
           app->snapshot.power_factor,
           app->snapshot.frequency_hz,
           app->relay.relay_on ? "ON" : "OFF",
           (int)app->relay.last_trip_reason);
}

static bool host_demo_read_reg(uint16_t reg, uint16_t *value)
{
    if (value == 0)
    {
        return false;
    }

    if (!g_host_demo_meter.demo_mode_started &&
        (reg != ATM90E26_REG_SYS_STATUS))
    {
        return false;
    }

    switch (reg)
    {
    case ATM90E26_REG_SYS_STATUS:
        *value = g_host_demo_meter.demo_mode_started ? 0x0001U : 0x0000U;
        return true;

    case ATM90E26_REG_URMS:
        *value = 22340U; /* 223.40V */
        return true;

    case ATM90E26_REG_IRMS:
        if (g_host_demo_meter.read_cycle < 2U)
        {
            *value = 850U; /* 0.850A */
        }
        else if (g_host_demo_meter.read_cycle < 4U)
        {
            *value = 12U; /* standby-like low current */
        }
        else
        {
            *value = 18250U; /* 18.250A -> overcurrent demo */
        }
        return true;

    case ATM90E26_REG_IRMS2:
        *value = 5U; /* 0.005A */
        return true;

    case ATM90E26_REG_PMEAN:
        if (g_host_demo_meter.read_cycle < 2U)
        {
            *value = 1800U; /* 1.800W */
        }
        else if (g_host_demo_meter.read_cycle < 4U)
        {
            *value = 2U; /* 0.002W -> standby threshold */
        }
        else
        {
            *value = 5500U; /* 5.500W */
        }
        return true;

    case ATM90E26_REG_QMEAN:
        *value = 320U; /* 0.320var */
        return true;

    case ATM90E26_REG_SMEAN:
        *value = 2100U; /* 2.100VA */
        return true;

    case ATM90E26_REG_POWERF:
        *value = 860U; /* 0.860 */
        return true;

    case ATM90E26_REG_FREQ:
        *value = 6000U; /* 60.00Hz */
        return true;

    case ATM90E26_REG_PANGLE:
        *value = 35U; /* 3.5deg */
        return true;

    case ATM90E26_REG_AP_ENERGY:
        *value = 1U;
        return true;

    case ATM90E26_REG_RP_ENERGY:
        *value = 1U;
        g_host_demo_meter.read_cycle++;
        return true;

    default:
        *value = 0U;
        return true;
    }
}

static bool host_demo_write_reg(uint16_t reg, uint16_t value)
{
    printf("[write] reg=0x%04X value=0x%04X\n", reg, value);

    if ((reg == ATM90E26_REG_SOFT_RESET) && (value == ATM90E26_SOFT_RESET_KEY))
    {
        g_host_demo_meter.soft_reset_done = true;
        g_host_demo_meter.demo_mode_started = false;
        g_host_demo_meter.read_cycle = 0U;
        return true;
    }

    if ((reg == ATM90E26_REG_CAL_START) && (value == ATM90E26_STARTUP_BYPASS_KEY))
    {
        return g_host_demo_meter.soft_reset_done;
    }

    if ((reg == ATM90E26_REG_ADJ_START) && (value == ATM90E26_STARTUP_BYPASS_KEY))
    {
        if (g_host_demo_meter.soft_reset_done)
        {
            g_host_demo_meter.demo_mode_started = true;
            return true;
        }
    }

    return true;
}

static void host_demo_delay_ms(uint32_t delay_ms)
{
    printf("[delay] %lu ms\n", (unsigned long)delay_ms);
}

int main(void)
{
    lcu_demo_app_t app;
    uint32_t tick_ms = 0U;
    uint32_t step = 0U;

    printf("=== Smart Load host demo start ===\n");

    lcu_demo_app_init(&app,
                      host_demo_read_reg,
                      host_demo_write_reg,
                      host_demo_delay_ms);

    if (!lcu_demo_app_start_meter(&app))
    {
        printf("ATM90E26 startup failed\n");
        return 1;
    }

    printf("ATM90E26 startup success\n");

    for (step = 0U; step < 6U; ++step)
    {
        tick_ms += 1000U;

        if (!lcu_demo_app_poll(&app, tick_ms))
        {
            printf("poll failed at step %lu\n", (unsigned long)step);
            return 1;
        }

        host_demo_print_snapshot(&app, tick_ms);
    }

    printf("=== Smart Load host demo end ===\n");
    return 0;
}
