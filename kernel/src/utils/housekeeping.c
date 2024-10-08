/**
 * @file    housekeeping.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for housekeeping
 * @date    22/10/2023
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
 * @param[in]   hk_message Housekeeping message that will be sent
 * @param[in]   buffer Buffer in which the message is put
 * @retval      #RET_INVALID_PARAM if the message is a null pointer
 * @retval      #RET_ERROR if an error has been encountered
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SendHkMessage(hkMessage_t *hk_message, bufferNo_t buffer)
{
    // Variable Initialisation
    returnCode_t return_val = RET_SUCCESSFUL;

    // Function Core
    if (hk_message != NULL)
    {
        returnCode_t test_val = BufferWrite(buffer, (data_t)hk_message, HK_MESSAGE_SIZE);
        if (test_val != RET_SUCCESSFUL)
        {
            return_val = RET_ERROR;
        }
    }
    else
    {
        return_val = RET_INVALID_PARAM;
    }

    return return_val;
}

/**
 * @fn          ReceiveHkMessage(hkMessage_t *hk_message, bufferNo_t buffer)
 * @brief       Function that receives a housekeeping message (right buffer has to be chosen)
 * @param[out]  hk_message Housekeeping message that will be received
 * @param[in]   buffer Buffer from which the message is received
 * @retval      #RET_INVALID_PARAM if the message is a null pointer
 * @retval      #RET_TIMEOUT if there is no housekeeping message available
 * @retval      #RET_ERROR if an error has been encountered
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ReceiveHkMessage(hkMessage_t *hk_message, bufferNo_t buffer)
{
    // Variable Initialisation
    returnCode_t return_val = RET_SUCCESSFUL;

    // Function Core
    if (hk_message != NULL)
    {
        returnCode_t test_val = BufferRead(buffer, (data_t)hk_message, HK_MESSAGE_SIZE);
        if (test_val != RET_SUCCESSFUL)
        {
            if (test_val == RET_TIMEOUT)
            {
                return_val = RET_TIMEOUT;
            }
            else
            {
                return_val = RET_ERROR;
            }
        }
    }
    else
    {
        return_val = RET_INVALID_PARAM;
    }

    return return_val;
}
