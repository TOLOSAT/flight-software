/**
 * @file    life.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for life messages
 * @date    22/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "life.h"
#include "core.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          SendLifeMessage(lifeMessage_t *life_message, bufferNo_t buffer)
 * @brief       Function that sends a life message toward SALAMI (right buffer has to be chosen)
 * @param[in]   life_message Life message that will be sent
 * @param[in]   buffer Buffer in which the message is put
 * @retval      #CORE_INVALID_PARAM if the message is a null pointer
 * @retval      #CORE_ERROR if an error has been encountered
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION SendLifeMessage(lifeMessage_t *life_message, bufferNo_t buffer)
{
    // Variable Initialisation
    coreStatus_t return_val = CORE_SUCCESSFUL;

    // Function Core
    if (life_message != NULL)
    {
        coreStatus_t test_val = WriteBuffer(buffer, life_message, LIFE_MESSAGE_SIZE);
        if (test_val != CORE_SUCCESSFUL)
        {
            return_val = CORE_ERROR;
        }
    }
    else
    {
        return_val = CORE_INVALID_PARAM;
    }

    return return_val;
}

/**
 * @fn          ReceiveLifeMessage(lifeMessage_t *life_message, bufferNo_t buffer)
 * @brief       Function that receives a life message (right buffer has to be chosen)
 * @param[out]  life_message Life message that will be received
 * @param[in]   buffer Buffer from which the message is received
 * @retval      #CORE_INVALID_PARAM if the message is a null pointer
 * @retval      #CORE_TIMEOUT if there is no life message available
 * @retval      #CORE_ERROR if an error has been encountered
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION ReceiveLifeMessage(lifeMessage_t *life_message, bufferNo_t buffer)
{
    // Variable Initialisation
    coreStatus_t return_val = CORE_SUCCESSFUL;

    // Function Core
    if (life_message != NULL)
    {
        coreStatus_t test_val = ReadBuffer(buffer, life_message, LIFE_MESSAGE_SIZE);
        if (test_val != CORE_SUCCESSFUL)
        {
            if (test_val == CORE_TIMEOUT)
            {
                return_val = CORE_TIMEOUT;
            }
            else
            {
                return_val = CORE_ERROR;
            }
        }
    }
    else
    {
        return_val = CORE_INVALID_PARAM;
    }

    return return_val;
}