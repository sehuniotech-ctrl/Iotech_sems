#ifndef ATM90E26_REGS_H
#define ATM90E26_REGS_H

#include <stdint.h>

/* Only the registers needed for bring-up and Smart Load 1st-step demo are
 * listed here. This keeps the driver focused on early verification first.
 */
#define ATM90E26_REG_SOFT_RESET           ((uint16_t)0x00U)
#define ATM90E26_REG_SYS_STATUS           ((uint16_t)0x01U)
#define ATM90E26_REG_FUNC_EN              ((uint16_t)0x02U)
#define ATM90E26_REG_SAG_TH               ((uint16_t)0x03U)
#define ATM90E26_REG_LAST_DATA            ((uint16_t)0x06U)
#define ATM90E26_REG_LSB                  ((uint16_t)0x08U)

#define ATM90E26_REG_CAL_START            ((uint16_t)0x20U)
#define ATM90E26_REG_ADJ_START            ((uint16_t)0x30U)

#define ATM90E26_REG_AP_ENERGY            ((uint16_t)0x40U)
#define ATM90E26_REG_RP_ENERGY            ((uint16_t)0x43U)

#define ATM90E26_REG_IRMS                 ((uint16_t)0x48U)
#define ATM90E26_REG_URMS                 ((uint16_t)0x49U)
#define ATM90E26_REG_PMEAN                ((uint16_t)0x4AU)
#define ATM90E26_REG_QMEAN                ((uint16_t)0x4BU)
#define ATM90E26_REG_FREQ                 ((uint16_t)0x4CU)
#define ATM90E26_REG_POWERF               ((uint16_t)0x4DU)
#define ATM90E26_REG_PANGLE               ((uint16_t)0x4EU)
#define ATM90E26_REG_SMEAN                ((uint16_t)0x4FU)

#define ATM90E26_REG_IRMS2                ((uint16_t)0x68U)
#define ATM90E26_REG_PMEAN2               ((uint16_t)0x6AU)
#define ATM90E26_REG_QMEAN2               ((uint16_t)0x6BU)
#define ATM90E26_REG_POWERF2              ((uint16_t)0x6DU)
#define ATM90E26_REG_PANGLE2              ((uint16_t)0x6EU)
#define ATM90E26_REG_SMEAN2               ((uint16_t)0x6FU)

/* Special command values taken from the datasheet. */
#define ATM90E26_SOFT_RESET_KEY           ((uint16_t)0x789AU)
#define ATM90E26_STARTUP_BYPASS_KEY       ((uint16_t)0x5678U)
#define ATM90E26_STARTUP_CHECKSUM_KEY     ((uint16_t)0x8765U)

#endif
