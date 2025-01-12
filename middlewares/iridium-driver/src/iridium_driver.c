/**
 * @file    iridium_driver.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Iridium Driver
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "kernel.h"
#include "iridium_driver.h"
#include "iridium_constants.h"

/***************************** Macros Definitions ****************************/

#define ASCII_NUMBER_OFFSET 0x30u /**< Correspond to the '0' character */
#define ARRAY_MAX_SIZE_UINT16 6u  /**< Correspond to the "64535" size plus one for margin */

/*************************** Functions Declarations **************************/

// Init static function

static returnCode_t IridiumCheckPresence(iridiumInst_t *iridium_inst);
static returnCode_t IridiumSetupHW(iridiumInst_t *iridium_inst);
static returnCode_t IridiumGetSerialNumber(iridiumInst_t *iridium_inst);
static returnCode_t IridiumSaveConf(iridiumInst_t *iridium_inst);

// SBD related function

static returnCode_t IridiumSetupSBD(iridiumInst_t *iridium_inst);
static returnCode_t IridiumParseSBDStatus(char *msg, uint32_t msg_length, iridiumSBDStatus_t *status);
static returnCode_t IridiumSBDPutDataInBuffer(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t tx_msg);
static returnCode_t IridiumSBDSendData(iridiumInst_t *iridium_inst);

// Generic static function

static returnCode_t IridiumSendCommand(iridiumInst_t *iridium_inst, const char *command, uint8_t command_size,
                                          const char *arg, uint8_t arg_size, uint8_t arg_pos,
                                          char *answer, uint32_t *answer_size);
static returnCode_t IridiumParseAnswer(const char *msg, uint32_t size, char *answer, uint32_t *answer_size);
static returnCode_t IridiumParseAck(const char *msg, uint32_t size);

// Miscellaneous

static uint16_t ConvertUint16FromASCII(const char ascii_array[ARRAY_MAX_SIZE_UINT16]);
static uint16_t ComputeHalfWordCheckSum(const uint8_t *data, uint32_t size);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              IridiumStart(iridiumInst_t *iridium_inst)
 * @brief           Start a connection with the Iridium transceiver
 * @param[in,out]   iridium_inst Iridium instance used by the driver
 * @retval          #RET_INVALID_PARAM if there is a null pointer
 * @retval          #RET_TIMEOUT if uart read or write has timeouted
 * @retval          #RET_ERROR if an error occured during the initialisation
 * @retval          #RET_SUCCESSFUL if the start procedure went well
 */
