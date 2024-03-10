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

#define ASCII_NUMBER_OFFSET 0x30u /**< Correspond to the '0' character */
#define ARRAY_MAX_SIZE_UINT16 6u  /**< Correspond to the "64535" size plus one for margin */
#define IRIDIUM_MAX_TIMEOUT 1000u /**< Max delay before timeout */

/*************************** Functions Declarations **************************/

// Init static function

static iridiumStatus_t IridiumCheckBaudrate(iridiumInst_t *iridium_inst);
static iridiumStatus_t IridiumCheckPresence(iridiumInst_t *iridium_inst);
static iridiumStatus_t IridiumSetupHW(iridiumInst_t *iridium_inst);
static iridiumStatus_t IridiumGetInfo(iridiumInst_t *iridium_inst);
static iridiumStatus_t IridiumSaveConf(iridiumInst_t *iridium_inst);

// SBD related function

static iridiumStatus_t IridiumSetupSBD(iridiumInst_t *iridium_inst);
static iridiumStatus_t IridiumParseSBDStatus(char *msg, uint32_t msg_length, iridiumSBDStatus_t *status);
static iridiumStatus_t IridiumSBDPutDataInBuffer(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t tx_msg);
static iridiumStatus_t IridiumSBDSendData(iridiumInst_t *iridium_inst);

// Generic static function

static iridiumStatus_t IridiumSendCommand(iridiumInst_t *iridium_inst, const char *command, uint8_t command_size,
                                          const char *arg, uint8_t arg_size, uint8_t arg_pos,
                                          char *answer, uint32_t *answer_size);
static iridiumStatus_t IridiumGetAnswer(iridiumInst_t *iridium_inst, char *answer, uint32_t *answer_size);
static iridiumStatus_t IridiumCheckAck(iridiumInst_t *iridium_inst);

// Miscellaneous

