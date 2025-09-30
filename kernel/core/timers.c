/**
 * @file    timers.c
 * @author  Matteo Planchet
 * @author  Nell Truong
 * @brief   Source file defining timer
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "core/timers.h"
#include "core/signals.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define DEFAULT_TIMER_PERIOD (-1u) /**< Timers default period (uint32_t max) */

/*************************** Functions Declarations **************************/

static void GenericTimerCallback(timerHandle_t handle);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_timers_desc_table
 * @brief   Configuration table where all timers' descriptors are stored
 */
timerDesc_t g_timers_desc_table[CONFIG_MAX_NB_TIMERS] = { 0 };

/*************************** Functions Definitions ***************************/

/**
 * @fn          CreateTimers(void)
 * @brief       Function that send a message in a buffer
 * @return      Nothing
 */
void
CreateTimers(void)
{
    timerNo_t timer = 1u;

    // Create statically every timer
    while (TIMER_CONF(timer).timer != NO_TIMER)
    {
        // Create timer
        // Note : timer ID is used to share the descriptor in the generic callback.
        TIMER_DESC(timer).handle = xTimerCreateStatic("timer",                         // Timer name
                                                      DEFAULT_TIMER_PERIOD,            // Timer period
                                                      pdTRUE,                          // Timer mode
                                                      &TIMER_DESC(timer),              // Timer ID
                                                      (void *)GenericTimerCallback,    // Timer callback
                                                      TIMER_CONF(timer).p_tim_buffer); // Timer buffer
        if (TIMER_DESC(timer).handle == NULL)
        {
            KernelPanic();
        }

        // Update timer owner in the timer descriptor
        TIMER_DESC(timer).owner = TIMER_CONF(timer).owner;

        // Indicates the timer is initialised
        TIMER_DESC(timer).status = DESC_USED;
        timer++;
    }
}
/**
 * @fn          StartTimer(timerNo_t timer)
 * @brief       Function that starts a timer
 * @param[in]   timer  The ID of the timer to start
 * @retval      #RET_INVALID_PARAM if the timer is not a valid timer.
 * @retval      #RET_SUCCESSFUL if the timer failed to start
 * @retval      #RET_ERROR else
 */
returnCode_t StartTimer(timerNo_t timer)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_TIMER(timer) && (GetCurrentTask() == TIMER_DESC(timer).owner))
    {
        // Start timer
        BaseType_t test_timer = xTimerStart(TIMER_DESC(timer).handle, 0);
        if (test_timer != pdPASS)
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
 * @fn          PauseTimer(timerNo_t timer)
 * @brief       Function that pauses a timer
 * @param[in]   timer  The ID of the timer to pause
 * @retval      #RET_INVALID_PARAM if the timer is not a valid timer.
 * @retval      #RET_SUCCESSFUL if the timer failed to pause
 * @retval      #RET_ERROR else
 *
 * This function does not support timeout.
 */
returnCode_t PauseTimer(timerNo_t timer)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_TIMER(timer) && (GetCurrentTask() == TIMER_DESC(timer).owner))
    {
        // Stop timer
        BaseType_t test_timer = xTimerStop(TIMER_DESC(timer).handle, 0);
        if (test_timer == pdPASS)
        {
            // Save remaining time in the descriptor
            TIMER_DESC(timer).saved_counter = xTimerGetExpiryTime(TIMER_DESC(timer).handle) - xTaskGetTickCount();
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
/**
 * @fn          ResumeTimer(timerNo_t timer)
 * @brief       Function that resumes a timer
 * @param[in]   timer  The ID of the timer to resume
 * @retval      #RET_INVALID_PARAM if the timer is not a valid timer.
 * @retval      #RET_INVALID_PARAM if the timer is in periodic mode.
 * @retval      #RET_ERROR if the timer failed to resume
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ResumeTimer(timerNo_t timer)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_TIMER(timer) && (GetCurrentTask() == TIMER_DESC(timer).owner))
    {
        // Check timer mode.
        if (xTimerGetReloadMode(TIMER_DESC(timer).handle) == pdTRUE)
        {
            // Auto-reload mode.
            // Resume is not available for this mode.
            return_value = RET_INVALID_PARAM;
        }
        else
        {
            // Oneshot mode.
            // Then change the timer period to the one saved.
            BaseType_t test_timer = xTimerChangePeriod(TIMER_DESC(timer).handle, TIMER_DESC(timer).saved_counter, 0);
            if (test_timer == pdPASS)
            {
                // Then restart the timer.
                test_timer = xTimerStart(TIMER_DESC(timer).handle, 0);
                if (test_timer != pdPASS)
                {
                    return_value = RET_ERROR;
                }
            }
            else
            {
                return_value = RET_ERROR;
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
/**
 * @fn          SetTimer(timerNo_t timer, tick_t period, timerMode_t mode)
 * @brief       Function that sets the parameters of a timer
 * @param[in]   timer  The ID of the timer
 * @param[in]   period  The new timer period
 * @param[in]   mode  The new timer mode
 * @retval      #RET_INVALID_PARAM if the timer is not a valid timer.
 * @retval      #RET_INVALID_PARAM if the period is zero.
 * @retval      #RET_INVALID_PARAM if the mode is neither TIMER_ONESHOT nor TIMER_PERIODIC.
 * @retval      #RET_SUCCESSFUL if the timer failed to set its period
 * @retval      #RET_ERROR else
 */
returnCode_t SetTimer(timerNo_t timer, tick_t period, timerMode_t mode)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((IS_A_VALID_TIMER(timer)) && (GetCurrentTask() == TIMER_DESC(timer).owner) && (period != 0u)
        && ((mode == TIMER_ONESHOT) || (mode == TIMER_PERIODIC)))
    {
        // First change timer period
        BaseType_t test_timer = xTimerChangePeriod(TIMER_DESC(timer).handle, period, 0);
        if (test_timer == pdPASS)
        {
            // Then change the reload mode
            vTimerSetReloadMode(TIMER_DESC(timer).handle, mode);
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

/**
 * @fn          GenericTimerCallback(void (*callback)(timerHandle_t), timerNo_t)
 * @brief       Function serves as generic callbakc that calls the actual callback
 * @param[in]   timer  The callback associated with the timer
 * @param[in]   timer  The ID of the timer
 * @return      Nothing
 */
static void GenericTimerCallback(timerHandle_t handle)
{
    // Get the descriptor using the timer ID field
    timerDesc_t *desc = (timerDesc_t *)pvTimerGetTimerID(handle);

    // IMPROVEMENT : add specific callback
    // if desc.callback != null -> execute

    // Sends a signal to the owner to let him know that the timer has ended
    (void)SendSignal(desc->owner, SIGNAL_TIMER_ENDED);
}
