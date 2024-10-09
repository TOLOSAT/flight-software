/**
 * @file    mutex.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining mutexes
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core/mutex.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      CreateMutexes(void)
 * @brief   Function that creates all mutexes
 * @retval  #RET_SUCCESSFUL if creation succeed
 * @retval  #RET_ERROR if at least one task creation failed
 */
returnCode_t CreateMutexes(void)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    mutexNo_t mutex = 0;

    // Function Core
    while ((mutex < (mutexNo_t)NB_MUTEXES) && (return_value == RET_SUCCESSFUL))
    {
        g_mutexes_desc_table[mutex].handle = xSemaphoreCreateMutexStatic(g_mutex_conf_table[mutex].p_queue);
        if (g_mutexes_desc_table[mutex].handle == NULL)
        {
            return_value = RET_ERROR;
        }
        mutex++;
    }

    return return_value;
}

/**
 * @fn          AcquireMutex(mutexNo_t mutex)
 * @brief       Function that acquires the mutex.
 * @param[in]   mutex   Mutex reference number as defined in MUTEX_ENUM
 * @retval      #RET_INVALID_PARAM if mutex ref does not exist
 * @retval      #RET_ERROR if cannot acquires the mutex
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t AcquireMutex(mutexNo_t mutex)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    BaseType_t mutex_status;

    // Function Core
    if (mutex < (mutexNo_t)NB_MUTEXES)
    {
        mutex_status = xSemaphoreTake(g_mutexes_desc_table[mutex].handle, portMAX_DELAY);
        if (mutex_status != pdTRUE)
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ReleaseMutex(mutexNo_t mutex)
 * @brief       Function that releases the mutex.
 * @param[in]   mutex   Mutex reference number as defined in MUTEX_ENUM
 * @retval      #RET_INVALID_PARAM if mutex ref does not exist
 * @retval      #RET_ERROR if cannot release the mutex
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ReleaseMutex(mutexNo_t mutex)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    BaseType_t mutex_status;

    // Function Core
    if (mutex < (mutexNo_t)NB_MUTEXES)
    {
        // First check if the current task is the owner of the mutex
        if (xSemaphoreGetMutexHolder(g_mutexes_desc_table[mutex].handle) == xTaskGetCurrentTaskHandle())
        {
            mutex_status = xSemaphoreGive(g_mutexes_desc_table[mutex].handle);
            if (mutex_status != pdTRUE)
            {
                return_value = RET_ERROR;
            }
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

    return return_value;
}