returnCode_t IridiumStart(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        // First CheckPresence
        return_value = IridiumCheckPresence(iridium_inst);

        // Continue if a transceiver is detected
        if (return_value == RET_SUCCESSFUL)
        {
            iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_INIT;
            // Setup the transceiver
            return_value = IridiumSetupHW(iridium_inst);

            // Continue if the HW setup went well
            if (return_value == RET_SUCCESSFUL)
            {
                // Then get info from the transceiver
                return_value = IridiumGetSerialNumber(iridium_inst);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Then setup SBD
                    return_value = IridiumSetupSBD(iridium_inst);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        // Finally save the conf
                        return_value = IridiumSaveConf(iridium_inst);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_READY;
                        }
                        else
                        {
                            iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_ERROR;
                            return_value = RET_ERROR;
                        }
                    }
                    else
                    {
                        iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_ERROR;
                        return_value = RET_ERROR;
                    }
                }
                else
                {
                    iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_ERROR;
                    return_value = RET_ERROR;
                }
            }
            else
            {
                iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_ERROR;
                return_value = RET_ERROR;
            }
        }
        else
        {
            iridium_inst->iridium_state = IRIDIUM_TRANSCEIVER_OFF;
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumSendSDB(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t tx_msg)
 * @brief       This function sends a message through SBD
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @param[in]   tx_msg Message to be sent
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if uart read or write has timeouted
 * @retval      #RET_ERROR if an error occured during the discussion with the transceiver or before the call
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t IridiumSendSDB(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t tx_msg)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((iridium_inst != NULL) && (tx_msg != NULL))
    {
        // Check if the transceiver is available
        if (iridium_inst->iridium_state == IRIDIUM_TRANSCEIVER_READY)
        {
            // First get the SBD Status
            iridiumSBDStatus_t sbd_status = {0};
            return_value = IridiumGetSBDStatus(iridium_inst, &sbd_status);
            if ((return_value == RET_SUCCESSFUL) &&
                (sbd_status.network_availability >= iridium_inst->minimum_availability))
            {
                if (sbd_status.tx_message_presence == IRIDIUM_SBD_MSG_NOT_PRESENT)
                {
                    return_value = IridiumSBDPutDataInBuffer(iridium_inst, tx_msg);
                    if (return_value == RET_SUCCESSFUL)
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
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumGetNetworkAvailability(iridiumInst_t *iridium_inst, iridiumNetworkAvailability_t *availability)
 * @brief       Function that get the iridium network availability.
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @param[out]  availability Availability of the network
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if uart read or write has timeouted
 * @retval      #RET_ERROR if an error occured during the discussion with the transceiver or before the call
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t IridiumGetNetworkAvailability(iridiumInst_t *iridium_inst, iridiumNetworkAvailability_t *availability)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

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
            if ((return_value == RET_SUCCESSFUL) && (answer_size != 0u) && (answer_size >= AT_CMD_SIGNAL_QUALITY_ANS_MIN_SIZE))
            {
                *availability = (iridiumNetworkAvailability_t)(answer[AT_CMD_SIGNAL_QUALITY_ANSWER_OFFSET] - ASCII_NUMBER_OFFSET);
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumGetSBDStatus(iridiumInst_t *iridium_inst, iridiumSBDStatus_t *status)
 * @brief       This function gets the relevant information for the SBD (network availability, message presence, ...)
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @param[out]  status Struct including all the relevant information for the SBD
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if uart read or write has timeouted
 * @retval      #RET_ERROR if an error occured during the discussion with the transceiver or before the call
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t IridiumGetSBDStatus(iridiumInst_t *iridium_inst, iridiumSBDStatus_t *status)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

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
            if ((return_value == RET_SUCCESSFUL) && (answer_size != 0u) && (answer_size >= AT_CMD_SIGNAL_QUALITY_ANS_MIN_SIZE))
            {
                status->network_availability = (iridiumNetworkAvailability_t)(answer[AT_CMD_SIGNAL_QUALITY_ANSWER_OFFSET] - ASCII_NUMBER_OFFSET);

                // Then get SDB Status
                return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_GET_STATUS_EXT, AT_CMD_SBD_GET_STATUS_EXT_SIZE,
                                                  NULL, 0u, 0u, answer, &answer_size);
                if ((return_value == RET_SUCCESSFUL) && (answer_size != 0u) && (answer_size >= AT_CMD_SBD_GET_STAT_EXT_ANS_MIN_SIZE))
                {
                    return_value = IridiumParseSBDStatus(answer, answer_size, status);
                }
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumCheckPresence(iridiumInst_t *iridium_inst)
 * @brief       Check if an Iridium transceiver is present
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if uart read or write has timeouted
 * @retval      #RET_ERROR if there is no iridium transceiver or uart is not working
 * @retval      #RET_SUCCESSFUL if an iridium transceiver is detected
 */
static returnCode_t IridiumCheckPresence(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_EMPTY, AT_CMD_EMPTY_SIZE,
                                          NULL, 0u, 0u, NULL, NULL);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumSetupHW(iridiumInst_t *iridium_inst)
 * @brief       This function sets up the transceiver.
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if uart read or write has timeouted
 * @retval      #RET_ERROR if an error has been encountered when setting up the hardware
 * @retval      #RET_SUCCESSFUL
 */
static returnCode_t IridiumSetupHW(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        // First set baudrate
        char baudrate = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_BAUDRATE_MASK) >> HW_CTRL_REG_BAUDRATE_POS) + ASCII_NUMBER_OFFSET;
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_SET_BAUDRATE, AT_CMD_SET_BAUDRATE_SIZE,
                                          &baudrate, 1u, AT_CMD_SET_BAUDRATE_ARG_POS, NULL, NULL);
        if (return_value == RET_SUCCESSFUL)
        {
            // Then set hardware control flow mode
            char ctrl_flow_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_CTRL_FLOW_MODE_MASK) >> HW_CTRL_REG_CTRL_FLOW_MODE_POS) + ASCII_NUMBER_OFFSET;
            return_value = IridiumSendCommand(iridium_inst, AT_CMD_SET_FLOW_CTRL, AT_CMD_SET_FLOW_CTRL_SIZE,
                                              &ctrl_flow_mode, 1u, AT_CMD_SET_FLOW_CTRL_ARG_POS, NULL, NULL);
            if (return_value == RET_SUCCESSFUL)
            {
                // Then set hardware DTR mode
                char dtr_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_DTR_MODE_MASK) >> HW_CTRL_REG_DTR_MODE_POS) + ASCII_NUMBER_OFFSET;
                return_value = IridiumSendCommand(iridium_inst, AT_CMD_SET_DTR, AT_CMD_SET_DTR_SIZE,
                                                  &dtr_mode, 1u, AT_CMD_SET_DTR_ARG_POS, NULL, NULL);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Then set hardware echo mode
                    char echo_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_ECHO_MODE_MASK) >> HW_CTRL_REG_ECHO_MODE_POS) + ASCII_NUMBER_OFFSET;
                    return_value = IridiumSendCommand(iridium_inst, AT_CMD_ECHO, AT_CMD_ECHO_SIZE,
                                                      &echo_mode, 1u, AT_CMD_ECHO_ARG_POS, NULL, NULL);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        // Then set ring alert mode
                        char ring_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_RING_MODE_MASK) >> HW_CTRL_REG_RING_MODE_POS) + ASCII_NUMBER_OFFSET;
                        return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_SET_MSG_RX_ALERT, AT_CMD_SBD_SET_MSG_RX_ALERT_SIZE,
                                                          &ring_mode, 1u, AT_CMD_SBD_SET_MSG_RX_ALERT_ARG_POS, NULL, NULL);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // Then set quiet mode
                            char quiet_mode = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_QUIET_MASK) >> HW_CTRL_REG_QUIET_POS) + ASCII_NUMBER_OFFSET;
                            return_value = IridiumSendCommand(iridium_inst, AT_CMD_QUIET_MODE, AT_CMD_QUIET_MODE_SIZE,
                                                              &quiet_mode, 1u, AT_CMD_QUIET_MODE_ARG_POS, NULL, NULL);
                            if (return_value == RET_SUCCESSFUL)
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
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              IridiumGetSerialNumber(iridiumInst_t *iridium_inst)
 * @brief           Function that get serial number from the transceiver
 * @param[in,out]   iridium_inst Iridium instance used by the driver
 * @retval          #RET_INVALID_PARAM if there is a null pointer
 * @retval          #RET_TIMEOUT if uart read or write has timeouted
 * @retval          #RET_ERROR if an error has been encountered when getting info
 * @retval          #RET_SUCCESSFUL
 */
