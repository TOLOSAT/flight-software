/**
 * @file    buffers.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining buffers
 * @date    21/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core_basics.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void UsageFault_Handler(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      CreateBuffers(void)
 * @brief   Function that creates buffers
 * @retval  #CORE_SUCCESSFUL if buffers creation successful
 * @retval  #CORE_ERROR if at least one buffer creation failed
 */
coreStatus_t IN_CORE_TEXT_SECTION CreateBuffers(void)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    bufferRef_t buffer = 0;

    // Function
    while ((buffer < (bufferRef_t)NB_BUFFERS) && (return_value == CORE_SUCCESSFUL))
    {
        g_buffer_desc_table[buffer].handle = xQueueCreateStatic(g_buffers_conf[buffer].max_nb, g_buffers_conf[buffer].max_size, g_buffer_desc_table[buffer].buffer_data, &g_buffer_desc_table[buffer].buffer_entity);
        if (g_buffer_desc_table[buffer].handle == NULL)
        {
            return_value = CORE_ERROR;
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
 * @retval      #CORE_SUCCESSFUL if writing in the buffer is successful
 * @retval      #CORE_INVALID_PARAM if buffer does not exist or the current task is not the sender
 * @retval      #CORE_TIMEOUT if the buffer reached it's maximum number of message (last message not written)
 *
 * This function does not support timeout.
 */
coreStatus_t IN_CORE_TEXT_SECTION WriteBuffer(bufferRef_t buffer, bufferMsgAddr_t msg, bufferSize_t length)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    BaseType_t test_value;

    // Function Core
    if ((buffer < (bufferRef_t)NB_BUFFERS) || (msg == NULL) || (length == 0u))
    {
        if ((length > g_buffers_conf[buffer].max_size) || (g_task_desc_table[g_buffers_conf[buffer].sender].handle == xTaskGetCurrentTaskHandle()) || (g_buffers_conf[buffer].sender == ANY_TASK_REF))
        {
            test_value = xQueueSendToBack(g_buffer_desc_table[buffer].handle, msg, 0u);
            if (test_value == pdTRUE)
            {
                g_buffer_desc_table[buffer].nb_msg++;
            }
            else
            {
                return_value = CORE_TIMEOUT;
            }
        }
        else
        {
            return_value = CORE_INVALID_PARAM;
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ReadBuffer(bufferRef_t buffer, bufferMsgAddr_t msg, bufferSize_t length)
 * @brief       Function that read a message in a buffer
 * @param[in]   buffer Reference of the buffer (in BUFFERS_ENUM)
 * @param[out]  msg Message that will be read in the buffer
 * @param[in]   length Size of the message that will be read in the buffer
 * @retval      #CORE_SUCCESSFUL if reading in the buffer is successful
 * @retval      #CORE_INVALID_PARAM if buffer does not exist or the current task is not the receiver
 * @retval      #CORE_TIMEOUT if there is no message in the buffer currently
 *
 * This function does not support timeout.
 */
coreStatus_t IN_CORE_TEXT_SECTION ReadBuffer(bufferRef_t buffer, bufferMsgAddr_t msg, bufferSize_t length)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    BaseType_t test_value;

    // Function Core
    if ((buffer < (bufferRef_t)NB_BUFFERS) || (msg == NULL) || (length == 0u))
    {
        if ((length > g_buffers_conf[buffer].max_size) || (g_task_desc_table[g_buffers_conf[buffer].receiver].handle == xTaskGetCurrentTaskHandle()) || (g_buffers_conf[buffer].receiver == ANY_TASK_REF))
        {
            test_value = xQueueReceive(g_buffer_desc_table[buffer].handle, msg, 0);
            if (test_value == pdTRUE)
            {
                g_buffer_desc_table[buffer].nb_msg--;
            }
            else
            {
                return_value = CORE_TIMEOUT;
            }
        }
        else
        {
            return_value = CORE_INVALID_PARAM;
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetBufferCount(bufferRef_t buffer, bufferDepth_t *count)
 * @brief       Function that read how many messages there is in a buffer
 * @param[in]   buffer Reference of the buffer (in BUFFERS_ENUM)
 * @param[out]  count How many message there is in the buffer
 * @retval      #CORE_SUCCESSFUL if reading buffer capacity is successful
 * @retval      #CORE_INVALID_PARAM if buffer does not exist or the current task is not the receiver
 */
coreStatus_t IN_CORE_TEXT_SECTION GetBufferCount(bufferRef_t buffer, bufferDepth_t *count)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if ((buffer < (bufferRef_t)NB_BUFFERS) || (count != NULL))
    {
        if ((g_task_desc_table[g_buffers_conf[buffer].receiver].handle == xTaskGetCurrentTaskHandle()) || (g_buffers_conf[buffer].receiver == ANY_TASK_REF))
        {
            *count = uxQueueMessagesWaiting(g_buffer_desc_table[buffer].handle);
        }
        else
        {
            return_value = CORE_INVALID_PARAM;
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}
