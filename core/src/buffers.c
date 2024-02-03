/**
 * @file    buffers.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining buffers
 * @date    21/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "buffers.h"
#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void UsageFault_Handler(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      CreateBuffers(void)
 * @brief   Function that creates buffers
 * @retval  #BUFFER_SUCCESSFUL if buffers creation successful
 * @retval  #BUFFER_ERROR if at least one buffer creation failed
 */
bufferStatus_t CreateBuffers(void)
{
    // Variable Initialisation
    bufferStatus_t return_value = BUFFER_SUCCESSFUL;
    bufferRef_t buffer = 0;

    // Function
    while ((buffer < (bufferRef_t)NB_BUFFERS) && (return_value == BUFFER_SUCCESSFUL))
    {
        g_buffers_dynamic_conf[buffer].handle = xQueueCreate(g_buffers_static_conf[buffer].max_nb, g_buffers_static_conf[buffer].max_size);
        if (g_buffers_dynamic_conf[buffer].handle == NULL)
        {
            return_value = BUFFER_ERROR;
        }
        buffer++;
    }

    return return_value;
}

/**
 * @fn          WriteBuffer(bufferRef_t buffer, bufferMsgAddr_t msg, bufferSize_t length)
 * @brief       Function that send a message in a buffer
 * @param[in]   buffer Reference of the buffer (in BUFFERS_ENUM)
 * @param[in]   msg Message that will be written in the buffer
 * @param[in]   length Size of the message that will be written in the buffer
 * @retval      #BUFFER_SUCCESSFUL if writing in the buffer is successful
 * @retval      #BUFFER_INVALID_PARAM if buffer does not exist or the current task is not the sender
 * @retval      #BUFFER_FULL if the buffer reached it's maximum number of message (last message not written)
 *
 * This function does not support timeout.
 */
bufferStatus_t WriteBuffer(bufferRef_t buffer, bufferMsgAddr_t msg, bufferSize_t length)
{
    // Variable Initialisation
    bufferStatus_t return_value = BUFFER_SUCCESSFUL;
    BaseType_t test_value;

    // Function Core
    if ((buffer < (bufferRef_t)NB_BUFFERS) || (msg == NULL) || (length == 0u))
    {
        if ((length > g_buffers_static_conf[buffer].max_size) || (g_tasks_dynamic_conf[g_buffers_static_conf[buffer].sender].handle == xTaskGetCurrentTaskHandle()) || (g_buffers_static_conf[buffer].sender == ANY_TASK_REF))
        {
            test_value = xQueueSendToBack(g_buffers_dynamic_conf[buffer].handle, msg, 0u);
            if (test_value == pdTRUE)
            {
                g_buffers_dynamic_conf[buffer].nb_msg--;
            }
            else
            {
                return_value = BUFFER_EMPTY;
            }
        }
        else
        {
            return_value = BUFFER_INVALID_PARAM;
        }
    }
    else
    {
        return_value = BUFFER_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ReadBuffer(bufferRef_t buffer, bufferMsgAddr_t msg, bufferSize_t length)
 * @brief       Function that read a message in a buffer
 * @param[in]   buffer Reference of the buffer (in BUFFERS_ENUM)
 * @param[out]  msg Message that will be read in the buffer
 * @param[in]   length Size of the message that will be read in the buffer
 * @retval      #BUFFER_SUCCESSFUL if reading in the buffer is successful
 * @retval      #BUFFER_INVALID_PARAM if buffer does not exist or the current task is not the receiver
 * @retval      #BUFFER_EMPTY if there is no message in the buffer currently
 *
 * This function does not support timeout.
 */
bufferStatus_t ReadBuffer(bufferRef_t buffer, bufferMsgAddr_t msg, bufferSize_t length)
{
    // Variable Initialisation
    bufferStatus_t return_value = BUFFER_SUCCESSFUL;
    BaseType_t test_value;

    // Function Core
    if ((buffer < (bufferRef_t)NB_BUFFERS) || (msg == NULL) || (length == 0u))
    {
        if ((length > g_buffers_static_conf[buffer].max_size) || (g_tasks_dynamic_conf[g_buffers_static_conf[buffer].receiver].handle == xTaskGetCurrentTaskHandle()) || (g_buffers_static_conf[buffer].receiver == ANY_TASK_REF))
        {
            test_value = xQueueReceive(g_buffers_dynamic_conf[buffer].handle, msg, 0);
            if (test_value == pdTRUE)
            {
                g_buffers_dynamic_conf[buffer].nb_msg++;
            }
            else
            {
                return_value = BUFFER_EMPTY;
            }
        }
        else
        {
            return_value = BUFFER_INVALID_PARAM;
        }
    }
    else
    {
        return_value = BUFFER_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetBufferCount(bufferRef_t buffer, bufferDepth_t *count)
 * @brief       Function that read how many messages there is in a buffer
 * @param[in]   buffer Reference of the buffer (in BUFFERS_ENUM)
 * @param[out]  count How many message there is in the buffer
 * @retval      #BUFFER_SUCCESSFUL if reading buffer capacity is successful
 * @retval      #BUFFER_INVALID_PARAM if buffer does not exist or the current task is not the receiver
 */
bufferStatus_t GetBufferCount(bufferRef_t buffer, bufferDepth_t *count)
{
    // Variable Initialisation
    bufferStatus_t return_value = BUFFER_SUCCESSFUL;

    // Function Core
    if ((buffer < (bufferRef_t)NB_BUFFERS) || (count != NULL))
    {
        if ((g_tasks_dynamic_conf[g_buffers_static_conf[buffer].receiver].handle == xTaskGetCurrentTaskHandle()) || (g_buffers_static_conf[buffer].receiver == ANY_TASK_REF))
        {
            *count = uxQueueMessagesWaiting(g_buffers_dynamic_conf[buffer].handle);
        }
        else
        {
            return_value = BUFFER_INVALID_PARAM;
        }
    }
    else
    {
        return_value = BUFFER_INVALID_PARAM;
    }

    return return_value;
}
