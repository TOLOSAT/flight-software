/**
 * @file    eps.c
 * @author  Matteo Planchet
 * @brief   Source file for EPS
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "kernel.h"
#include "eps_driver.h"
#include <string.h>

/***************************** Macros Definitions ****************************/

#define CSP_PACKET_HEADER_SIZE     6                                                   /**< @brief Size of the CSP packet header in bytes (48 bits) */
#define CSP_PACKET_BODY_MAX_SIZE   128                                                 /**< @brief Size of the CSP packet body in bytes */
#define CSP_PACKET_MAX_SIZE        (CSP_PACKET_HEADER_SIZE + CSP_PACKET_BODY_MAX_SIZE) /**< @brief Maximum size of the CSP packet in bytes */

#define CSP_PRIO_MASK              0x3                                /**< @brief Mask for the CSP priority */
#define CSP_PRIO_OFFSET            46                                 /**< @brief Offset for the CSP priority in the header */
#define CSP_14BIT_MASK             0x3FFF                             /**< @brief Mask for the CSP destination address */
#define CSP_DST_OFFSET             32                                 /**< @brief Offset for the CSP destination address in the header */
#define CSP_DST_MASK               (CSP_14BIT_MASK << CSP_DST_OFFSET) /**< @brief Mask for the CSP destination address */
#define CSP_SRC_OFFSET             18 /**< @brief Offset for the CSP source address in the header - should be : 0x00003FFF00000000*/
#define CSP_SRC_MASK               (CSP_14BIT_MASK << CSP_SRC_OFFSET) /**< @brief Mask for the CSP source address  - should be : 0x00000000FFFC0000*/
#define CSP_DPORT_MASK             0x3F                               /**< @brief Mask for the CSP destination port */
#define CSP_DPORT_OFFSET           12                                 /**< @brief Offset for the CSP destination port in the header */
#define CSP_SPORT_MASK             0x3F                               /**< @brief Mask for the CSP source port */
#define CSP_SPORT_OFFSET           6                                  /**< @brief Offset for the CSP source port in the header */
#define CSP_FLAGS_MASK             0x3F                               /**< @brief Mask for the CSP flags */
#define CSP_FLAGS_OFFSET           0                                  /**< @brief Offset for the CSP flags in the header */

#define CSP_EPS_ADDRESS            2 /**< @brief CSP Address for the EPS */

#define CSP_EPS_PORT_HK            8  /**< @brief CSP Port for the EPS housekeeping data */
#define CSP_EPS_PORT_OUTPUT        9  /**< @brief CSP Port for the EPS output data */
#define CSP_EPS_PORT_SINGLE_OUTPUT 10 /**< @brief CSP Port for the EPS single output data */
#define CSP_EPS_PORT_VOLT          11 /**< @brief CSP Port for the EPS voltage data */
#define CSP_EPS_PORT_AUTO          12 /**< @brief CSP Port for the EPS auto data */
#define CSP_EPS_PORT_HEATER        13 /**< @brief CSP Port for the EPS heater data */
#define CSP_EPS_PORT_RESET_COUNTER 15 /**< @brief CSP Port for the EPS reset counter data */
#define CSP_EPS_PORT_RESET_WDT     16 /**< @brief CSP Port for the EPS reset watchdog timer data */
#define CSP_EPS_PORT_CONFIG_CMD    17 /**< @brief CSP Port for the EPS configuration command data */
#define CSP_EPS_PORT_CONFIG_GET    18 /**< @brief CSP Port for the EPS configuration get data */
#define CSP_EPS_PORT_CONFIG_SET    19 /**< @brief CSP Port for the EPS configuration set data */
#define CSP_EPS_PORT_HARD_RESET    20 /**< @brief CSP Port for the EPS hard reset data */
#define CSP_EPS_PORT_CONFIG2_CMD   21 /**< @brief CSP Port for the EPS configuration command data (2nd version) */
#define CSP_EPS_PORT_CONFIG2_GET   22 /**< @brief CSP Port for the EPS configuration get data (2nd version) */
#define CSP_EPS_PORT_CONFIG2_SET   23 /**< @brief CSP Port for the EPS configuration set data (2nd version) */

