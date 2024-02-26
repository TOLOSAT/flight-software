/**
 * @file    housekeeping.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for housekeeping messages
 * @date    22/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "core_basics.h"
#include "housekeeping.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          SendHousekeepingMessage(housekeepingMessage_t *housekeeping_message, bufferRef_t buffer_ref)
 * @brief       Function that sends a housekeeping message toward SALAMI (right buffer has to be chosen)
 * @param[in]   housekeeping_message Housekeeping message that will be sent
 * @param[in]   buffer_ref Buffer in which the message is put
 * @retval      HOUSEKEEPING_INVALID_PARAM if the message is a null pointer
 * @retval      HOUSEKEEPING_ERROR if an error has been encountered
 * @retval      HOUSEKEEPING_SUCCESSFUL else
 */
housekeepingStatus_t SendHousekeepingMessage(housekeepingMessage_t *housekeeping_message, bufferRef_t buffer_ref)
{
    // Variable Initialisation
    housekeepingStatus_t return_val = HOUSEKEEPING_SUCCESSFUL;

    // Function Core
    if (housekeeping_message != NULL)
    {
        bufferStatus_t test_val = WriteBuffer(buffer_ref, housekeeping_message, HOUSEKEEPING_MESSAGE_SIZE);
        if (test_val != BUFFER_SUCCESSFUL)
        {
            return_val = HOUSEKEEPING_ERROR;
        }
    }
    else
    {
        return_val = HOUSEKEEPING_INVALID_PARAM;
    }

    return return_val;
}

/**
 * @fn          ReceiveHousekeepingMessage(housekeepingMessage_t *housekeeping_message, bufferRef_t buffer_ref)
 * @brief       Function that receives a housekeeping message (right buffer has to be chosen)
 * @param[out]  housekeeping_message Housekeeping message that will be received
 * @param[in]   buffer_ref Buffer from which the message is received
 * @retval      HOUSEKEEPING_INVALID_PARAM if the message is a null pointer
 * @retval      HOUSEKEEPING_TIMEOUT if there is no housekeeping message available
 * @retval      HOUSEKEEPING_ERROR if an error has been encountered
 * @retval      HOUSEKEEPING_SUCCESSFUL else
 */
housekeepingStatus_t ReceiveHousekeepingMessage(housekeepingMessage_t *housekeeping_message, bufferRef_t buffer_ref)
{
    // Variable Initialisation
    housekeepingStatus_t return_val = HOUSEKEEPING_SUCCESSFUL;

    // Function Core
    if (housekeeping_message != NULL)
    {
        bufferStatus_t test_val = ReadBuffer(buffer_ref, housekeeping_message, HOUSEKEEPING_MESSAGE_SIZE);
        if (test_val != BUFFER_SUCCESSFUL)
        {
            if (test_val == BUFFER_EMPTY)
            {
                return_val = HOUSEKEEPING_TIMEOUT;
            }
            else
            {
                return_val = HOUSEKEEPING_ERROR;
            }
        }
    }
    else
    {
        return_val = HOUSEKEEPING_INVALID_PARAM;
    }

    return return_val;
}

/**
 * @fn          ConvertHousekeepingMessageIntoReport(housekeepingMessage_t *housekeeping_message, housekeepingReport_t *housekeeping_report)
 * @brief       Function that convert an housekeeping message into an housekeeping report (PUS)
 * @param[in]   housekeeping_message Housekeeping Message
 * @param[out]  housekeeping_report Housekeeping Report (PUS)
 * @retval      HOUSEKEEPING_INVALID_PARAM if the message is a null pointer
 * @retval      HOUSEKEEPING_SUCCESSFUL else
 */
housekeepingStatus_t ConvertHousekeepingMessageIntoReport(housekeepingMessage_t *housekeeping_message, housekeepingReport_t *housekeeping_report)
{
    // Variable Initialisation
    housekeepingStatus_t return_val = HOUSEKEEPING_SUCCESSFUL;

    // Function Core
    if ((housekeeping_message != NULL) && (housekeeping_report != NULL))
    {
        housekeeping_report->HKID = (uint32_t)(housekeeping_message->task_ref << 8u) + (uint32_t)(housekeeping_message->observable);
        (void)memcpy((void *)housekeeping_report->data, (void *)housekeeping_message->value, HOUSEKEEPING_DATA_SIZE);
    }
    else
    {
        return_val = HOUSEKEEPING_INVALID_PARAM;
    }

    return return_val;
}