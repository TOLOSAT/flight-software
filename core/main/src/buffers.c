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

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

extern void UsageFault_Handler(void);

/************************** Variable Definitions *****************************/

extern bufferDef_t g_buffers_conf[NB_BUFFERS];
extern bufferId_t g_buffers_ids[NB_BUFFERS];

/************************* Functions Definitions *****************************/

/**
 * @fn      createBuffers(void)
 * @brief   Function that creates buffers
 * @retval  BUFFER_SUCCESSFUL if buffers creation successful
 * @retval  BUFFER_INVALID_PARAM if at least one buffer configuration is invalid
 */
bufferStatus_t createBuffers(void)
{
    // Variable Initialisation
    bufferStatus_t return_value = BUFFER_SUCCESSFUL;
    bufferRef_t buffer = 0;

    // Function
    while (buffer < NB_BUFFERS && return_value == BUFFER_SUCCESSFUL)
    {
        g_buffers_ids[buffer] = osMessageQueueNew(g_buffers_conf[buffer].buffer_max_nb, g_buffers_conf[buffer].buffer_max_size, &g_buffers_conf[buffer].buffer_attribute);
        if (g_buffers_ids[buffer] == NULL)
        {
            return_value = BUFFER_INVALID_PARAM;
        }
        buffer++;
    }

    return (return_value);
}

/**
 * @fn      WriteBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length)
 * @brief   Function that send a message in a buffer
 * @param   buffer Reference of the buffer (in BUFFERS_ENUM)
 * @param   msg Message that will be written in the buffer
 * @param   length Size of the message that will be written in the buffer
 * @retval  BUFFER_SUCCESSFUL if writing in the buffer is successful
 * @retval  BUFFER_INVALID_PARAM if buffer does not exist
 * @retval  BUFFER_FULL if the buffer reached it's maximum number of message (last message not written)
 * @retval  BUFFER_ERROR if writing fails
 *
 * This function does not support timeout.
 */
bufferStatus_t WriteBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length)
{
    // Variable Initialisation
    bufferStatus_t return_value = BUFFER_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (buffer < NB_BUFFERS || msg == NULL || length == 0 || length > g_buffers_conf[buffer].buffer_max_size)
    {
        test_value = osMessageQueuePut(g_buffers_ids[buffer], msg, 0u, 0u);
        switch (test_value)
        {
        case osOK:
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

    return (return_value);
}

/**
 * @fn      ReadBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length)
 * @brief   Function that read a message in a buffer
 * @param   buffer Reference of the buffer (in BUFFERS_ENUM)
 * @param   msg Message that will be read in the buffer
 * @param   length Size of the message that will be read in the buffer
 * @retval  BUFFER_SUCCESSFUL if reading in the buffer is successful
 * @retval  BUFFER_INVALID_PARAM if buffer does not exist
 * @retval  BUFFER_EMPTY if there is no message in the buffer currently
 * @retval  BUFFER_ERROR if reading fails
 *
 * This function does not support timeout.
 */
bufferStatus_t ReadBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length)
{
    // Variable Initialisation
    bufferStatus_t return_value = BUFFER_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (buffer < NB_BUFFERS || msg == NULL || length == 0 || length > g_buffers_conf[buffer].buffer_max_size)
    {
        test_value = osMessageQueueGet(g_buffers_ids[buffer], msg, NULL, 0);
        switch (test_value)
        {
        case osOK:
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

    return (return_value);
}