/**
 * @file    mutex.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining mutexes
 * @date    28/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "core.h"
#include "conf/mutex_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      CreateMutexes(void)
 * @brief   Function that creates all mutexes
 * @retval  #CORE_SUCCESSFUL if creation succeed
 * @retval  #CORE_ERROR if at least one task creation failed
 */
coreStatus_t IN_CORE_TEXT_SECTION CreateMutexes(void)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    mutexNo_t mutex = 0;

    // Function Core
    while ((mutex < (mutexNo_t)NB_MUTEXES) && (return_value == CORE_SUCCESSFUL))
    {
        g_mutexes_desc_table[mutex].handle = xSemaphoreCreateMutexStatic(g_mutex_conf_table[mutex].p_data);
        if (g_mutexes_desc_table[mutex].handle == NULL)
        {
            return_value = CORE_ERROR;
        }
        mutex++;
    }

    return return_value;
}

/**
 * @fn          AcquireMutex(mutexNo_t mutex)
 * @brief       Function that acquires the mutex.
 * @param[in]   mutex Mutex reference number as defined in MUTEX_ENUM
 * @retval      #CORE_INVALID_PARAM if mutex ref does not exist
 * @retval      #CORE_ERROR if cannot acquires the mutex
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION AcquireMutex(mutexNo_t mutex)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    BaseType_t mutex_status;

    // Function Core
    if (mutex < (mutexNo_t)NB_MUTEXES)
    {
        mutex_status = xSemaphoreTake(g_mutexes_desc_table[mutex].handle, 0u);
        if (mutex_status != pdTRUE)
        {
            return_value = CORE_ERROR;
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ReleaseMutex(mutexNo_t mutex)
 * @brief       Function that releases the mutex.
 * @param[in]   mutex Mutex reference number as defined in MUTEX_ENUM
 * @retval      #CORE_INVALID_PARAM if mutex ref does not exist
 * @retval      #CORE_ERROR if cannot release the mutex
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION ReleaseMutex(mutexNo_t mutex)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
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
                return_value = CORE_ERROR;
            }
        }
        else
        {
            return_value = CORE_ERROR;
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}
