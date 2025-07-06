/**
 * @file    pus193.c
 * @author  Yann Awbi
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 193 (Iridium) functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "kernel.h"
#include "tm_management.h"
#include "iridium_driver.h"
#include "pus193.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

iridiumInst_t *iridium_inst = NULL;

/*************************** Functions Definitions ***************************/

/**
 * @fn          ExecuteS193SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that initialises the Iridium transceiver after receiving a S193SS1 TC
 * @param[in]   tc S191SS1 TC that initialises the Iridium transceiver
 * @param[out]  tm Not used here
 * @param[out]  error_code Indicates which error has been encountered
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS193SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(tc);
    (void)(tm);

    // Check parameter(s)
    if (error_code != NULL)
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Start Iridium Iridium Driver
        return_value = IridiumStart(iridium_inst);
        if (return_value != RET_SUCCESSFUL)
        {
            *error_code  = PUS_EXECUTION_FAILED;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS193SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that stops the Iridium transceiver after receiving a S193SS2 TC
 * @param[in]   tc S161SS2 TC that requests the stop of the transciever
 * @param[out]  tm Not used here
 * @param[out]  error_code Indicates which error has been encountered
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS193SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(tc);
    (void)(tm);

    // Check parameter(s)
    if (error_code != NULL)
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Start Iridium Iridium Driver
        return_value = IridiumStop(iridium_inst);
        if (return_value != RET_SUCCESSFUL)
        {
            *error_code = PUS_EXECUTION_FAILED;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S193SS4 TM (Iridium SBD status) when requested by a S193SS3
 * @param[in]   tc S161SS3 TC that requests this TM
 * @param[out]  tm S161SS4 TM that we will send
 * @param[out]  error_code Indicates which error has been encountered
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS193SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(tc);

    // Check parameter(s)
    if (error_code != NULL)
    {
        iridiumSBDStatus_t status = { 0 };

        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // GetStatus
        return_value = IridiumGetSBDStatus(iridium_inst, &status);
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = BuildTM(tm, 193u, 4u, (data_t)&status, sizeof(iridiumSBDStatus_t));
            if (return_value != RET_SUCCESSFUL)
            {
                *error_code = PUS_EXECUTION_FAILED;
            }
        }
        else
        {
            *error_code = PUS_EXECUTION_FAILED;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS193SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send a SBD when receiving a S161SS5
 * @param[in]   tc S193SS5 TC that request an SBD message and include the message to send
 * @param[out]  tm Not used here
 * @param[out]  error_code Indicates which error has been encountered
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS193SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(tm);

    // Check parameter(s)
    if (error_code != NULL)
    {
        iridiumSBDTxMsg_t tx_msg = {0};
        uint16_t msg_size = 0u;

        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Get message size
        msg_size = tc->spp_header.packet_data_length +1u - TC_HEADER_SIZE - CRC_TRAILER_SIZE;

        // Get tx_message from tc
        (void)memcpy(tx_msg, tc->data, msg_size);
        
        // Send the message
        return_value = IridiumSendSBD(iridium_inst, tx_msg);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}