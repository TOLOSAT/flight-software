/**
 * @file    mutex.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining mutexes
 * @date    28/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "mutex.h"
#include "conf/mutex_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      CreateMutexes(void)
 * @brief   Function that creates all mutexes
 * @retval  #TASK_SUCCESSFUL if creation succeed
 * @retval  #TASK_ERROR if at least one task creation failed
 */
mutexStatus_t CreateMutexes(void)
{
    // Variable Initialisation
    mutexStatus_t return_value = MUTEX_SUCCESSFUL;
    mutexRef_t mutex = 0;

    // Function Core
    while ((mutex < (mutexRef_t)NB_MUTEXES) && (return_value == MUTEX_SUCCESSFUL))
    {
        g_mutex_conf[mutex].handle = xSemaphoreCreateMutex();
        if (g_mutex_conf[mutex].handle == NULL)
        {
            return_value = MUTEX_ERROR;
        }
        mutex++;
    }

    return return_value;
}

/**
 * @fn          AcquireMutex(mutexRef_t mutex)
 * @brief       Function that acquires the mutex.
 * @param[in]   mutex Mutex reference number as defined in MUTEX_ENUM
 * @retval      MUTEX_INVALID_PARAM if mutex ref does not exist
 * @retval      MUTEX_ERROR if cannot acquires the mutex
 * @retval      MUTEX_SUCCESSFUL else
 */
mutexStatus_t AcquireMutex(mutexRef_t mutex)
{
    // Variable Initialisation
    mutexStatus_t return_value = MUTEX_SUCCESSFUL;
    BaseType_t mutex_status;

    // Function Core
    if (mutex < (mutexRef_t)NB_MUTEXES)
    {
        mutex_status = xSemaphoreTake(g_mutex_conf[mutex].handle, 0u);
        if (mutex_status != pdTRUE)
        {
            return_value = MUTEX_ERROR;
        }
    }
    else
    {
        return_value = MUTEX_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ReleaseMutex(mutexRef_t mutex)
 * @brief       Function that releases the mutex.
 * @param[in]   mutex Mutex reference number as defined in MUTEX_ENUM
 * @retval      MUTEX_INVALID_PARAM if mutex ref does not exist
 * @retval      MUTEX_ERROR if cannot release the mutex
 * @retval      MUTEX_SUCCESSFUL else
 */
mutexStatus_t ReleaseMutex(mutexRef_t mutex)
{
    // Variable Initialisation
    mutexStatus_t return_value = MUTEX_SUCCESSFUL;
    BaseType_t mutex_status;

    // Function Core
    if (mutex < (mutexRef_t)NB_MUTEXES)
    {
        mutex_status = xSemaphoreGive(g_mutex_conf[mutex].handle);
        if (mutex_status != pdTRUE)
        {
            return_value = MUTEX_ERROR;
        }
    }
    else
    {
        return_value = MUTEX_INVALID_PARAM;
    }

    return return_value;
}