#define CSP_OBC_ADDRESS            0xACAB /**< @brief CSP Address for the OBC */

#define CSP_OBC_PORT               0 /**< @brief CSP Port for the OBC */

#define CSP_BASE_HEADER                                                                                                                           \
    ((uint64_t)CSP_EPS_ADDRESS & CSP_DST_MASK) << CSP_DST_OFFSET | ((uint64_t)CSP_OBC_ADDRESS & CSP_SRC_MASK) << CSP_SRC_OFFSET                   \
        | ((uint64_t)CSP_OBC_PORT & CSP_SPORT_MASK) << CSP_SPORT_OFFSET /**< @brief Base header for the CSP packet, with OBC as source and EPS as \
                                                                           destination */

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

static returnCode_t SendGenericPacket(epsInst_t *power_inst, uint8_t *input, size_t input_size, void *output, size_t output_size,
                                      uint64_t csp_dest_port)
{
    returnCode_t return_value  = RET_SUCCESSFUL;
    size_t current_packet_size = CSP_PACKET_HEADER_SIZE + input_size;

    // Check params
    if ((power_inst != NULL) && (current_packet_size <= CSP_PACKET_MAX_SIZE))
    {
        uint64_t csp_packet_header  = CSP_BASE_HEADER;
        csp_packet_header          |= (csp_dest_port & CSP_DPORT_MASK) << CSP_DPORT_OFFSET;

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
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

// All commands are from the data sheet

static returnCode_t GetHk1(epsInst_t *power_inst, hkparam_t *hk)
{
    return SendGenericPacket(power_inst, NULL, 0, hk, sizeof(hkparam_t), (uint64_t)CSP_EPS_PORT_HK);
}

static returnCode_t GetHk2(epsInst_t *power_inst, eps_hk_t *hk)
{
    uint8_t mode = 0;
    return SendGenericPacket(power_inst, &mode, sizeof(mode), hk, sizeof(eps_hk_t), (uint64_t)CSP_EPS_PORT_HK);
}

static returnCode_t GetHk2Vi(epsInst_t *power_inst, eps_hk_vi_t *hk)
{
    uint8_t mode = 1;
    return SendGenericPacket(power_inst, &mode, sizeof(mode), hk, sizeof(eps_hk_vi_t), (uint64_t)CSP_EPS_PORT_HK);
}

static returnCode_t GetHk2Wdt(epsInst_t *power_inst, eps_hk_wdt_t *hk)
{
    uint8_t mode = 2;
    return SendGenericPacket(power_inst, &mode, sizeof(mode), hk, sizeof(eps_hk_wdt_t), (uint64_t)CSP_EPS_PORT_HK);
}

static returnCode_t GetHk2Basic(epsInst_t *power_inst, eps_hk_basic_t *hk)
{
    uint8_t mode = 3;
    return SendGenericPacket(power_inst, &mode, sizeof(mode), hk, sizeof(eps_hk_basic_t), (uint64_t)CSP_EPS_PORT_HK);
}

static returnCode_t SetOutput(epsInst_t *power_inst, uint8_t output_byte)
{
    return SendGenericPacket(power_inst, &output_byte, sizeof(output_byte), NULL, 0, (uint64_t)CSP_EPS_PORT_OUTPUT);
}

static returnCode_t SetSingleOutput(epsInst_t *power_inst, uint8_t channel, uint8_t value, uint16_t delay)
{
    uint8_t params[4] = { channel, value, (uint8_t)(delay >> 8), (uint8_t)(delay & 0xFF) };
    return SendGenericPacket(power_inst, params, sizeof(params), NULL, 0, (uint64_t)CSP_EPS_PORT_SINGLE_OUTPUT);
}

static returnCode_t SetPVVolt(epsInst_t *power_inst, uint16_t voltage1, uint16_t voltage2, uint16_t voltage3)
{
    uint8_t voltages[6] = { (uint8_t)(voltage1 >> 8),   (uint8_t)(voltage1 & 0xFF), (uint8_t)(voltage2 >> 8),
                            (uint8_t)(voltage2 & 0xFF), (uint8_t)(voltage3 >> 8),   (uint8_t)(voltage3 & 0xFF) };
    return SendGenericPacket(power_inst, voltages, sizeof(voltages), NULL, 0, (uint64_t)CSP_EPS_PORT_VOLT);
}

static returnCode_t SetHeaterAuto(epsInst_t *power_inst, uint8_t mode, uint8_t *returned_mode)
{
    return SendGenericPacket(power_inst, &mode, sizeof(uint8_t), returned_mode, sizeof(uint8_t), (uint64_t)CSP_EPS_PORT_AUTO);
}

static returnCode_t ResetCounters(epsInst_t *power_inst)
{
    uint8_t magic = 0x42;
    return SendGenericPacket(power_inst, &magic, sizeof(uint8_t), NULL, 0, (uint64_t)CSP_EPS_PORT_RESET_COUNTER);
}

static returnCode_t ResetWDT(epsInst_t *power_inst)
{
    uint8_t magic = 0x78;
    return SendGenericPacket(power_inst, &magic, sizeof(uint8_t), NULL, 0, (uint64_t)CSP_EPS_PORT_RESET_WDT);
}

static returnCode_t ConfigCmd(epsInst_t *power_inst, uint8_t cmd)
{
    return SendGenericPacket(power_inst, &cmd, sizeof(uint8_t), NULL, 0, (uint64_t)CSP_EPS_PORT_CONFIG_CMD);
}

static returnCode_t ConfigGet(epsInst_t *power_inst, eps_config_t *eps_config)
{
    return SendGenericPacket(power_inst, eps_config, sizeof(eps_config_t), NULL, 0, (uint64_t)CSP_EPS_PORT_CONFIG_GET);
}

static returnCode_t ConfigSet(epsInst_t *power_inst, eps_config_t *eps_config)
{
    return SendGenericPacket(power_inst, NULL, 0, eps_config, sizeof(eps_config_t), (uint64_t)CSP_EPS_PORT_CONFIG_SET);
}

static returnCode_t HardReset(epsInst_t *power_inst)
{
    return SendGenericPacket(power_inst, NULL, 0, NULL, 0, (uint64_t)CSP_EPS_PORT_HARD_RESET);
}

static returnCode_t ConfigCmd2(epsInst_t *power_inst, uint8_t cmd)
{
    return SendGenericPacket(power_inst, &cmd, sizeof(uint8_t), NULL, 0, (uint64_t)CSP_EPS_PORT_CONFIG2_CMD);
}

static returnCode_t ConfigGet2(epsInst_t *power_inst, eps_config2_t *eps_config2)
{
    return SendGenericPacket(power_inst, eps_config2, sizeof(eps_config2_t), NULL, 0, (uint64_t)CSP_EPS_PORT_CONFIG2_GET);
}

static returnCode_t ConfigSet2(epsInst_t *power_inst, eps_config2_t *eps_config2)
{
    return SendGenericPacket(power_inst, NULL, 0, eps_config2, sizeof(eps_config2_t), (uint64_t)CSP_EPS_PORT_CONFIG2_SET);
}


returnCode_t PowerStart(epsInst_t *power_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    if (power_inst != NULL)
    {
        // Open a new device
        return_value = DeviceOpen(&power_inst->dev_i2c, DEVICE_TYPE_PERIPHERAL, I2C_AVIONIC);
        if (return_value == RET_SUCCESSFUL)
        {
            // Setup the slave's I2C address
            return_value = DeviceIoctl(power_inst->dev_i2c, IOCTL_I2C_SET_SLAVE_ADDR, power_inst->i2c_address, sizeof(power_inst->i2c_address));

            if (return_value == RET_SUCCESSFUL)
            {

                // Quoient faire ?

            }
        }

    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

