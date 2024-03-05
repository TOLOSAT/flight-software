/**
 * @file    iridium_driver.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Iridium Driver
 * @date    04/03/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "iridium_driver.h"
#include "iridium_constants.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static iridiumStatus_t IridiumCheckPresence(iridiumInst_t *iridium_inst);
// static iridiumStatus_t IridiumSetupHW(iridiumInst_t *iridium_inst);
// static iridiumStatus_t IridiumIdentification(iridiumInst_t *iridium_inst);
static iridiumStatus_t IridiumStrCompare(uint8_t *str1, uint8_t *str2, uint32_t len);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              IridiumStart(iridiumInst_t *iridium_inst)
 * @brief           Start a connection with the Iridium transceiver
 * @param[in,out]   iridium_inst Iridium instance used by the driver
 * @retval          #IRIDIUM_INVALID_PARAM if there is a null pointer, or baudrate is not set correctly
 * @retval          #IRIDIUM_ERROR if an error occured during the initialisation
 * @retval          #IRIDIUM_SUCCESSFUL if the start procedure went well
 */
iridiumStatus_t IridiumStart(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        // Get HW CTRL parameters
        uint8_t baudrate = (iridium_inst->hw_ctrl_reg & HW_CTRL_REG_BAUDRATE_MASK) >> HW_CTRL_REG_BAUDRATE_POS;

        // First check if baudrate is correctly setup
        switch (baudrate)
        {
        case 1u:
            if (iridium_inst->uart_inst->baudrate != 600u)
            {
                return_value = IRIDIUM_INVALID_PARAM;
            }
            break;
        case 2u:
            if (iridium_inst->uart_inst->baudrate != 1200u)
            {
                return_value = IRIDIUM_INVALID_PARAM;
            }
            break;
        case 3u:
            if (iridium_inst->uart_inst->baudrate != 2400u)
            {
                return_value = IRIDIUM_INVALID_PARAM;
            }
            break;
        case 4u:
            if (iridium_inst->uart_inst->baudrate != 4800u)
            {
                return_value = IRIDIUM_INVALID_PARAM;
            }
            break;
        case 5u:
            if (iridium_inst->uart_inst->baudrate != 9600u)
            {
                return_value = IRIDIUM_INVALID_PARAM;
            }
            break;
        case 6u:
            if (iridium_inst->uart_inst->baudrate != 19200u)
            {
                return_value = IRIDIUM_INVALID_PARAM;
            }
            break;
        case 7u:
            if (iridium_inst->uart_inst->baudrate != 38400u)
            {
                return_value = IRIDIUM_INVALID_PARAM;
            }
            break;
        case 8u:
            if (iridium_inst->uart_inst->baudrate != 57600u)
            {
                return_value = IRIDIUM_INVALID_PARAM;
            }
            break;
        case 9u:
            if (iridium_inst->uart_inst->baudrate != 115200u)
            {
                return_value = IRIDIUM_INVALID_PARAM;
            }
            break;
        default:
            return_value = IRIDIUM_INVALID_PARAM;
            break;
        }

        // Continue only if baudrate is correctly selected
        if (return_value == IRIDIUM_SUCCESSFUL)
        {
            // Then CheckPresence
            return_value = IridiumCheckPresence(iridium_inst);
        }
    }
    else
    {
        return_value = IRIDIUM_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumCheckPresence(iridiumInst_t *iridium_inst)
 * @brief       Check if an Iridium transceiver is present
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_ERROR if there is no iridium transceiver or uart is not working
 * @retval      #IRIDIUM_SUCCESSFUL if an iridium transceiver is detected
 */
static iridiumStatus_t IridiumCheckPresence(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        halStatus_t test_hal = GEN_HAL_SUCCESSFUL;
        uint8_t at_tx_msg[3u] = AT_CMD_EMPTY;
        uint8_t at_rx_msg[3u] = {0};
        // Send
        test_hal = UartWrite(iridium_inst->uart_inst, at_tx_msg, 3u);
        if (test_hal == GEN_HAL_SUCCESSFUL)
        {
            test_hal = UartRead(iridium_inst->uart_inst, at_rx_msg, 3u);
            if (test_hal == GEN_HAL_SUCCESSFUL)
            {
                // Check if we have received OK message
                return_value = IridiumStrCompare(at_rx_msg, (uint8_t *)AT_OK_ANSWER, 2u);
            }
            else
            {
                return_value = IRIDIUM_ERROR;
            }
        }
        else
        {
            return_value = IRIDIUM_ERROR;
        }
    }
    else
    {
        return_value = IRIDIUM_INVALID_PARAM;
    }

    return return_value;
}

// static iridiumStatus_t IridiumSetupHW(iridiumInst_t *iridium_inst)
// {
//     // Variable Initialisation
//     iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

//     // Function Core
//     if (iridium_inst != NULL)
//     {
//         // halStatus_t test_hal = GEN_HAL_SUCCESSFUL;
//         // uint8_t at_tx_msg[3u] = {0};
//         // uint8_t at_rx_msg[3u] = {0};
//         // uint8_t baudrate = (iridium_inst->hw_ctrl_reg & HW_CTRL_REG_BAUDRATE_MASK) >> HW_CTRL_REG_BAUDRATE_POS;
//         // uint8_t dtr_mode = (iridium_inst->hw_ctrl_reg & HW_CTRL_REG_DTR_MODE_MASK) >> HW_CTRL_REG_DTR_MODE_POS;
//         // uint8_t ctrl_flow_mode = (iridium_inst->hw_ctrl_reg & HW_CTRL_REG_CTRL_FLOW_MODE_MASK) >> HW_CTRL_REG_CTRL_FLOW_MODE_POS;
//         // uint8_t ring_mode = (iridium_inst->hw_ctrl_reg & HW_CTRL_REG_RING_MODE_MASK) >> HW_CTRL_REG_RING_MODE_POS;
//         // uint8_t echo_mode = (iridium_inst->hw_ctrl_reg & HW_CTRL_REG_ECHO_MODE_MASK) >> HW_CTRL_REG_ECHO_MODE_POS;

//         // First set baudrate

//     }
//     else
//     {
//         return_value = IRIDIUM_INVALID_PARAM;
//     }

//     return return_value;
// }

// static iridiumStatus_t IridiumIdentification(iridiumInst_t *iridium_inst)
// {
//     // Variable Initialisation
//     iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

//     // Function Core
//     if (iridium_inst != NULL)
//     {

//     }
//     else
//     {
//         return_value = IRIDIUM_INVALID_PARAM;
//     }

//     return return_value;
// }

/**
 * @fn          IridiumStrCompare(uint8_t *str1, uint8_t *str2, uint32_t len)
 * @brief       This function compares two strings together.
 * @param[in]   str1 First string
 * @param[in]   str2 Second string
 * @param[in]   len Length of the two strings
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer or len is zero
 * @retval      #IRIDIUM_ERROR if the two strings are different
 * @retval      #IRIDIUM_SUCCESSFUL else
 */
static iridiumStatus_t IridiumStrCompare(uint8_t *str1, uint8_t *str2, uint32_t len)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if ((str1 != NULL) && (str2 != NULL) && (len != 0u))
    {
        uint32_t i = 0u;
        while ((return_value == IRIDIUM_SUCCESSFUL) && (i < len))
        {
            if (str1[i] != str2[i])
            {
                return_value = IRIDIUM_ERROR;
            }
            i++;
        }
    }
    else
    {
        return_value = IRIDIUM_INVALID_PARAM;
    }

    return return_value;
}