static returnCode_t IridiumGetSerialNumber(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        char answer[AT_MSG_MAX_SIZE] = {0};
        uint32_t answer_size = 0u;
        // Get the serial number
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_GET_SERIAL_NB, AT_CMD_GET_SERIAL_NB_SIZE,
                                          NULL, 0u, 0u, answer, &answer_size);
        if ((return_value == RET_SUCCESSFUL) && (answer_size != 0u))
        {
            // Update Serial Number
            (void)memcpy(iridium_inst->serial_number, answer, answer_size);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumSetupSBD(iridiumInst_t *iridium_inst)
 * @brief       Function that setup the transceiver for SBD
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if uart read or write has timeouted
 * @retval      #RET_ERROR if an error has been encountered when setting up SBD
 * @retval      #RET_SUCCESSFUL
 */
static returnCode_t IridiumSetupSBD(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        char answer[AT_MSG_MAX_SIZE] = {0};
        uint32_t answer_size = 0u;
        // First clear Mobile Originated Message Sequence Number
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_CLEAR_MSG_SEQ_NB, AT_CMD_SBD_CLEAR_MSG_SEQ_NB_SIZE,
                                          NULL, 0u, 0u, answer, &answer_size);
        if ((return_value == RET_SUCCESSFUL) && (answer_size != 0u) && (answer[AT_NUMERIC_ANSWER_CHAR_OFFSET] == AT_NUMERIC_OK_ANSWER[AT_NUMERIC_ANSWER_CHAR_OFFSET]))
        {
            // Then clear all buffers
            char clear_buffer_sel = '2';
            return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_CLEAR_MSG_BUFF, AT_CMD_SBD_CLEAR_MSG_BUFF_SIZE,
                                              &clear_buffer_sel, 1u, AT_CMD_SBD_CLEAR_MSG_BUFF_ARG_POS, answer, &answer_size);
            if ((return_value == RET_SUCCESSFUL) && (answer_size != 0u) && (answer[AT_NUMERIC_ANSWER_CHAR_OFFSET] == AT_NUMERIC_OK_ANSWER[AT_NUMERIC_ANSWER_CHAR_OFFSET]))
            {
                char timeout = ((iridium_inst->hw_ctrl_reg & HW_CTRL_REG_SBD_TIMEOUT_MASK) >> HW_CTRL_REG_SBD_TIMEOUT_POS) + ASCII_NUMBER_OFFSET;
                return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_SET_TIMEOUT, AT_CMD_SBD_SET_TIMEOUT_SIZE,
                                                  &timeout, 1u, AT_CMD_SBD_SET_TIMEOUT_ARG_POS, NULL, NULL);
            }
            else
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumSaveConf(iridiumInst_t *iridium_inst)
 * @brief       Function that save the current configuration in non volatile memory of the transceiver
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if uart read or write has timeouted
 * @retval      #RET_ERROR if an error has been encountered when setting up SBD
 * @retval      #RET_SUCCESSFUL
 */