static uint16_t ConvertUint16FromASCII(const char ascii_array[ARRAY_MAX_SIZE_UINT16]);
static uint16_t ComputeHalfWordCheckSum(const uint8_t *data, uint32_t size);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              IridiumStart(iridiumInst_t *iridium_inst)
 * @brief           Start a connection with the Iridium transceiver
 * @param[in,out]   iridium_inst Iridium instance used by the driver
 * @retval          #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval          #IRIDIUM_TIMEOUT if uart read or write has timeouted
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
                iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_INIT;
                // Setup the transceiver
                return_value = IridiumSetupHW(iridium_inst);

                // Continue if the HW setup went well
                if (return_value == IRIDIUM_SUCCESSFUL)
                {
                    // Then get info from the transceiver
                    return_value = IridiumGetInfo(iridium_inst);
                    if (return_value == IRIDIUM_SUCCESSFUL)
                    {
                        // Then setup SBD
                        return_value = IridiumSetupSBD(iridium_inst);
                        if (return_value == IRIDIUM_SUCCESSFUL)
                        {
                            // Finally save the conf
                            return_value = IridiumSaveConf(iridium_inst);
                            if (return_value == IRIDIUM_SUCCESSFUL)
                            {
                                iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_READY;
                            }
                            else
                            {
                                iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_ERROR;
                                return_value = IRIDIUM_ERROR;
                            }
                        }
                        else
                        {
                            iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_ERROR;
                            return_value = IRIDIUM_ERROR;
                        }
                    }
                    else
                    {
                        iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_ERROR;
                        return_value = IRIDIUM_ERROR;
                    }
                }
                else
                {
                    iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_ERROR;
                    return_value = IRIDIUM_ERROR;
                }
            }
            else
            {
                iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_OFF;
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
 * @fn          IridiumSendSDB(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t tx_msg)
 * @brief       This function sends a message through SBD
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @param[in]   tx_msg Message to be sent
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_TIMEOUT if uart read or write has timeouted
 * @retval      #IRIDIUM_ERROR if an error occured during the discussion with the transceiver or before the call
 * @retval      #IRIDIUM_SUCCESSFUL else
 */
iridiumStatus_t IridiumSendSDB(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t tx_msg)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if ((iridium_inst != NULL) && (tx_msg != NULL))
    {
        // Check if the transceiver is available
        if (iridium_inst->iridium_state == IRIDIUM_TRANSCEIVER_READY)
        {
            // First get the SBD Status
            iridiumSBDStatus_t sbd_status = {0};
            return_value = IridiumGetSBDStatus(iridium_inst, &sbd_status);
            if ((return_value == IRIDIUM_SUCCESSFUL) &&
                (sbd_status.network_availability >= iridium_inst->minimum_availability))
            {
                if (sbd_status.tx_message_presence == IRIDIUM_SBD_MSG_NOT_PRESENT)
                {
                    return_value = IridiumSBDPutDataInBuffer(iridium_inst, tx_msg);
                    if (return_value == IRIDIUM_SUCCESSFUL)
                    {
                        return_value = IridiumSBDSendData(iridium_inst);
                    }
                }
                else
                {
                    return_value = IridiumSBDSendData(iridium_inst);
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

/**
 * @fn          IridiumGetNetworkAvailability(iridiumInst_t *iridium_inst, iridiumNetworkAvailability_t *availability)
 * @brief       Function that get the iridium network availability.
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @param[out]  availability Availability of the network
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_TIMEOUT if uart read or write has timeouted
 * @retval      #IRIDIUM_ERROR if an error occured during the discussion with the transceiver or before the call
 * @retval      #IRIDIUM_SUCCESSFUL else
 */
iridiumStatus_t IridiumGetNetworkAvailability(iridiumInst_t *iridium_inst, iridiumNetworkAvailability_t *availability)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        // Check if the transceiver is available
        if (iridium_inst->iridium_state == IRIDIUM_TRANSCEIVER_READY)
        {
            char answer[AT_MSG_MAX_SIZE] = {0};
            uint32_t answer_size = 0u;
            // Get network availability
            return_value = IridiumSendCommand(iridium_inst, AT_CMD_GET_SIGNAL_QUALITY, AT_CMD_GET_SIGNAL_QUALITY_SIZE,
                                              NULL, 0u, 0u, answer, &answer_size);
            if ((return_value == IRIDIUM_SUCCESSFUL) && (answer_size != 0u) && (answer_size >= AT_CMD_SIGNAL_QUALITY_ANS_MIN_SIZE))
            {
                *availability = (iridiumNetworkAvailability_t)(answer[AT_CMD_SIGNAL_QUALITY_ANSWER_OFFSET] - ASCII_NUMBER_OFFSET);
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
 * @fn          IridiumGetSBDStatus(iridiumInst_t *iridium_inst, iridiumSBDStatus_t *status)
 * @brief       This function gets the relevant information for the SBD (network availability, message presence, ...)
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @param[out]  status Struct including all the relevant information for the SBD
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_TIMEOUT if uart read or write has timeouted
 * @retval      #IRIDIUM_ERROR if an error occured during the discussion with the transceiver or before the call
 * @retval      #IRIDIUM_SUCCESSFUL else
 */
iridiumStatus_t IridiumGetSBDStatus(iridiumInst_t *iridium_inst, iridiumSBDStatus_t *status)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        // Check if the transceiver is available
        if (iridium_inst->iridium_state == IRIDIUM_TRANSCEIVER_READY)
        {
            char answer[AT_MSG_MAX_SIZE] = {0};
            uint32_t answer_size = 0u;
            // First get network availability
            return_value = IridiumSendCommand(iridium_inst, AT_CMD_GET_SIGNAL_QUALITY, AT_CMD_GET_SIGNAL_QUALITY_SIZE,
                                              NULL, 0u, 0u, answer, &answer_size);
            if ((return_value == IRIDIUM_SUCCESSFUL) && (answer_size != 0u) && (answer_size >= AT_CMD_SIGNAL_QUALITY_ANS_MIN_SIZE))
            {
                status->network_availability = (iridiumNetworkAvailability_t)(answer[AT_CMD_SIGNAL_QUALITY_ANSWER_OFFSET] - ASCII_NUMBER_OFFSET);

                // Then get SDB Status
                return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_GET_STATUS_EXT, AT_CMD_SBD_GET_STATUS_EXT_SIZE,
                                                  NULL, 0u, 0u, answer, &answer_size);
                if ((return_value == IRIDIUM_SUCCESSFUL) && (answer_size != 0u) && (answer_size >= AT_CMD_SBD_GET_STAT_EXT_ANS_MIN_SIZE))
                {
                    return_value = IridiumParseSBDStatus(answer, answer_size, status);
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
 * @retval      #IRIDIUM_TIMEOUT if uart read or write has timeouted
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
            return_value = IridiumCheckAck(iridium_inst);
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
 * @fn          IridiumSetupHW(iridiumInst_t *iridium_inst)
 * @brief       This function sets up the transceiver.
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_TIMEOUT if uart read or write has timeouted
 * @retval      #IRIDIUM_ERROR if an error has been encountered when setting up the hardware
 * @retval      #IRIDIUM_SUCCESSFUL
 */
static iridiumStatus_t IridiumSetupHW(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        // First set baudrate
        char baudrate = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_BAUDRATE_MASK) >> HW_CTRL_REG_BAUDRATE_POS) + ASCII_NUMBER_OFFSET;
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_SET_BAUDRATE, AT_CMD_SET_BAUDRATE_SIZE,
                                          &baudrate, 1u, AT_CMD_SET_BAUDRATE_ARG_POS, NULL, NULL);
        if (return_value == IRIDIUM_SUCCESSFUL)
        {
            // Then set hardware control flow mode
            char ctrl_flow_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_CTRL_FLOW_MODE_MASK) >> HW_CTRL_REG_CTRL_FLOW_MODE_POS) + ASCII_NUMBER_OFFSET;
            return_value = IridiumSendCommand(iridium_inst, AT_CMD_SET_FLOW_CTRL, AT_CMD_SET_FLOW_CTRL_SIZE,
                                              &ctrl_flow_mode, 1u, AT_CMD_SET_FLOW_CTRL_ARG_POS, NULL, NULL);
            if (return_value == IRIDIUM_SUCCESSFUL)
            {
                // Then set hardware DTR mode
                char dtr_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_DTR_MODE_MASK) >> HW_CTRL_REG_DTR_MODE_POS) + ASCII_NUMBER_OFFSET;
                return_value = IridiumSendCommand(iridium_inst, AT_CMD_SET_DTR, AT_CMD_SET_DTR_SIZE,
                                                  &dtr_mode, 1u, AT_CMD_SET_DTR_ARG_POS, NULL, NULL);
                if (return_value == IRIDIUM_SUCCESSFUL)
                {
                    // Then set hardware DTR mode
                    char echo_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_ECHO_MODE_MASK) >> HW_CTRL_REG_ECHO_MODE_POS) + ASCII_NUMBER_OFFSET;
                    return_value = IridiumSendCommand(iridium_inst, AT_CMD_ECHO, AT_CMD_ECHO_SIZE,
                                                      &echo_mode, 1u, AT_CMD_ECHO_ARG_POS, NULL, NULL);
                    if (return_value == IRIDIUM_SUCCESSFUL)
                    {
                        // Then set ring alert mode
                        char ring_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_RING_MODE_MASK) >> HW_CTRL_REG_RING_MODE_POS) + ASCII_NUMBER_OFFSET;
                        return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_SET_MSG_RX_ALERT, AT_CMD_SBD_SET_MSG_RX_ALERT_SIZE,
                                                          &ring_mode, 1u, AT_CMD_SBD_SET_MSG_RX_ALERT_ARG_POS, NULL, NULL);
                        if (return_value == IRIDIUM_SUCCESSFUL)
                        {
                            // Then set quiet mode
                            char quiet_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_QUIET_MASK) >> HW_CTRL_REG_QUIET_POS) + ASCII_NUMBER_OFFSET;
                            return_value = IridiumSendCommand(iridium_inst, AT_CMD_QUIET_MODE, AT_CMD_QUIET_MODE_SIZE,
                                                              &quiet_mode, 1u, AT_CMD_QUIET_MODE_ARG_POS, NULL, NULL);
                            if (return_value == IRIDIUM_SUCCESSFUL)
                            {
                                // Finally set the verbosity
                                char verbosity = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_VERBOSITY_MASK) >> HW_CTRL_REG_VERBOSITY_POS) + ASCII_NUMBER_OFFSET;
                                return_value = IridiumSendCommand(iridium_inst, AT_CMD_VERBOSE_MODE, AT_CMD_VERBOSE_MODE_SIZE,
                                                                  &verbosity, 1u, AT_CMD_VERBOSE_MODE_ARG_POS, NULL, NULL);
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
 * @fn              IridiumGetInfo(iridiumInst_t *iridium_inst)
 * @brief           Function that get info from the transceiver such as manufacturer, model, and serial number
 * @param[in,out]   iridium_inst Iridium instance used by the driver
 * @retval          #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval          #IRIDIUM_TIMEOUT if uart read or write has timeouted
 * @retval          #IRIDIUM_ERROR if an error has been encountered when getting info
 * @retval          #IRIDIUM_SUCCESSFUL
 */
