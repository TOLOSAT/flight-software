/**
 * @file    mutex.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining mutexes
 * @date    28/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core_basics.h"
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
    mutexRef_t mutex = 0;

    // Function Core
    while ((mutex < (mutexRef_t)NB_MUTEXES) && (return_value == CORE_SUCCESSFUL))
    {
        g_mutex_conf[mutex].handle = xSemaphoreCreateMutex();
        if (g_mutex_conf[mutex].handle == NULL)
        {
            return_value = CORE_ERROR;
        }
        mutex++;
    }

    return return_value;
}

/**
 * @fn          AcquireMutex(mutexRef_t mutex)
 * @brief       Function that acquires the mutex.
 * @param[in]   mutex Mutex reference number as defined in MUTEX_ENUM
 * @retval      #CORE_INVALID_PARAM if mutex ref does not exist
 * @retval      #CORE_ERROR if cannot acquires the mutex
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION AcquireMutex(mutexRef_t mutex)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    BaseType_t mutex_status;

    // Function Core
    if (mutex < (mutexRef_t)NB_MUTEXES)
    {
        mutex_status = xSemaphoreTake(g_mutex_conf[mutex].handle, 0u);
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
 * @fn          ReleaseMutex(mutexRef_t mutex)
 * @brief       Function that releases the mutex.
 * @param[in]   mutex Mutex reference number as defined in MUTEX_ENUM
 * @retval      #CORE_INVALID_PARAM if mutex ref does not exist
 * @retval      #CORE_ERROR if cannot release the mutex
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION ReleaseMutex(mutexRef_t mutex)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    BaseType_t mutex_status;

    // Function Core
    if (mutex < (mutexRef_t)NB_MUTEXES)
    {
        mutex_status = xSemaphoreGive(g_mutex_conf[mutex].handle);
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