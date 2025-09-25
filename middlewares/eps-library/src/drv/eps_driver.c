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

// All commands are from the data sheet "gs-ds-nanopower-p31u-8.0.pdf"

/**
 * @brief Get the Hk 1 object
 *
 * @param power_inst
 * @param hk
 * @return returnCode_t
 */
static returnCode_t GetHk1(epsInst_t *power_inst, hkparam_t *hk)
{
    return SendGenericPacket(power_inst, NULL, 0, hk, sizeof(hkparam_t), (uint64_t)CSP_EPS_PORT_HK);
}

/**
 * @brief Get the Hk 2 object
 *
 * @param power_inst
 * @param hk
 * @return returnCode_t
 */
static returnCode_t GetHk2(epsInst_t *power_inst, eps_hk_t *hk)
{
    uint8_t mode = 0;
    return SendGenericPacket(power_inst, &mode, sizeof(mode), hk, sizeof(eps_hk_t), (uint64_t)CSP_EPS_PORT_HK);
}

/**
 * @brief Get the Hk 2 Vi object
 *
 * @param power_inst
 * @param hk
 * @return returnCode_t
 */
static returnCode_t GetHk2Vi(epsInst_t *power_inst, eps_hk_vi_t *hk)
{
    uint8_t mode = 1;
    return SendGenericPacket(power_inst, &mode, sizeof(mode), hk, sizeof(eps_hk_vi_t), (uint64_t)CSP_EPS_PORT_HK);
}

/**
 * @brief Get the Hk 2 Wdt object
 *
 * @param power_inst
 * @param hk
 * @return returnCode_t
 */
static returnCode_t GetHk2Wdt(epsInst_t *power_inst, eps_hk_wdt_t *hk)
{
    uint8_t mode = 2;
    return SendGenericPacket(power_inst, &mode, sizeof(mode), hk, sizeof(eps_hk_wdt_t), (uint64_t)CSP_EPS_PORT_HK);
}

/**
 * @brief Get the Hk 2 Basic object
 *
 * @param power_inst
 * @param hk
 * @return returnCode_t
 */
static returnCode_t GetHk2Basic(epsInst_t *power_inst, eps_hk_basic_t *hk)
{
    uint8_t mode = 3;
    return SendGenericPacket(power_inst, &mode, sizeof(mode), hk, sizeof(eps_hk_basic_t), (uint64_t)CSP_EPS_PORT_HK);
}

/**
 * @brief Set the Output object
 *
 * @param power_inst
 * @param output_byte
 * @return returnCode_t
 */
static returnCode_t SetOutput(epsInst_t *power_inst, uint8_t output_byte)
{
    return SendGenericPacket(power_inst, &output_byte, sizeof(output_byte), NULL, 0, (uint64_t)CSP_EPS_PORT_OUTPUT);
}

/**
 * @brief Set the Single Output object
 *
 * @param power_inst
 * @param channel
 * @param value
 * @param delay
 * @return returnCode_t
 */
static returnCode_t SetSingleOutput(epsInst_t *power_inst, uint8_t channel, uint8_t value, uint16_t delay)
{
    uint8_t params[4] = { channel, value, (uint8_t)(delay >> 8), (uint8_t)(delay & 0xFF) };
    return SendGenericPacket(power_inst, params, sizeof(params), NULL, 0, (uint64_t)CSP_EPS_PORT_SINGLE_OUTPUT);
}

/**
 * @brief Set the PV Voltages
 *
 * @param power_inst
 * @param voltage1
 * @param voltage2
 * @param voltage3
 * @return returnCode_t
 */
static returnCode_t SetPVVolt(epsInst_t *power_inst, uint16_t voltage1, uint16_t voltage2, uint16_t voltage3)
{
    uint8_t voltages[6] = { (uint8_t)(voltage1 >> 8),   (uint8_t)(voltage1 & 0xFF), (uint8_t)(voltage2 >> 8),
                            (uint8_t)(voltage2 & 0xFF), (uint8_t)(voltage3 >> 8),   (uint8_t)(voltage3 & 0xFF) };
    return SendGenericPacket(power_inst, voltages, sizeof(voltages), NULL, 0, (uint64_t)CSP_EPS_PORT_VOLT);
}

/**
 * @brief Set the Heater Auto object
 *
 * @param power_inst
 * @param mode
 * @param returned_mode
 * @return returnCode_t
 */
static returnCode_t SetHeaterAuto(epsInst_t *power_inst, uint8_t mode, uint8_t *returned_mode)
{
    return SendGenericPacket(power_inst, &mode, sizeof(uint8_t), returned_mode, sizeof(uint8_t), (uint64_t)CSP_EPS_PORT_AUTO);
}

/**
 * @brief Reset the Counters object
 *
 * @param power_inst
 * @return returnCode_t
 */
