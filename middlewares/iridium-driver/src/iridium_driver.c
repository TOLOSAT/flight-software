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

#define ASCII_NUMBER_OFFSET (uint8_t)'0'   /**< Correspond to the '0' character */

/*************************** Functions Declarations **************************/

// Init static function
static iridiumStatus_t IridiumCheckBaudrate(iridiumInst_t *iridium_inst);
static iridiumStatus_t IridiumCheckPresence(iridiumInst_t *iridium_inst);
static iridiumStatus_t IridiumSetupHW(iridiumInst_t *iridium_inst);

// Generic static function
static iridiumStatus_t IridiumSetCommand(iridiumInst_t *iridium_inst, const char *command, uint8_t command_size,
                                         uint8_t *arg, uint8_t arg_size, uint8_t arg_pos);
static iridiumStatus_t IridiumCheckAnswer(iridiumInst_t *iridium_inst, char *answer, uint8_t *length);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              IridiumStart(iridiumInst_t *iridium_inst)
 * @brief           Start a connection with the Iridium transceiver
 * @param[in,out]   iridium_inst Iridium instance used by the driver
 * @retval          #IRIDIUM_INVALID_PARAM if there is a null pointer
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
        // First check baudrate
        return_value = IridiumCheckBaudrate(iridium_inst);

        // Continue only if baudrate is correctly selected
        if (return_value == IRIDIUM_SUCCESSFUL)
        {
            // Then CheckPresence
            return_value = IridiumCheckPresence(iridium_inst);

            // Continue if a transceiver is detected
            if (return_value == IRIDIUM_SUCCESSFUL)
            {
                return_value = IridiumSetupHW(iridium_inst);
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

/**
 * @fn          IridiumCheckBaudrate(iridiumInst_t *iridium_inst)
 * @brief       Check if wanted baudrate is the baudrate used for UART
 * @param[in]   iridium_inst iridium_inst Iridium instance used by the driver
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_ERROR if baudrate is not set correctly
 * @retval      #IRIDIUM_SUCCESSFUL else
 */
static iridiumStatus_t IridiumCheckBaudrate(iridiumInst_t *iridium_inst)
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
                return_value = IRIDIUM_ERROR;
            }
            break;
        case 2u:
            if (iridium_inst->uart_inst->baudrate != 1200u)
            {
                return_value = IRIDIUM_ERROR;
            }
            break;
        case 3u:
            if (iridium_inst->uart_inst->baudrate != 2400u)
            {
                return_value = IRIDIUM_ERROR;
            }
            break;
        case 4u:
            if (iridium_inst->uart_inst->baudrate != 4800u)
            {
                return_value = IRIDIUM_ERROR;
            }
            break;
        case 5u:
            if (iridium_inst->uart_inst->baudrate != 9600u)
            {
                return_value = IRIDIUM_ERROR;
            }
            break;
        case 6u:
            if (iridium_inst->uart_inst->baudrate != 19200u)
            {
                return_value = IRIDIUM_ERROR;
            }
            break;
        case 7u:
            if (iridium_inst->uart_inst->baudrate != 38400u)
            {
                return_value = IRIDIUM_ERROR;
            }
            break;
        case 8u:
            if (iridium_inst->uart_inst->baudrate != 57600u)
            {
                return_value = IRIDIUM_ERROR;
            }
            break;
        case 9u:
            if (iridium_inst->uart_inst->baudrate != 115200u)
            {
                return_value = IRIDIUM_ERROR;
            }
            break;
        default:
            return_value = IRIDIUM_ERROR;
            break;
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
        uint8_t at_tx_msg[AT_CMD_EMPTY_SIZE] = {0};
        (void)memcpy(at_tx_msg, AT_CMD_EMPTY, AT_CMD_EMPTY_SIZE);
        // Send
        halStatus_t test_hal = UartWrite(iridium_inst->uart_inst, at_tx_msg, AT_CMD_EMPTY_SIZE);
        if (test_hal == GEN_HAL_SUCCESSFUL)
        {
            return_value = IridiumCheckAnswer(iridium_inst, NULL, NULL);
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

static iridiumStatus_t IridiumSetupHW(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        // First set baudrate
        uint8_t baudrate = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_BAUDRATE_MASK) >> HW_CTRL_REG_BAUDRATE_POS) + ASCII_NUMBER_OFFSET;
        return_value = IridiumSetCommand(iridium_inst, AT_CMD_SET_BAUDRATE, AT_CMD_SET_BAUDRATE_SIZE,
                                         &baudrate, 1u, AT_CMD_SET_BAUDRATE_ARG_POS);
        if (return_value == IRIDIUM_SUCCESSFUL)
        {
            // Then set hardware control flow mode
            uint8_t ctrl_flow_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_CTRL_FLOW_MODE_MASK) >> HW_CTRL_REG_CTRL_FLOW_MODE_POS) + ASCII_NUMBER_OFFSET;
            return_value = IridiumSetCommand(iridium_inst, AT_CMD_SET_FLOW_CTRL, AT_CMD_SET_FLOW_CTRL_SIZE,
                                             &ctrl_flow_mode, 1u, AT_CMD_SET_FLOW_CTRL_ARG_POS);
            if (return_value == IRIDIUM_SUCCESSFUL)
            {
                // Then set hardware DTR mode
                uint8_t dtr_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_DTR_MODE_MASK) >> HW_CTRL_REG_DTR_MODE_POS) + ASCII_NUMBER_OFFSET;
                return_value = IridiumSetCommand(iridium_inst, AT_CMD_SET_DTR, AT_CMD_SET_DTR_SIZE,
                                                 &dtr_mode, 1u, AT_CMD_SET_DTR_ARG_POS);
                if (return_value == IRIDIUM_SUCCESSFUL)
                {
                    // Then set hardware DTR mode
                    uint8_t echo_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_ECHO_MODE_MASK) >> HW_CTRL_REG_ECHO_MODE_POS) + ASCII_NUMBER_OFFSET;
                    return_value = IridiumSetCommand(iridium_inst, AT_CMD_ECHO, AT_CMD_ECHO_SIZE,
                                                     &echo_mode, 1u, AT_CMD_ECHO_ARG_POS);
                    if (return_value == IRIDIUM_SUCCESSFUL)
                    {
                        // Then set ring alert mode
                        uint8_t ring_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_RING_MODE_MASK) >> HW_CTRL_REG_RING_MODE_POS) + ASCII_NUMBER_OFFSET;
                        return_value = IridiumSetCommand(iridium_inst, AT_CMD_SBD_SET_RING_ALERT, AT_CMD_SBD_SET_RING_ALERT_SIZE,
                                                         &ring_mode, 1u, AT_CMD_SBD_SET_RING_ALERT_ARG_POS);
                        if (return_value == IRIDIUM_SUCCESSFUL)
                        {
                            // Then set quiet mode
                            uint8_t quiet_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_QUIET_MASK) >> HW_CTRL_REG_QUIET_POS) + ASCII_NUMBER_OFFSET;
                            return_value = IridiumSetCommand(iridium_inst, AT_CMD_QUIET_MODE, AT_CMD_QUIET_MODE_SIZE,
                                                             &quiet_mode, 1u, AT_CMD_QUIET_MODE_ARG_POS);
                            if (return_value == IRIDIUM_SUCCESSFUL)
                            {
                                // Finally set the verbosity
                                uint8_t verbosity = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_VERBOSITY_MASK) >> HW_CTRL_REG_VERBOSITY_POS) + ASCII_NUMBER_OFFSET;
                                return_value = IridiumSetCommand(iridium_inst, AT_CMD_VERBOSE_MODE, AT_CMD_VERBOSE_MODE_SIZE,
                                                                 &verbosity, 1u, AT_CMD_VERBOSE_MODE_ARG_POS);
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        return_value = IRIDIUM_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumSetCommand(iridiumInst_t *iridium_inst, uint8_t *command, uint8_t command_size, uint8_t *arg, uint8_t arg_size, uint8_t arg_pos)
 * @brief       Function that sends an AT command to the Iridium transceiver
 * @param[in]   iridium_inst    Iridium instance used by the driver
 * @param[in]   command         Command to be send (include dummy arg)
 * @param[in]   command_size    Size of the command (include arg)
 * @param[in]   arg             Argument to replace in the command
 * @param[in]   arg_size        Size of the argument
 * @param[in]   arg_pos         Position of the argument in the function
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_ERROR if the command has encountered an error
 * @retval      #IRIDIUM_SUCCESSFUL else
 */