static returnCode_t IridiumSaveConf(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        // Save the conf in the profil 0
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_WRITE_CONF, AT_CMD_WRITE_CONF_SIZE,
                                          "0", 1u, AT_CMD_WRITE_CONF_ARG_POS, NULL, NULL);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumParseSBDStatus(char *msg, uint32_t msg_length, iridiumSBDStatus_t *status)
 * @brief       This function takes the answer of the iridium transceiver containing the status and update the status variable
 * @param[in]   msg             Answer message given by the iridium transceiver (in ASCII)
 * @param[in]   msg_length      Size of the answer
 * @param[out]  status          SBD status (parsed from the message)
 * @retval      #RET_INVALID_PARAM if there is a null pointer or message length is too small
 * @retval      #RET_ERROR if an error occured
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t IridiumParseSBDStatus(char *msg, uint32_t msg_length, iridiumSBDStatus_t *status)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

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
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumSBDPutDataInBuffer(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t tx_msg)
 * @brief       This function puts a message in the TX buffer of the transceiver
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @param[in]   tx_msg Message that will be sent
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if uart read or write has timeouted
 * @retval      #RET_ERROR if an error has been encountered
 * @retval      #RET_SUCCESSFUL
 */
static returnCode_t IridiumSBDPutDataInBuffer(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t tx_msg)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((iridium_inst != NULL) && (tx_msg != NULL))
    {
        // Send the message to the buffer
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_WRITE_BIN_DATA, AT_CMD_SBD_WRITE_BIN_DATA_SIZE + 2u,
                                          IRIDIUM_SDB_TX_MSG_SIZE_ASCII, IRIDIUM_SDB_TX_MSG_SIZE_ASCII_SIZE, AT_CMD_SBD_WRITE_BIN_DATA_ARG_POS, NULL, NULL);
        if (return_value == RET_SUCCESSFUL)
        {
            // Init Messages
            uint16_t checksum = ComputeHalfWordCheckSum((uint8_t *)tx_msg, IRIDIUM_SDB_TX_MSG_SIZE);
            tx_msg[IRIDIUM_SDB_TX_MSG_SIZE] = (uint8_t)((0xff00u & checksum) >> 8u);
            tx_msg[IRIDIUM_SDB_TX_MSG_SIZE + 1u] = (uint8_t)(0x00ffu & checksum);
            uint8_t at_rx_msg[AT_MSG_MAX_SIZE] = {0};

            // Send the message
            return_value = DeviceWrite(iridium_inst->dev_uart, (data_t)tx_msg, (IRIDIUM_SDB_TX_MSG_SIZE + IRIDIUM_CHECKSUM_SIZE));
            if (return_value == RET_SUCCESSFUL)
            {
                // Check the answer
                return_value = DeviceRead(iridium_inst->dev_uart, at_rx_msg, AT_MSG_MAX_SIZE);
                if (return_value == RET_SUCCESSFUL)
                {
                    return_value = IridiumParseAck((char *)at_rx_msg, AT_MSG_MAX_SIZE);
                }
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumSBDSendData(iridiumInst_t *iridium_inst)
 * @brief       This function starts the transfer of data contained in the transceiver to the Iridium constellation
 * @param[in]   iridium_inst Iridium instance used by the driver
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if uart read or write has timeouted
 * @retval      #RET_ERROR if an error has been encountered
 * @retval      #RET_SUCCESSFUL
 */
static returnCode_t IridiumSBDSendData(iridiumInst_t *iridium_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (iridium_inst != NULL)
    {
        // Send the message to the buffer
        return_value = IridiumSendCommand(iridium_inst, AT_CMD_SBD_INIT_SESSION_EXT, AT_CMD_SBD_INIT_SESSION_EXT_SIZE,
                                          NULL, 0u, 0u, NULL, NULL);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
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
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if uart read or write has timeouted
 * @retval      #RET_ERROR if the command has encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t IridiumSendCommand(iridiumInst_t *iridium_inst, const char *command, uint8_t command_size,
                                          const char *arg, uint8_t arg_size, uint8_t arg_pos,
                                          char *answer, uint32_t *answer_size)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((iridium_inst != NULL) && (command != NULL) && (command_size != 0u))
    {
        // Setup messages
        uint8_t at_tx_msg[AT_MSG_MAX_SIZE] = {0};
        uint8_t at_rx_msg[AT_MSG_MAX_SIZE] = {0};

        // Set the command
        (void)memcpy(&at_tx_msg[0], command, command_size);

        // Set the argument
        if ((arg_size != 0u) && (arg != NULL))
        {
            (void)memcpy(&at_tx_msg[arg_pos], arg, arg_size);
        }

        // Send the message
        return_value = DeviceWrite(iridium_inst->dev_uart, at_tx_msg, command_size);
        if (return_value == RET_SUCCESSFUL)
        {
            // Check if an answer is required
            if ((answer != NULL) && (answer_size != NULL))
            {
                // First Get Answer
                return_value = DeviceRead(iridium_inst->dev_uart, at_rx_msg, AT_MSG_MAX_SIZE);
                if (return_value == RET_SUCCESSFUL)
                {
                    return_value = IridiumParseAnswer((char *)at_rx_msg, AT_MSG_MAX_SIZE, answer, answer_size);
                }

                // Note : ACK is discarded by the OBC because if we get the message no error occured
            }
            else
            {
                // Get ACK directly
                return_value = DeviceRead(iridium_inst->dev_uart, at_rx_msg, AT_MSG_MAX_SIZE);
                if (return_value == RET_SUCCESSFUL)
                {
                    return_value = IridiumParseAck((char *)at_rx_msg, AT_MSG_MAX_SIZE);
                }
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumParseAnswer(const char *msg, uint32_t size, char *answer, uint32_t *answer_size)
 * @brief       Function that get answer of the AT command
 * @param[in]   msg Message send by the transceiver
 * @param[in]   size Size of the message
 * @param[out]  answer Answer of the command
 * @param[out]  answer_size Size of the answer
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if uart read or write has timeouted
 * @retval      #RET_ERROR if the transceiver answered something else than OK
 * @retval      #RET_SUCCESSFUL if an ok is received
 */
static returnCode_t IridiumParseAnswer(const char *msg, uint32_t size, char *answer, uint32_t *answer_size)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((msg != NULL) && (size != 0u))
    {
        // Start the parsing
        uint32_t i = 0u;
        // Look for useless begining of the frame (if verbose)
        if ((msg[i] == '\r') && (msg[i + 1u] == '\n'))
        {
            i += 2u; // Skip those characters if any
        }

        // Check if there is at least one readable character
        if (msg[i] != '\0')
        {
            // Save the start index of the answer
            uint32_t answer_start = i;

            // Now look for the end of the answer
            while ((msg[i] != '\r') && (msg[i] != '\0') && (i < AT_MSG_MAX_SIZE))
            {
                i++;
            }

            // Save the stop index
            uint32_t answer_stop = i;
            if (answer_stop != answer_start)
            {
                (void)memcpy(answer, &msg[answer_start], (answer_stop - answer_start));
                *answer_size = answer_stop - answer_start;
            }
            else
            {
                *answer_size = 0u;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }

        // Get answer
        (void)(answer);
        (void)(answer_size);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IridiumParseAck(const char *msg, uint32_t size)
 * @brief       Check if the transceiver answered to the command and forward the answer if any
 * @param[in]   msg Message send by the transceiver
 * @param[in]   size Size of the message
 * @retval      #RET_INVALID_PARAM if there is a null pointer
 * @retval      #RET_TIMEOUT if uart read or write has timeouted
 * @retval      #RET_ERROR if the transceiver answered something else than OK
 * @retval      #RET_SUCCESSFUL if an ok is received
 */
static returnCode_t IridiumParseAck(const char *msg, uint32_t size)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((msg != NULL) && (size != 0u))
    {
        // Start the parsing
        uint32_t i = 0u;
        // Look for useless begining of the frame (if verbose)
        if ((msg[i] == '\r') && (msg[i + 1u] == '\n'))
        {
            i += 2u; // Skip those characters if any
        }

        // Check if there is at least one readable character
        if (msg[i] != '\0')
        {
            // Check for ERROR or OK
            if ((strncmp(&msg[i], AT_OK_ANSWER, AT_OK_ANSWER_SIZE) == 0) ||
                (strncmp(&msg[i], AT_NUMERIC_OK_ANSWER, AT_NUMERIC_OK_ANSWER_SIZE) == 0))
            {
                return_value = RET_SUCCESSFUL;
            }
            else if (strncmp(&msg[i], AT_READY_ANSWER, AT_READY_ANSWER_SIZE) == 0)
            {
                return_value = RET_SUCCESSFUL;
            }
            else if ((strncmp(&msg[i], AT_ERROR_ANSWER, AT_ERROR_ANSWER_SIZE) == 0) ||
                     (strncmp(&msg[i], AT_NUMERIC_ERROR_ANSWER, AT_NUMERIC_ERROR_ANSWER_SIZE) == 0))
            {
                return_value = RET_ERROR;
            }
            else
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
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