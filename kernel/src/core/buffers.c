/**
 * @file    buffers.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining buffers
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core/buffers.h"
#include "core/tasks.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void UsageFault_Handler(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      CreateBuffers(void)
 * @brief   Function that creates buffers
 * @retval  #RET_SUCCESSFUL if buffers creation successful
 * @retval  #RET_ERROR if at least one buffer creation failed
 */
returnCode_t CreateBuffers(void)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    bufferNo_t buffer = 0;

    // Function
    while ((buffer < (bufferNo_t)NB_BUFFERS) && (return_value == RET_SUCCESSFUL))
    {
        g_buffers_desc_table[buffer].handle = xQueueCreateStatic(g_buffers_conf[buffer].max_nb, g_buffers_conf[buffer].max_size, g_buffers_conf[buffer].p_buffer_array, g_buffers_conf[buffer].p_buffer_entity);
        if (g_buffers_desc_table[buffer].handle == NULL)
        {
            return_value = RET_ERROR;
        }
        buffer++;
    }

    return return_value;
}

/**
 * @fn          BufferWrite(bufferNo_t buffer, data_t data, length_t length)
 * @brief       Function that send a message in a buffer
 * @param[in]   buffer  Reference of the buffer (in BUFFERS_ENUM)
 * @param[in]   data    Message that will be written in the buffer
 * @param[in]   length  Size of the message that will be written in the buffer
 * @retval      #RET_SUCCESSFUL if writing in the buffer is successful
 * @retval      #RET_INVALID_PARAM if buffer does not exist or the current task is not the sender
 * @retval      #RET_ERROR if the buffer reached its maximum capacity (last message not written)
 *
 * This function does not support timeout.
 */
returnCode_t BufferWrite(bufferNo_t buffer, data_t data, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    BaseType_t test_value;

    // Function Core
    if ((buffer < (bufferNo_t)NB_BUFFERS) || (data == NULL) || (length == 0u))
    {
        if ((length > g_buffers_conf[buffer].max_size) || (g_tasks_desc_table[g_buffers_conf[buffer].sender].handle == xTaskGetCurrentTaskHandle()) || (g_buffers_conf[buffer].sender == ANY_TASK_REF))
        {
            test_value = xQueueSendToBack(g_buffers_desc_table[buffer].handle, data, 0u);
            if (test_value == pdTRUE)
            {
                g_buffers_desc_table[buffer].nb_msg++;
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
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BufferRead(bufferNo_t buffer, data_t data, length_t length)
 * @brief       Function that read a message in a buffer
 * @param[in]   buffer  Reference of the buffer (in BUFFERS_ENUM)
 * @param[out]  data    Message that will be read in the buffer
 * @param[in]   length  Size of the message that will be read in the buffer
 * @retval      #RET_SUCCESSFUL if reading in the buffer is successful
 * @retval      #RET_INVALID_PARAM if buffer does not exist or the current task is not the receiver
 * @retval      #RET_NOT_AVAILABLE if there is no message in the buffer currently
 *
 * This function does not support timeout.
 */
returnCode_t BufferRead(bufferNo_t buffer, data_t data, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    BaseType_t test_value;

    // Function Core
    if ((buffer < (bufferNo_t)NB_BUFFERS) || (data == NULL) || (length == 0u))
    {
        if ((length > g_buffers_conf[buffer].max_size) || (g_tasks_desc_table[g_buffers_conf[buffer].receiver].handle == xTaskGetCurrentTaskHandle()) || (g_buffers_conf[buffer].receiver == ANY_TASK_REF))
        {
            test_value = xQueueReceive(g_buffers_desc_table[buffer].handle, data, 0);
            if (test_value == pdTRUE)
            {
                g_buffers_desc_table[buffer].nb_msg--;
            }
            else
            {
                return_value = RET_NOT_AVAILABLE;
            }
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetBufferCount(bufferNo_t buffer, length_t *count)
 * @brief       Function that read how many messages there is in a buffer
 * @param[in]   buffer  Reference of the buffer (in BUFFERS_ENUM)
 * @param[out]  count   How many message there is in the buffer
 * @retval      #RET_SUCCESSFUL if reading buffer capacity is successful
 * @retval      #RET_INVALID_PARAM if buffer does not exist or the current task is not the receiver
 */
returnCode_t GetBufferCount(bufferNo_t buffer, length_t *count)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((buffer < (bufferNo_t)NB_BUFFERS) || (count != NULL))
    {
        if ((g_tasks_desc_table[g_buffers_conf[buffer].receiver].handle == xTaskGetCurrentTaskHandle()) || (g_buffers_conf[buffer].receiver == ANY_TASK_REF))
        {
            *count = uxQueueMessagesWaiting(g_buffers_desc_table[buffer].handle);
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
