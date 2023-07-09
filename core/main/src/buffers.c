/**
 * @file buffers.c
 * @author Merlin Kooshmanian
 * @brief Source file defining tasks
 * @date 21/04/2023
 *
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include "buffers.h"
#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

extern void UsageFault_Handler(void);

/************************** Variable Definitions *****************************/

extern bufferStaticConf_t g_buffers_static_conf[NB_BUFFERS];
extern bufferDynamicConf_t g_buffers_dynamic_conf[NB_BUFFERS];
extern taskDynamicConf_t g_tasks_dynamic_conf[NB_TASKS];

/************************* Functions Definitions *****************************/

/**
 * @fn      createBuffers(void)
 * @brief   Function that creates buffers
 * @retval  #BUFFER_SUCCESSFUL if buffers creation successful
 * @retval  #BUFFER_INVALID_PARAM if at least one buffer configuration is invalid
 */
bufferStatus_t createBuffers(void)
{
    // Variable Initialisation
    bufferStatus_t return_value = BUFFER_SUCCESSFUL;
    bufferRef_t buffer = 0;

    // Function
    while (buffer < NB_BUFFERS && return_value == BUFFER_SUCCESSFUL)
    {
        g_buffers_dynamic_conf[buffer].id = osMessageQueueNew(g_buffers_static_conf[buffer].max_nb, g_buffers_static_conf[buffer].max_size, NULL);
        if (g_buffers_dynamic_conf[buffer].id == NULL)
        {
            return_value = BUFFER_INVALID_PARAM;
        }
        buffer++;
    }

    return (return_value);
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
 * @retval      #BUFFER_ERROR if writing fails
 *
 * This function does not support timeout.
 */
bufferStatus_t WriteBuffer(bufferRef_t buffer, bufferMsgAddr_t msg, bufferSize_t length)
{
    // Variable Initialisation
    bufferStatus_t return_value = BUFFER_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (buffer < NB_BUFFERS || msg == NULL || length == 0 || length > g_buffers_static_conf[buffer].max_size)
    {
        if (g_tasks_dynamic_conf[g_buffers_static_conf[buffer].sender].id == osThreadGetId() || g_buffers_static_conf[buffer].sender == ANY_TASK_REF)
        {
            test_value = osMessageQueuePut(g_buffers_dynamic_conf[buffer].id, msg, 0u, 0u);
            switch (test_value)
            {
            case osOK:
                g_buffers_dynamic_conf[buffer].nb_msg++;
                return_value = BUFFER_SUCCESSFUL;
                break;
            case osErrorResource:
                return_value = BUFFER_FULL;
                break;
            default:
                return_value = BUFFER_ERROR;
                break;
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

    return (return_value);
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
 * @retval      #BUFFER_ERROR if reading fails
 *
 * This function does not support timeout.
 */
bufferStatus_t ReadBuffer(bufferRef_t buffer, bufferMsgAddr_t msg, bufferSize_t length)
{
    // Variable Initialisation
    bufferStatus_t return_value = BUFFER_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (buffer < NB_BUFFERS || msg == NULL || length == 0 || length > g_buffers_static_conf[buffer].max_size)
    {
        if (g_tasks_dynamic_conf[g_buffers_static_conf[buffer].receiver].id == osThreadGetId() || g_buffers_static_conf[buffer].receiver == ANY_TASK_REF)
        {
            test_value = osMessageQueueGet(g_buffers_dynamic_conf[buffer].id, msg, NULL, 0);
            switch (test_value)
            {
            case osOK:
                g_buffers_dynamic_conf[buffer].nb_msg--;
                return_value = BUFFER_SUCCESSFUL;
                break;
            case osErrorResource:
                return_value = BUFFER_EMPTY;
                break;
            default:
                return_value = BUFFER_ERROR;
                break;
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

    return (return_value);
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
    if (buffer < NB_BUFFERS || count != NULL)
    {
        if (g_tasks_dynamic_conf[g_buffers_static_conf[buffer].receiver].id == osThreadGetId() || g_buffers_static_conf[buffer].receiver == ANY_TASK_REF)
        {
            *count = osMessageQueueGetCount(g_buffers_dynamic_conf[buffer].id);
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

    return (return_value);
}
