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
        g_mutex_conf[mutex].handle = xSemaphoreCreateMutexStatic(g_mutex_conf[mutex].data);
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
        // First check if the current task is the owner of the mutex
        if (xSemaphoreGetMutexHolder(g_mutex_conf[mutex].handle) == xTaskGetCurrentTaskHandle())
        {
            mutex_status = xSemaphoreGive(g_mutex_conf[mutex].handle);
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

/**
 * @fn          ResetMutex(mutexRef_t mutex)
 * @brief       Function that reset the mutex.
 * @param[in]   mutex Mutex reference number as defined in MUTEX_ENUM
 * @retval      #CORE_INVALID_PARAM if mutex ref does not exist
 * @retval      #CORE_ERROR if cannot recreate the mutex
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION ResetMutex(mutexRef_t mutex)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if (mutex < (mutexRef_t)NB_MUTEXES)
    {
        // Entering in the critical section because  
        // this action cannot be preempted.
        taskENTER_CRITICAL();

        // First delete mutex and erase content
        vSemaphoreDelete(g_mutex_conf[mutex].handle);
        memset(&g_mutex_conf[mutex].handle, 0, sizeof(mutexHandle_t));
        memset(g_mutex_conf[mutex].data, 0, sizeof(mutexData_t));

        // Then recreate the mutex
        g_mutex_conf[mutex].handle = xSemaphoreCreateMutexStatic(g_mutex_conf[mutex].data);
        if (g_mutex_conf[mutex].handle == NULL)
        {
            return_value = CORE_ERROR;
        }

        // Come back to normal execution
        taskEXIT_CRITICAL();
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ResetHoldedMutexes(taskRef_t task)
 * @brief       Function that reset all the mutexes holded by a task.
 * @param[in]   mutex Mutex reference number as defined in MUTEX_ENUM
 * @retval      #CORE_INVALID_PARAM if mutex ref does not exist
 * @retval      #CORE_ERROR if cannot recreate a mutex
 * @retval      #CORE_SUCCESSFUL else
 * 
 * This function will check for each mutex whether it has been holded by
 * the task. Consequently, the more mutexes there are, the longer it is.
 */
coreStatus_t IN_CORE_TEXT_SECTION ResetHoldedMutexes(taskRef_t task)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    mutexRef_t mutex = 0;

    // Function Core
    if (task < (taskRef_t)NB_TASKS)
    {
        // Entering in the critical section because 
        // this action cannot be preempted.
        taskENTER_CRITICAL();

        while ((mutex < (mutexRef_t)NB_MUTEXES) && (return_value == CORE_SUCCESSFUL))
        {
            // Reset the mutex only if it has been held by the task
            if (xSemaphoreGetMutexHolder(g_mutex_conf[mutex].handle) == g_tasks_dynamic_conf[task].handle)
            {
                ResetMutex(mutex);
            }
            mutex++;
        }

        // Come back to normal execution
        taskEXIT_CRITICAL();
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}