static returnCode_t ResetCounters(epsInst_t *power_inst)
{
    uint8_t magic = 0x42;
    return SendGenericPacket(power_inst, &magic, sizeof(uint8_t), NULL, 0, (uint64_t)CSP_EPS_PORT_RESET_COUNTER);
}

/**
 * @brief Reset the WDT object
 *
 * @param power_inst
 * @return returnCode_t
 */
static returnCode_t ResetWDT(epsInst_t *power_inst)
{
    uint8_t magic = 0x78;
    return SendGenericPacket(power_inst, &magic, sizeof(uint8_t), NULL, 0, (uint64_t)CSP_EPS_PORT_RESET_WDT);
}

/**
 * @brief Control config system 1 on the EPS
 *
 * @param power_inst
 * @param cmd
 * @return returnCode_t
 */
static returnCode_t ConfigCmd(epsInst_t *power_inst, uint8_t cmd)
{
    return SendGenericPacket(power_inst, &cmd, sizeof(uint8_t), NULL, 0, (uint64_t)CSP_EPS_PORT_CONFIG_CMD);
}

/**
 * @brief Get the config from the EPS
 *
 * @param power_inst
 * @param eps_config
 * @return returnCode_t
 */
static returnCode_t ConfigGet(epsInst_t *power_inst, eps_config_t *eps_config)
{
    return SendGenericPacket(power_inst, eps_config, sizeof(eps_config_t), NULL, 0, (uint64_t)CSP_EPS_PORT_CONFIG_GET);
}

/**
 * @brief Set the config to the EPS
 *
 * @param power_inst
 * @param eps_config
 * @return returnCode_t
 */
static returnCode_t ConfigSet(epsInst_t *power_inst, eps_config_t *eps_config)
{
    return SendGenericPacket(power_inst, NULL, 0, eps_config, sizeof(eps_config_t), (uint64_t)CSP_EPS_PORT_CONFIG_SET);
}

/**
 * @brief Perform a hard reset on the EPS
 *
 * @param power_inst
 * @return returnCode_t
 */
static returnCode_t HardReset(epsInst_t *power_inst)
{
    return SendGenericPacket(power_inst, NULL, 0, NULL, 0, (uint64_t)CSP_EPS_PORT_HARD_RESET);
}

/**
 * @brief Control config system 2 on the EPS
 *
 * @param power_inst
 * @param cmd
 * @return returnCode_t
 */
static returnCode_t ConfigCmd2(epsInst_t *power_inst, uint8_t cmd)
{
    return SendGenericPacket(power_inst, &cmd, sizeof(uint8_t), NULL, 0, (uint64_t)CSP_EPS_PORT_CONFIG2_CMD);
}

/**
 * @brief Get the config 2 from the EPS
 *
 * @param power_inst
 * @param eps_config2
 * @return returnCode_t
 */
static returnCode_t ConfigGet2(epsInst_t *power_inst, eps_config2_t *eps_config2)
{
    return SendGenericPacket(power_inst, eps_config2, sizeof(eps_config2_t), NULL, 0, (uint64_t)CSP_EPS_PORT_CONFIG2_GET);
}

/**
 * @brief Set the config 2 to the EPS
 *
 * @param power_inst
 * @param eps_config2
 * @return returnCode_t
 */
static returnCode_t ConfigSet2(epsInst_t *power_inst, eps_config2_t *eps_config2)
{
    return SendGenericPacket(power_inst, NULL, 0, eps_config2, sizeof(eps_config2_t), (uint64_t)CSP_EPS_PORT_CONFIG2_SET);
}

/**
 * @fn          PowerStart(epsInst_t *power_inst)
 * @brief       Function that initialise the EPS
 * @param[in]   power_inst EPS instance used by the driver
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if i2c read or write has timeouted
 * @retval      #RET_ERROR if an error has been encountered when setting up the EPS
 * @retval      #RET_SUCCESSFUL
 */
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
                // Set default values and retrieve basic housekeeping
                ConfigSet(power_inst, &(eps_config_t){
                                          .ppt_mode                 = 1,
                                          .battheater_mode          = 1,
                                          .battheater_low           = -10,
                                          .battheater_high          = 0,
                                          .output_normal_value      = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
                                          .output_safe_value        = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                                          .output_initial_on_delay  = { 0, 0, 0, 0, 0, 0, 0, 0 },
                                          .output_initial_off_delay = { 0, 0, 0, 0, 0, 0, 0, 0 },
                                          .vboost                   = { 5000, 5000, 5000 },
                });
                ConfigSet2(power_inst, &(eps_config2_t){
                                           .batt_criticalvoltage = 3200,
                                           .batt_maxvoltage      = 4200,
                                           .batt_normalvoltage   = 3700,
                                           .batt_safevoltage     = 3400,
                                       });
                SetPVVolt(power_inst, 5000, 5000, 5000);
                SetOutput(power_inst, 0x00);
                SetHeaterAuto(power_inst, 1, NULL);
                SetSingleOutput(power_inst, 0, 0x00, 0);

                // make conf struct and inst struct
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
