/**
 * @file    timer.c
 * @author  Matteo Planchet, Nell Truong
 * @brief   Source file defining timer
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

// TODO : change return types (see time.c)

/******************************* Include Files *******************************/

#include "core/timer.h"
#include "core/signals.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define DEFAULT_TIMER_PERIOD (-1u) /**< Timers default period (uint32_t max) */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          GenericTimerCallback(void (*callback)(timerHandle_t), timerNo_t)
 * @brief       Function serves as generic callbakc that calls the actual callback
 * @param[in]   timer  The callback associated with the timer
 * @param[in]   timer  The ID of the timer
 * @return      Nothing
 */
static void GenericTimerCallback(timerHandle_t handle)
{
    timerDesc_t *desc = (timerDesc_t *)pvTimerGetTimerID(handle);
    // TODO : faire un vrai callback
    // if desc.callback != null -> execute
    SendSignal(desc->owner, SIGNAL_TIMER_ENDED);
}

/**
 * @fn          CreateTimers(void)
 * @brief       Function that send a message in a buffer
 * @return      Nothing
 */
void CreateTimers(void)
{
    timerNo_t timer = 0u;

    // Create statically every timer
    while (timer < NB_TIMERS)
    {
        // Create timer
        g_timers_desc_table[timer].handle = xTimerCreateStatic("timer",                             // Timer name
                                                               DEFAULT_TIMER_PERIOD,                // Timer period
                                                               pdTRUE,                              // Timer mode
                                                               &g_timers_desc_table[timer],         // Timer ID
                                                               (void *)GenericTimerCallback,        // Timer callback
                                                               &g_timers_desc_table[timer].buffer); // Timer structure
        if (g_timers_desc_table[timer].handle == NULL)
        {
            KernelPanic();
        }
        g_timers_desc_table[timer].owner = g_timers_conf[timer].owner;

        timer++;
    }
}
/**
 * @fn          StartTimer(timerNo_t timer)
 * @brief       Function that starts a timer
 * @param[in]   timer  The ID of the timer to start
 * @retval      #RET_SUCCESSFUL if the timer failed to start
 * @retval      #RET_ERROR else
 */
returnCode_t StartTimer(timerNo_t timer)
{
    BaseType_t timerStarted = xTimerStart(g_timers_desc_table[timer].handle, 0);
    return (timerStarted == pdPASS) ? RET_SUCCESSFUL : RET_ERROR;
}
/**
 * @fn          PauseTimer(timerNo_t timer)
 * @brief       Function that pauses a timer
 * @param[in]   timer  The ID of the timer to pause
 * @retval      #RET_SUCCESSFUL if the timer failed to pause
 * @retval      #RET_ERROR else
 *
 * This function does not support timeout.
 */
returnCode_t PauseTimer(timerNo_t timer)
{
    BaseType_t timerPaused = xTimerStop(g_timers_desc_table[timer].handle, 0);
    g_timers_desc_table[timer].saved_counter = xTimerGetExpiryTime(g_timers_desc_table[timer].handle);
    return (timerPaused == pdPASS) ? RET_SUCCESSFUL : RET_ERROR;
}
/**
 * @fn          ResumeTimer(timerNo_t timer)
 * @brief       Function that resumes a timer
 * @param[in]   timer  The ID of the timer to resume
 * @retval      #RET_SUCCESSFUL if the timer failed to resume
 * @retval      #RET_ERROR else
 */
returnCode_t ResumeTimer(timerNo_t timer)
{
    BaseType_t timerUpdated = xTimerChangePeriod(g_timers_desc_table[timer].handle, g_timers_desc_table[timer].saved_counter, 0);
    // TODO: check if that works
    BaseType_t timerRestarted = xTimerStart(g_timers_desc_table[timer].handle, 0);
    return (timerUpdated == pdPASS) && (timerRestarted == pdPASS) ? RET_SUCCESSFUL : RET_ERROR;

}
/**
 * @fn          SetTimer(timerNo_t timer, tick_t period, timerMode_t mode)
 * @brief       Function that sets the parameters of a timer
 * @param[in]   timer  The ID of the timer
 * @param[in]   period  The new timer period
 * @param[in]   mode  The new timer mode
 * @retval      #RET_SUCCESSFUL if the timer failed to set its period
 * @retval      #RET_ERROR else
 */
returnCode_t SetTimer(timerNo_t timer, tick_t period, timerMode_t mode)
{
    BaseType_t timerPeriodUpdated = xTimerChangePeriod(g_timers_desc_table[timer].handle, period, 0);
    vTimerSetReloadMode(g_timers_desc_table[timer].handle, mode); // Does not have a return type
    return (timerPeriodUpdated == pdPASS) ? RET_SUCCESSFUL : RET_ERROR;
}
