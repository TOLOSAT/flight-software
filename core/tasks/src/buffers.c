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

extern bufferDef_t  g_buffers[NB_BUFFERS];
extern bufferId_t   g_buffers_ids[NB_BUFFERS];

/************************* Functions Definitions *****************************/

/**
 * @fn      bufferStatus_t createBuffers(void)
 * @brief   Function that creates buffers
 * @param   void
 * @retval  BUFFERS_SUCCESSFUL
 */
bufferStatus_t createBuffers(void){
    // Variable Initialisation
    bufferStatus_t return_value = BUFFERS_SUCCESSFUL;
    bufferRef_t buffer = 0;

    // Function
    while(buffer < NB_BUFFERS && return_value == BUFFERS_SUCCESSFUL){
        g_buffers_ids[buffer] = osMessageQueueNew(g_buffers[buffer].buffer_max_nb,g_buffers[buffer].buffer_max_size,&g_buffers[buffer].buffer_attribute);
        if(g_buffers_ids[buffer] == NULL){
            return_value = BUFFERS_INVALID_PARAM;
        }
        buffer++;
    }

    return(return_value);
}

/**
 * @fn      bufferStatus_t WriteBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length)
 * @brief   Function that send a message in a buffer
 * @param   buffer
 * @param   msg
 * @param   length
 * @retval  BUFFERS_SUCCESSFUL
 * 
 * This function does not support timeout.
 */
bufferStatus_t WriteBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length){
    // Variable Initialisation
    bufferStatus_t return_value = BUFFERS_SUCCESSFUL;
    osStatus_t test_value = osOK;

    // Function Core
    if(buffer < NB_BUFFERS || msg == NULL || length == 0 || length > g_buffers[buffer].buffer_max_size)
    {
        test_value = osMessageQueuePut(g_buffers_ids[buffer], msg, 0u, 0u);
        switch (test_value)
        {
            case osOK:
                return_value = BUFFERS_SUCCESSFUL;
                break;
            case osErrorResource:
                return_value = BUFFERS_FULL;
                break;
            default:
                return_value = BUFFERS_ERROR;
                break;
        }
    }
    else
    {
        return_value = BUFFERS_INVALID_PARAM;
    }

    return(return_value);
}

/**
 * @fn      bufferStatus_t ReadBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length)
 * @brief   Function that read a message in a buffer
 * @param   buffer
 * @param   msg
 * @param   length
 * @retval  BUFFERS_SUCCESSFUL
 * 
 * This function does not support timeout.
 */
bufferStatus_t ReadBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length){
    // Variable Initialisation
    bufferStatus_t return_value = BUFFERS_SUCCESSFUL;
    osStatus_t test_value = osOK;

    // Function Core
    if(buffer < NB_BUFFERS || msg == NULL || length == 0 || length > g_buffers[buffer].buffer_max_size)
    {
        test_value = osMessageQueueGet(g_buffers_ids[buffer], msg, NULL, 0);
        switch (test_value)
        {
            case osOK:
                return_value = BUFFERS_SUCCESSFUL;
                break;
            case osErrorResource:
                return_value = BUFFERS_EMPTY;
                break;
            default:
                return_value = BUFFERS_ERROR;
                break;
        }
    }
    else
    {
        return_value = BUFFERS_INVALID_PARAM;
    }

    return(return_value);
}