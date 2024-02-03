/**
 * @file    events.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for events messages
 * @date    22/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "events.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          SendEventMessage(eventMessage_t *event_message, bufferRef_t buffer_ref)
 * @brief       Function that sends a event message toward SALAMI (right buffer has to be chosen)
 * @param[in]   event_message Event message that will be sent
 * @param[in]   buffer_ref Buffer in which the message is put
 * @retval      EVENT_INVALID_PARAM if the message is a null pointer
 * @retval      EVENT_ERROR if an error has been encountered
 * @retval      EVENT_SUCCESSFUL else
 */
eventStatus_t SendEventMessage(eventMessage_t *event_message, bufferRef_t buffer_ref)
{
    // Variable Initialisation
    eventStatus_t return_val = EVENT_SUCCESSFUL;

    // Function Core
    if (event_message != NULL)
    {
        bufferStatus_t test_val = WriteBuffer(buffer_ref, event_message, EVENT_MESSAGE_SIZE);
        if (test_val != BUFFER_SUCCESSFUL)
        {
            return_val = EVENT_ERROR;
        }
    }
    else
    {
        return_val = EVENT_INVALID_PARAM;
    }

    return return_val;
}

/**
 * @fn          ReceiveEventMessage(eventMessage_t *event_message, bufferRef_t buffer_ref)
 * @brief       Function that receives a event message (right buffer has to be chosen)
 * @param[out]  event_message Event message that will be received
 * @param[in]   buffer_ref Buffer from which the message is received
 * @retval      EVENT_INVALID_PARAM if the message is a null pointer
 * @retval      EVENT_TIMEOUT if there is no event message available
 * @retval      EVENT_ERROR if an error has been encountered
 * @retval      EVENT_SUCCESSFUL else
 */
eventStatus_t ReceiveEventMessage(eventMessage_t *event_message, bufferRef_t buffer_ref)
{
    // Variable Initialisation
    eventStatus_t return_val = EVENT_SUCCESSFUL;

    // Function Core
    if (event_message != NULL)
    {
        bufferStatus_t test_val = ReadBuffer(buffer_ref, event_message, EVENT_MESSAGE_SIZE);
        if (test_val != BUFFER_SUCCESSFUL)
        {
            if (test_val == BUFFER_EMPTY)
            {
                return_val = EVENT_TIMEOUT;
            }
            else
            {
                return_val = EVENT_ERROR;
            }
        }
    }
    else
    {
        return_val = EVENT_INVALID_PARAM;
    }

    return return_val;
}

/**
 * @fn          ConvertEventMessageIntoReport(eventMessage_t *event_message, eventReport_t *event_report)
 * @brief       Function that convert an event message into an event report (PUS)
 * @param[in]   event_message Event Message
 * @param[out]  event_report Event Report (PUS)
 * @retval      EVENT_INVALID_PARAM if the message is a null pointer
 * @retval      EVENT_SUCCESSFUL else
 */
eventStatus_t ConvertEventMessageIntoReport(eventMessage_t *event_message, eventReport_t *event_report)
{
    // Variable Initialisation
    eventStatus_t return_val = EVENT_SUCCESSFUL;

    // Function Core
    if ((event_message != NULL) && (event_report != NULL))
    {
        event_report->EID = (uint32_t)(event_message->task_ref << 16u) + (uint32_t)(event_message->event_type << 8u) + (uint32_t)(event_message->event_subtype);
        (void)memcpy((void *)event_report->data, (void *)&event_message->time, EVENT_DATA_SIZE);
    }
    else
    {
        return_val = EVENT_INVALID_PARAM;
    }

    return return_val;
}