static iridiumStatus_t IridiumSetCommand(iridiumInst_t *iridium_inst, const char *command, uint8_t command_size,
                                         uint8_t *arg, uint8_t arg_size, uint8_t arg_pos)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if ((iridium_inst != NULL) && (command != NULL) && (command_size != 0u) && (arg_size < command_size))
    {
        // Setup the message
        uint8_t at_tx_msg[AT_MSG_MAX_SIZE] = {0};
        (void)memcpy(&at_tx_msg[0], command, command_size);

        if ((arg_size != 0u) && (arg != NULL))
        {
            (void)memcpy(&at_tx_msg[arg_pos], arg, arg_size);
        }

        // Send the message
        halStatus_t test_hal = UartWrite(iridium_inst->uart_inst, at_tx_msg, command_size);
        if (test_hal == GEN_HAL_SUCCESSFUL)
        {
            return_value = IridiumCheckAnswer(iridium_inst, NULL, NULL);
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

/**
 * @fn          IridiumCheckAnswer(iridiumInst_t *iridium_inst, uint8_t *answer, uint8_t length)
 * @brief       Check if the transceiver answered to the command and forward the answer if any
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @param[out]  answer Command answer if any
 * @param[in]   length Size of the answer
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_ERROR if the transceiver answered something else than OK
 * @retval      #IRIDIUM_SUCCESSFUL if an ok is received
 */
static iridiumStatus_t IridiumCheckAnswer(iridiumInst_t *iridium_inst, char *answer, uint8_t *length)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;
    (void)(answer);
    (void)(length);

    // Function Core
    if ((iridium_inst != NULL))
    {
        // First Read UART
        uint8_t at_rx_msg[AT_MSG_MAX_SIZE] = {0};
        halStatus_t test_hal = UartRead(iridium_inst->uart_inst, at_rx_msg, AT_MSG_MAX_SIZE);
        if (test_hal == GEN_HAL_SUCCESSFUL)
        {
            // Start the parsing
            uint32_t i = 0u;
            uint32_t end_of_parsing = 0u;
            while ((end_of_parsing == 0u) && (i < AT_MSG_MAX_SIZE))
            {
                // Look for useless characters at the begining of the frame
                if ((at_rx_msg[i] == (uint8_t)'\r') && (at_rx_msg[i + 1u] == (uint8_t)'\n'))
                {
                    i += 2u; // Skip those characters if any
                }

                // Check for ERROR or OK
                if (strncmp((char *)&at_rx_msg[i], AT_OK_ANSWER, AT_OK_ANSWER_SIZE) == 0)
                {
                    return_value = IRIDIUM_SUCCESSFUL;
                    end_of_parsing = 1u;
                }
                else if (strncmp((char *)&at_rx_msg[i], AT_ERROR_ANSWER, AT_ERROR_ANSWER_SIZE) == 0)
                {
                    return_value = IRIDIUM_ERROR;
                    end_of_parsing = 1u;
                }
                else
                {
                    // We move forward
                    i++;
                }
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
