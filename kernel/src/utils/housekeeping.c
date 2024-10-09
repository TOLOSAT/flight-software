/**
 * @file    housekeeping.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for housekeeping
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "utils/housekeeping.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          SendHkMessage(hkMessage_t *hk_message, bufferNo_t buffer)
 * @brief       Function that sends a housekeeping message toward SALAMI (right buffer has to be chosen)
 * @param[in]   hk_message  Housekeeping message that will be sent
 * @param[in]   buffer      Buffer in which the message is put
 * @retval      #RET_INVALID_PARAM if the message is a null pointer
 * @retval      #RET_NOT_AVAILABLE if HK buffer has reached its maximum capacity
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SendHkMessage(hkMessage_t *hk_message, bufferNo_t buffer)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (hk_message != NULL)
    {
        return_value = BufferWrite(buffer, (data_t)hk_message, HK_MESSAGE_SIZE);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ReceiveHkMessage(hkMessage_t *hk_message, bufferNo_t buffer)
 * @brief       Function that receives a housekeeping message (right buffer has to be chosen)
 * @param[out]  hk_message  Housekeeping message that will be received
 * @param[in]   buffer      Buffer from which the message is received
 * @retval      #RET_INVALID_PARAM if the message is a null pointer
 * @retval      #RET_NOT_AVAILABLE if there is no housekeeping message available
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ReceiveHkMessage(hkMessage_t *hk_message, bufferNo_t buffer)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (hk_message != NULL)
    {
        return_value = BufferRead(buffer, (data_t)hk_message, HK_MESSAGE_SIZE);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
