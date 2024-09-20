/**
 * @file    events.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for events messages
 * @date    22/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "com/events.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          SendEventMessage(eventMessage_t *event_message, bufferNo_t buffer)
 * @brief       Function that sends a event message toward SALAMI (right buffer has to be chosen)
 * @param[in]   event_message Event message that will be sent
 * @param[in]   buffer Buffer in which the message is put
 * @retval      #KERNEL_INVALID_PARAM if the message is a null pointer
 * @retval      #KERNEL_ERROR if an error has been encountered
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION SendEventMessage(eventMessage_t *event_message, bufferNo_t buffer)
{
    // Variable Initialisation
    kernelStatus_t return_val = KERNEL_SUCCESSFUL;

    // Function Core
    if (event_message != NULL)
    {
        kernelStatus_t test_val = WriteBuffer(buffer, event_message, EVENT_MESSAGE_SIZE);
        if (test_val != KERNEL_SUCCESSFUL)
        {
            return_val = KERNEL_ERROR;
        }
    }
    else
    {
        return_val = KERNEL_INVALID_PARAM;
    }

    return return_val;
}

/**
 * @fn          ReceiveEventMessage(eventMessage_t *event_message, bufferNo_t buffer)
 * @brief       Function that receives a event message (right buffer has to be chosen)
 * @param[out]  event_message Event message that will be received
 * @param[in]   buffer Buffer from which the message is received
 * @retval      #KERNEL_INVALID_PARAM if the message is a null pointer
 * @retval      #KERNEL_TIMEOUT if there is no event message available
 * @retval      #KERNEL_ERROR if an error has been encountered
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION ReceiveEventMessage(eventMessage_t *event_message, bufferNo_t buffer)
{
    // Variable Initialisation
    kernelStatus_t return_val = KERNEL_SUCCESSFUL;

    // Function Core
    if (event_message != NULL)
    {
        kernelStatus_t test_val = ReadBuffer(buffer, event_message, EVENT_MESSAGE_SIZE);
        if (test_val != KERNEL_SUCCESSFUL)
        {
            if (test_val == KERNEL_TIMEOUT)
            {
                return_val = KERNEL_TIMEOUT;
            }
            else
            {
                return_val = KERNEL_ERROR;
            }
        }
    }
    else
    {
        return_val = KERNEL_INVALID_PARAM;
    }

    return return_val;
}
