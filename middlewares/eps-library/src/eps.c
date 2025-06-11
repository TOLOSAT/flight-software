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
#include <string.h>

/***************************** Macros Definitions ****************************/

#define CSP_PACKET_HEADER_SIZE     48
#define CSP_PACKET_BODY_MAX_SIZE   128
#define CSP_PACKET_MAX_SIZE        (CSP_PACKET_HEADER_SIZE + CSP_PACKET_BODY_MAX_SIZE)

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

static returnCode_t SendGenericPacket(epsInst_t *power_inst, void* input, size_t input_size, void *output, size_t output_size, uint64_t csp_dest_port)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    size_t current_packet_size = CSP_PACKET_HEADER_SIZE + input_size;

    // Check params
    if ((power_inst != NULL) && (current_packet_size <= CSP_PACKET_MAX_SIZE))
    {
        uint64_t csp_packet_header = CSP_BASE_HEADER;
        csp_packet_header |= (csp_dest_port & CSP_DPORT_MASK) << CSP_DPORT_OFFSET;

        uint8_t csp_packet[CSP_PACKET_BODY_MAX_SIZE];

        // Copy the header and trim to only the size of the packet header
        memcpy(csp_packet, &csp_packet_header, CSP_PACKET_HEADER_SIZE);

        // Copy the payload
        memcpy(csp_packet + CSP_PACKET_HEADER_SIZE, input, input_size);

        return_value = DeviceWrite(power_inst->dev_i2c, &csp_packet, current_packet_size);

        if (return_value == RET_SUCCESSFUL)
        {
            return_value = DeviceRead(power_inst->dev_i2c, output, output_size);
        }
    } else {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

static returnCode_t GetHk1(epsInst_t *power_inst, hkparam_t *hk)
{
    return SendGenericPacket(power_inst, NULL, 0, hk, sizeof(hkparam_t), (uint64_t)CSP_EPS_PORT_HK);
}

static returnCode_t GetHk2(epsInst_t *power_inst, eps_hk_t *hk)
{
    return SendGenericPacket(power_inst, (uint8_t)0, 1, hk, sizeof(eps_hk_t), (uint64_t)CSP_EPS_PORT_HK);
}

static returnCode_t GetHk2Vi(epsInst_t *power_inst, eps_hk_vi_t *hk)
{
    return SendGenericPacket(power_inst, (uint8_t)1, 1, hk, sizeof(eps_hk_vi_t), (uint64_t)CSP_EPS_PORT_HK);
}

static returnCode_t GetHk2Wdt(epsInst_t *power_inst, eps_hk_wdt_t *hk)
{
    return SendGenericPacket(power_inst, (uint8_t)2, 1, hk, sizeof(eps_hk_wdt_t), (uint64_t)CSP_EPS_PORT_HK);
}

static returnCode_t GetHk2Basic(epsInst_t *power_inst, eps_hk_basic_t *hk)
{
    return SendGenericPacket(power_inst, (uint8_t)3, 1, hk, sizeof(eps_hk_basic_t), (uint64_t)CSP_EPS_PORT_HK);
}
