/**
 * @file    mutex.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining mutexes
 *
 * @copyright Copyright (c) TOLOSAT 2024
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
    mutexNo_t mutex = 0;

    // Create statically every mutex
    while (mutex < NB_MUTEXES)
    {
        g_mutexes_desc_table[mutex].handle = xSemaphoreCreateMutexStatic(g_mutex_conf_table[mutex].p_queue);
        if (g_mutexes_desc_table[mutex].handle == NULL)
        {
            KernelPanic();
        }
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
    if (mutex < NB_MUTEXES)
    {
        mutex_status = xSemaphoreTake(g_mutexes_desc_table[mutex].handle, portMAX_DELAY);
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
    if (mutex < NB_MUTEXES)
    {
        // First check if the current task is the owner of the mutex
        if (xSemaphoreGetMutexHolder(g_mutexes_desc_table[mutex].handle) == xTaskGetCurrentTaskHandle())
        {
            mutex_status = xSemaphoreGive(g_mutexes_desc_table[mutex].handle);
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