static iridiumStatus_t IridiumGetInfo(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        char answer[AT_MSG_MAX_SIZE] = {0};
        uint32_t answer_size = 0u;
        // First get the manufacturer ID
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_GET_MANUFACT_ID, AT_CMD_GET_MANUFACT_ID_SIZE,
                                          NULL, 0u, 0u, answer, &answer_size);
        if ((return_value == IRIDIUM_SUCCESSFUL) && (answer_size != 0u))
        {
            // Update Manufacturer
            (void)memcpy(iridium_inst->manufacturer_id, answer, answer_size);

            // Then get model id
            return_value = IridiumSendCommand(iridium_inst, AT_CMD_GET_MODEL_ID, AT_CMD_GET_MODEL_ID_SIZE,
                                              NULL, 0u, 0u, answer, &answer_size);
            if ((return_value == IRIDIUM_SUCCESSFUL) && (answer_size != 0u))
            {
                // Update Manufacturer
                (void)memcpy(iridium_inst->model_id, answer, answer_size);

                // Finally get the serial number
                return_value = IridiumSendCommand(iridium_inst, AT_CMD_GET_SERIAL_NB, AT_CMD_GET_SERIAL_NB_SIZE,
                                                  NULL, 0u, 0u, answer, &answer_size);
                if ((return_value == IRIDIUM_SUCCESSFUL) && (answer_size != 0u))
                {
                    // Update Serial Number
                    (void)memcpy(iridium_inst->serial_number, answer, answer_size);
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
 * @fn          IridiumSetupSBD(iridiumInst_t *iridium_inst)
 * @brief       Function that setup the transceiver for SBD
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_TIMEOUT if uart read or write has timeouted
 * @retval      #IRIDIUM_ERROR if an error has been encountered when setting up SBD
 * @retval      #IRIDIUM_SUCCESSFUL
 */
static iridiumStatus_t IridiumSetupSBD(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        char answer[AT_MSG_MAX_SIZE] = {0};
        uint32_t answer_size = 0u;
        // First clear Mobile Originated Message Sequence Number
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_CLEAR_MSG_SEQ_NB, AT_CMD_SBD_CLEAR_MSG_SEQ_NB_SIZE,
                                          NULL, 0u, 0u, answer, &answer_size);
        if ((return_value == IRIDIUM_SUCCESSFUL) && (answer_size != 0u) && (answer[AT_NUMERIC_ANSWER_CHAR_OFFSET] == AT_NUMERIC_OK_ANSWER[AT_NUMERIC_ANSWER_CHAR_OFFSET]))
        {
            // Then clear all buffers
            char clear_buffer_sel = '2';
            return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_CLEAR_MSG_BUFF, AT_CMD_SBD_CLEAR_MSG_BUFF_SIZE,
                                              &clear_buffer_sel, 1u, AT_CMD_SBD_CLEAR_MSG_BUFF_ARG_POS, answer, &answer_size);
            if ((return_value == IRIDIUM_SUCCESSFUL) && (answer_size != 0u) && (answer[AT_NUMERIC_ANSWER_CHAR_OFFSET] == AT_NUMERIC_OK_ANSWER[AT_NUMERIC_ANSWER_CHAR_OFFSET]))
            {
                char timeout = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_SBD_TIMEOUT_MASK) >> HW_CTRL_REG_SBD_TIMEOUT_POS) + ASCII_NUMBER_OFFSET;
                return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_SET_TIMEOUT, AT_CMD_SBD_SET_TIMEOUT_SIZE,
                                                  &timeout, 1u, AT_CMD_SBD_SET_TIMEOUT_ARG_POS, NULL, NULL);
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
 * @fn          IridiumSaveConf(iridiumInst_t *iridium_inst)
 * @brief       Function that save the current configuration in non volatile memory of the transceiver
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_TIMEOUT if uart read or write has timeouted
 * @retval      #IRIDIUM_ERROR if an error has been encountered when setting up SBD
 * @retval      #IRIDIUM_SUCCESSFUL
 */
static iridiumStatus_t IridiumSaveConf(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        // Save the conf in the profil 0
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_WRITE_CONF, AT_CMD_WRITE_CONF_SIZE,
                                          "0", 1u, AT_CMD_WRITE_CONF_ARG_POS, NULL, NULL);
    }
    else
    {
        return_value = IRIDIUM_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumParseSBDStatus(char *msg, uint32_t msg_length, iridiumSBDStatus_t *status)
 * @brief       This function takes the answer of the iridium transceiver containing the status and update the status variable
 * @param[in]   msg             Answer message given by the iridium transceiver (in ASCII)
 * @param[in]   msg_length      Size of the answer
 * @param[out]  status          SBD status (parsed from the message)
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer or message length is too small
 * @retval      #IRIDIUM_ERROR if an error occured
 * @retval      #IRIDIUM_SUCCESSFUL else
 */
static iridiumStatus_t IridiumParseSBDStatus(char *msg, uint32_t msg_length, iridiumSBDStatus_t *status)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if ((msg != NULL) && (msg_length > AT_CMD_SBD_STATUS_EXT_ANSW_HEAD_SIZE) && (status != NULL))
    {
        // First check if the header is right
        if (strncmp(msg, AT_CMD_SBD_STATUS_EXT_ANSW_HEAD, AT_CMD_SBD_STATUS_EXT_ANSW_HEAD_SIZE) == 0)
        {
            // Init index
            uint32_t i = AT_CMD_SBD_STATUS_EXT_ANSW_HEAD_SIZE;

            // Get MO Flag
            uint32_t number_buffer_index = 0u;
            char number_buffer[ARRAY_MAX_SIZE_UINT16] = {0};
            while ((i < msg_length) && (msg[i] != ','))
            {
                number_buffer[number_buffer_index] = msg[i];
                number_buffer_index++;
                i++;
            }
            status->tx_message_presence = ConvertUint16FromASCII(number_buffer);
            i += 2u; // Jump from 2 index because we have ", " useless char

            // Get MOMSN
            number_buffer_index = 0u;
            (void)memset(number_buffer, 0u, ARRAY_MAX_SIZE_UINT16);
            while ((i < msg_length) && (msg[i] != ','))
            {
                number_buffer[number_buffer_index] = msg[i];
                number_buffer_index++;
                i++;
            }
            status->tx_message_sequence_nb = ConvertUint16FromASCII(number_buffer);
            i += 2u; // Jump from 2 index because we have ", " useless char

            // Get MT Flag
            number_buffer_index = 0u;
            (void)memset(number_buffer, 0u, ARRAY_MAX_SIZE_UINT16);
            while ((i < msg_length) && (msg[i] != ','))
            {
                number_buffer[number_buffer_index] = msg[i];
                number_buffer_index++;
                i++;
            }
            status->rx_message_presence = ConvertUint16FromASCII(number_buffer);
            i += 2u; // Jump from 2 index because we have ", " useless char

            // Get MOMSN
            number_buffer_index = 0u;
            (void)memset(number_buffer, 0u, ARRAY_MAX_SIZE_UINT16);
            while ((i < msg_length) && (msg[i] != ','))
            {
                number_buffer[number_buffer_index] = msg[i];
                number_buffer_index++;
                i++;
            }
            status->rx_message_sequence_nb = ConvertUint16FromASCII(number_buffer);

            // Get RA status
            number_buffer_index = 0u;
            (void)memset(number_buffer, 0u, ARRAY_MAX_SIZE_UINT16);
            while ((i < msg_length) && (msg[i] != ','))
            {
                number_buffer[number_buffer_index] = msg[i];
                number_buffer_index++;
                i++;
            }
            status->ring_alert_status = ConvertUint16FromASCII(number_buffer);

            // Get NB RX message waiting
            number_buffer_index = 0u;
            (void)memset(number_buffer, 0u, ARRAY_MAX_SIZE_UINT16);
            while ((i < msg_length) && (msg[i] != ','))
            {
                number_buffer[number_buffer_index] = msg[i];
                number_buffer_index++;
                i++;
            }
            status->nb_rx_message = ConvertUint16FromASCII(number_buffer);
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
 * @fn          IridiumSBDPutDataInBuffer(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t tx_msg)
 * @brief       This function puts a message in the TX buffer of the transceiver
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @param[in]   tx_msg Message that will be sent
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_TIMEOUT if uart read or write has timeouted
 * @retval      #IRIDIUM_ERROR if an error has been encountered
 * @retval      #IRIDIUM_SUCCESSFUL
 */
static iridiumStatus_t IridiumSBDPutDataInBuffer(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t tx_msg)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if ((iridium_inst != NULL) && (tx_msg != NULL))
    {
        // Send the message to the buffer
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_WRITE_BIN_DATA, AT_CMD_SBD_WRITE_BIN_DATA_SIZE + 2u,
                                          IRIDIUM_SDB_TX_MSG_SIZE_ASCII, IRIDIUM_SDB_TX_MSG_SIZE_ASCII_SIZE, AT_CMD_SBD_WRITE_BIN_DATA_ARG_POS, NULL, NULL);
        if (return_value == IRIDIUM_SUCCESSFUL)
        {
            // Compute Checksum
            uint16_t checksum = ComputeHalfWordCheckSum((uint8_t *)tx_msg, IRIDIUM_SDB_TX_MSG_SIZE);
            (void)(checksum);
            tx_msg[IRIDIUM_SDB_TX_MSG_SIZE] = (uint8_t)((0xff00u & checksum) >> 8u);
            tx_msg[IRIDIUM_SDB_TX_MSG_SIZE + 1u] = (uint8_t)(0x00ffu & checksum);

            // Send the message
            halStatus_t test_hal = UartWrite(iridium_inst->uart_inst, (uartMsg_t *)tx_msg, (IRIDIUM_SDB_TX_MSG_SIZE + IRIDIUM_CHECKSUM_SIZE));
            if (test_hal == GEN_HAL_SUCCESSFUL)
            {
                // Check the answer
                char answer[AT_MSG_MAX_SIZE] = {0};
                uint32_t answer_size = 0u;
                return_value = IridiumGetAnswer(iridium_inst, answer, &answer_size);
                if ((return_value == IRIDIUM_SUCCESSFUL) && (answer_size != 0u) && (answer[0] == '0'))
                {
                    return_value = IRIDIUM_SUCCESSFUL;
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
 * @fn          IridiumSBDSendData(iridiumInst_t *iridium_inst)
 * @brief       This function starts the transfer of data contained in the transceiver to the Iridium constellation
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_TIMEOUT if uart read or write has timeouted
 * @retval      #IRIDIUM_ERROR if an error has been encountered
 * @retval      #IRIDIUM_SUCCESSFUL
 */
static iridiumStatus_t IridiumSBDSendData(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        // Send the message to the buffer
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_INIT_SESSION_EXT, AT_CMD_SBD_INIT_SESSION_EXT_SIZE,
                                          NULL, 0u, 0u, NULL, NULL);
    }
    else
    {
        return_value = IRIDIUM_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumSendCommand(iridiumInst_t *iridium_inst, const char *command, uint8_t command_size, const char *arg, uint8_t arg_size, uint8_t arg_pos, char *answer, uint8_t answer_size)
 * @brief       Function that sends an AT command to the Iridium transceiver and get the answer (if any)
 * @param[in]   iridium_inst    Iridium instance used by the driver
 * @param[in]   command         Command to be send (include dummy arg)
 * @param[in]   command_size    Size of the command (include arg)
 * @param[in]   arg             Argument to replace in the command
 * @param[in]   arg_size        Size of the argument
 * @param[in]   arg_pos         Position of the argument in the function
 * @param[in]   answer          Answer of the command
 * @param[in]   answer_size     Size of the answer
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_TIMEOUT if uart read or write has timeouted
 * @retval      #IRIDIUM_ERROR if the command has encountered an error
 * @retval      #IRIDIUM_SUCCESSFUL else
 */
static iridiumStatus_t IridiumSendCommand(iridiumInst_t *iridium_inst, const char *command, uint8_t command_size,
                                          const char *arg, uint8_t arg_size, uint8_t arg_pos,
                                          char *answer, uint32_t *answer_size)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if ((iridium_inst != NULL) && (command != NULL) && (command_size != 0u))
    {
        // Setup the message
        uint8_t at_tx_msg[AT_MSG_MAX_SIZE] = {0};
        (void)memcpy(&at_tx_msg[0], command, command_size);

        // Set the argument
        if ((arg_size != 0u) && (arg != NULL))
        {
            (void)memcpy(&at_tx_msg[arg_pos], arg, arg_size);
        }

        // Send the message
        halStatus_t test_hal = UartWrite(iridium_inst->uart_inst, at_tx_msg, command_size);
        if (test_hal == GEN_HAL_SUCCESSFUL)
        {
            // Check if an answer is required
            if ((answer != NULL) && (answer_size != NULL))
            {
                // First Get Answer
                return_value = IridiumGetAnswer(iridium_inst, answer, answer_size);
                if (return_value == IRIDIUM_SUCCESSFUL)
                {
                    // Get a Check for ACk.
                    // Currently because there is no interrupt if the
                    // answer is too long we could miss the ACK, that is why
                    // function return is not checked
                    (void)IridiumCheckAck(iridium_inst);
                }
            }
            else
            {
                return_value = IridiumCheckAck(iridium_inst);
            }
        }
        else if (test_hal == GEN_HAL_TIMEOUT)
        {
            return_value = IRIDIUM_TIMEOUT;
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
 * @fn          IridiumGetAnswer(iridiumInst_t *iridium_inst, char *answer, uint32_t *answer_size)
 * @brief       Function that get answer of the AT command
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @param[out]  answer Answer of the command
 * @param[out]  answer_size Size of the answer
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_TIMEOUT if uart read or write has timeouted
 * @retval      #IRIDIUM_ERROR if the transceiver answered something else than OK
 * @retval      #IRIDIUM_SUCCESSFUL if an ok is received
 */
static iridiumStatus_t IridiumGetAnswer(iridiumInst_t *iridium_inst, char *answer, uint32_t *answer_size)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if ((iridium_inst != NULL))
    {
        // First Read UART
        uint8_t at_rx_msg[AT_MSG_MAX_SIZE] = {0};
        uint32_t tickstart = HalGetTick();
        halStatus_t test_hal = GEN_HAL_TIMEOUT;
        while ((test_hal == GEN_HAL_TIMEOUT) && ((HalGetTick() - tickstart) < IRIDIUM_MAX_TIMEOUT))
        {
            test_hal = UartRead(iridium_inst->uart_inst, at_rx_msg, AT_MSG_MAX_SIZE);
        }
        if (test_hal == GEN_HAL_SUCCESSFUL)
        {
            // Start the parsing
            uint32_t i = 0u;
            // Look for useless begining of the frame (if verbose)
            if ((at_rx_msg[i] == (uint8_t)'\r') && (at_rx_msg[i + 1u] == (uint8_t)'\n'))
            {
                i += 2u; // Skip those characters if any
            }

            // Check if there is at least one readable character
            if (at_rx_msg[i] != 0u)
            {
                // Save the start index of the answer
                uint32_t answer_start = i;

                // Now look for the end of the answer
                while ((at_rx_msg[i] != (uint8_t)'\r') && (at_rx_msg[i] != 0u) && (i < AT_MSG_MAX_SIZE))
                {
                    i++;
                }

                // Save the stop index
                uint32_t answer_stop = i;
                if (answer_stop != answer_start)
                {
                    (void)memcpy(answer, &at_rx_msg[answer_start], (answer_stop - answer_start));
                    *answer_size = answer_stop - answer_start;
                }
                else
                {
                    *answer_size = 0u;
                }
            }
            else
            {
                return_value = IRIDIUM_ERROR;
            }

            // Get answer
            (void)(answer);
            (void)(answer_size);
        }
        else if (test_hal == GEN_HAL_TIMEOUT)
        {
            return_value = IRIDIUM_TIMEOUT;
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
 * @fn          IridiumCheckAck(iridiumInst_t *iridium_inst)
 * @brief       Check if the transceiver answered to the command and forward the answer if any
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @retval      #IRIDIUM_INVALID_PARAM if there is a null pointer
 * @retval      #IRIDIUM_TIMEOUT if uart read or write has timeouted
 * @retval      #IRIDIUM_ERROR if the transceiver answered something else than OK
 * @retval      #IRIDIUM_SUCCESSFUL if an ok is received
 */
static iridiumStatus_t IridiumCheckAck(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    iridiumStatus_t return_value = IRIDIUM_SUCCESSFUL;

    // Function Core
    if ((iridium_inst != NULL))
    {
        // First Read UART
        uint8_t at_rx_msg[AT_MSG_MAX_SIZE] = {0};
        uint32_t tickstart = HalGetTick();
        halStatus_t test_hal = GEN_HAL_TIMEOUT;
        while ((test_hal == GEN_HAL_TIMEOUT) && ((HalGetTick() - tickstart) < IRIDIUM_MAX_TIMEOUT))
        {
            test_hal = UartRead(iridium_inst->uart_inst, at_rx_msg, AT_MSG_MAX_SIZE);
        }
        if (test_hal == GEN_HAL_SUCCESSFUL)
        {
            // Start the parsing
            uint32_t i = 0u;
            // Look for useless begining of the frame (if verbose)
            if ((at_rx_msg[i] == (uint8_t)'\r') && (at_rx_msg[i + 1u] == (uint8_t)'\n'))
            {
                i += 2u; // Skip those characters if any
            }

            // Check if there is at least one readable character
            if (at_rx_msg[i] != 0u)
            {
                // Check for ERROR or OK
                if ((strncmp((char *)&at_rx_msg[i], AT_OK_ANSWER, AT_OK_ANSWER_SIZE) == 0) ||
                    (strncmp((char *)&at_rx_msg[i], AT_NUMERIC_OK_ANSWER, AT_NUMERIC_OK_ANSWER_SIZE) == 0))
                {
                    return_value = IRIDIUM_SUCCESSFUL;
                }
                else if (strncmp((char *)&at_rx_msg[i], AT_READY_ANSWER, AT_READY_ANSWER_SIZE) == 0)
                {
                    return_value = IRIDIUM_SUCCESSFUL;
                }
                else if ((strncmp((char *)&at_rx_msg[i], AT_ERROR_ANSWER, AT_ERROR_ANSWER_SIZE) == 0) ||
                         (strncmp((char *)&at_rx_msg[i], AT_NUMERIC_ERROR_ANSWER, AT_NUMERIC_ERROR_ANSWER_SIZE) == 0))
                {
                    return_value = IRIDIUM_ERROR;
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
        else if (test_hal == GEN_HAL_TIMEOUT)
        {
            return_value = IRIDIUM_TIMEOUT;
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
 * @fn          ConvertUint16FromASCII(const char ascii_array[ARRAY_MAX_SIZE_UINT16])
 * @brief       This function convert an ascii array containing an UINT16
 * @param[in]   ascii_array Array containing the UINT16
 * @return      uint16_t
 */
static uint16_t ConvertUint16FromASCII(const char ascii_array[ARRAY_MAX_SIZE_UINT16])
{
    // Variables Initialisation
    uint16_t number = 0u;

    // First make an exception for -1 because why not AT protocol
    if ((ascii_array[0u] == '-') && (ascii_array[1u] == '1'))
    {
        number = 0u;
    }
    else
    {
        // Convert ASCII array
        uint32_t i = 0u;
        while ((i < ARRAY_MAX_SIZE_UINT16) && (ascii_array[i] != '\0'))
        {
            if ((ascii_array[i] >= '0') && (ascii_array[i] <= '9'))
            {
                number = (number * 10u) + (ascii_array[i] - '0');
            }
            i++;
        }
    }

    return number;
}

/**
 * @fn          ComputeHalfWordCheckSum(const uint8_t *data, uint32_t size)
 * @brief       This function cmputes a half word checksum from data
 * @param[in]   data Pointer to the data from which the checksum is computed
 * @param[in]   size Size of the data
 * @return      uint16_t
 */
static uint16_t ComputeHalfWordCheckSum(const uint8_t *data, uint32_t size)
{
    // Variables Initialisation
    uint16_t checksum = 0u;

    // Function Core
    for (uint32_t i = 0; i < size; i++)
    {
        checksum += data[i];
    }

    return checksum;
}