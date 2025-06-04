/**
 * @file    eps.c
 * @author  Matteo Planchet
 * @brief   Source file for EPS
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "kernel.h"
#include "eps.h"

/***************************** Macros Definitions ****************************/

#define CSP_PRIO_MASK              0x3
#define CSP_PRIO_OFFSET            46
#define CSP_DST_MASK               0x3FFF
#define CSP_DST_OFFSET             32
#define CSP_SRC_MASK               0x3FFF
#define CSP_SRC_OFFSET             18
#define CSP_DPORT_MASK             0x3F
#define CSP_DPORT_OFFSET           12
#define CSP_SPORT_MASK             0x3F
#define CSP_SPORT_OFFSET           6
#define CSP_FLAGS_MASK             0x3F
#define CSP_FLAGS_OFFSET           0

#define CSP_EPS_ADDRESS            2

#define CSP_EPS_PORT_HK            8
#define CSP_EPS_PORT_OUTPUT        9
#define CSP_EPS_PORT_SINGLE_OUTPUT 10
#define CSP_EPS_PORT_VOLT          11
#define CSP_EPS_PORT_AUTO          12
#define CSP_EPS_PORT_HEATER        13
#define CSP_EPS_PORT_RESET_COUNTER 15
#define CSP_EPS_PORT_RESET_WDT     16

#define CSP_OBC_ADDRESS            0xACAB

#define CSP_OBC_PORT               0

#define CSP_BASE_HEADER                                                                                                         \
    ((uint64_t)CSP_EPS_ADDRESS & CSP_DST_MASK) << CSP_DST_OFFSET | ((uint64_t)CSP_OBC_ADDRESS & CSP_SRC_MASK) << CSP_SRC_OFFSET \
        | ((uint64_t)CSP_OBC_PORT & CSP_SPORT_MASK) << CSP_SPORT_OFFSET

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

static returnCode_t GetHk1(epsInst_t *power_inst, hkparam_t *hk)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    if (power_inst != NULL && hk != NULL)
    {
        uint64_t csp_packet_header = CSP_BASE_HEADER;

        // Set destination port
        csp_packet_header |= ((uint64_t)CSP_EPS_PORT_HK & CSP_DPORT_MASK) << CSP_DPORT_OFFSET;

        return_value = DeviceWrite(power_inst->dev_i2c, &csp_packet_header, sizeof(csp_packet_header));

        if (return_value == RET_SUCCESSFUL)
        {
            return_value = DeviceRead(power_inst->dev_i2c, hk, sizeof(hkparam_t));
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
