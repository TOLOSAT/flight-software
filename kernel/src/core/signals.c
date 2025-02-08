/**
 * @file    signals.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for signals handling functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core/signals.h"
#include "core/tasks.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          SendSignal(taskNo_t task, signalMask_t mask)
 * @brief       This function sends signal(s) to a specific task.
 * @param[in]   task    Task to which the signal will be sent
 * @param[in]   mask    Mask defining which signal type(s) will be sent
 * @retval      #RET_INVALID_PARAM if task does not exists
 * @retval      #RET_INVALID_PARAM if mask is null
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SendSignal(taskNo_t task, signalMask_t mask)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    BaseType_t test_value;

    // Check parameter(s)
    if ((task != 0u) && (task <= NB_TASKS) && (mask != 0u))
    {
        // Check if we are in an interrupt context or not
        if (xPortIsInsideInterrupt() == pdFALSE)
        {
            test_value = xTaskNotify(g_tasks_desc_table[TASKNO_TO_LINENO(task)].handle, mask, eSetBits);
            if (test_value != pdPASS)
            {
                KernelPanic();
            }
        }
        else
        {
            BaseType_t higher_priority_task_woken = pdFALSE;

            test_value = xTaskNotifyFromISR(g_tasks_desc_table[TASKNO_TO_LINENO(task)].handle, mask, eSetBits, &higher_priority_task_woken);
            if (test_value != pdPASS)
            {
                KernelPanic();
            }

            // If an higher priority task has been woken, the interrupted
            // task need to yield in order to not create delays.
            portYIELD_FROM_ISR(higher_priority_task_woken);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          WaitSignal(signalMask_t mask)
 * @brief       This function wait for specifics signals.
 * @param[in]   mask    Mask defining which signal type(s) will be waited for
 * @retval      #RET_INVALID_PARAM if mask is null
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t WaitSignal(signalMask_t mask)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    BaseType_t test_value;

    // Check parameter(s)
    if ((mask != 0u))
    {
        test_value = xTaskNotifyWait(0u, mask, NULL, portMAX_DELAY);
        if (test_value != pdPASS)
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