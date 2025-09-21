/**
 * @file    mutex.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining mutexes
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "core/mutex.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      CreateMutexes(void)
 * @brief   Function that creates all mutexes
 * @return  Nothing
 */
void CreateMutexes(void)
{
    mutexNo_t mutex = 1u;

    // Create statically every mutex
    while (MUTEX_CONF(mutex).mutex != NO_MUTEX)
    {
        MUTEX_DESC(mutex).handle = xSemaphoreCreateMutexStatic(MUTEX_CONF(mutex).p_queue);
        if (MUTEX_DESC(mutex).handle == NULL)
        {
            KernelPanic();
        }

        // Indicates the mutex is initialised
        MUTEX_DESC(mutex).status = DESC_USED;
        mutex++;
    }
}

/**
 * @fn          AcquireMutex(mutexNo_t mutex)
 * @brief       Function that acquires the mutex.
 * @param[in]   mutex   Mutex reference number
 * @retval      #RET_INVALID_PARAM if mutex ref does not exist
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t AcquireMutex(mutexNo_t mutex)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    BaseType_t mutex_status;

    // Check parameter(s)
    if (IS_A_VALID_MUTEX(mutex))
    {
        mutex_status = xSemaphoreTake(MUTEX_DESC(mutex).handle, portMAX_DELAY);
        if (mutex_status != pdTRUE)
        {
            KernelPanic();
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
 * @param[in]   mutex   Mutex reference number
 * @retval      #RET_INVALID_PARAM if mutex ref does not exist
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ReleaseMutex(mutexNo_t mutex)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    BaseType_t mutex_status;

    // Check parameter(s)
    if (IS_A_VALID_MUTEX(mutex))
    {
        // First check if the current task is the owner of the mutex
        if (xSemaphoreGetMutexHolder(MUTEX_DESC(mutex).handle) == xTaskGetCurrentTaskHandle())
        {
            mutex_status = xSemaphoreGive(MUTEX_DESC(mutex).handle);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
