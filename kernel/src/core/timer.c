/**
 * @file    timer.c
 * @author  Matteo Planchet, Nell Truong
 * @brief   Source file defining timer
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "core/timer.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define DEFAULT_TIMER_PERIOD (-1u)

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
void GenericTimerCallback(void (*callback)(timerHandle_t), timerNo_t timer) {
    (*callback)(g_timers_desc_table[timer].handle);
}

/**
 * @fn          CreateTimers(void)
 * @brief       Function that send a message in a buffer
 * @return      Nothing
 */
void CreateTimers(void)
{
    timerNo_t timer = 1u;

    // Create statically every timer
    while (timer < NB_TIMERS)
    {
        // Create timer
        g_timers_desc_table[timer].handle = xTimerCreateStatic("timer",                             // Timer name
                                                               DEFAULT_TIMER_PERIOD,                // Timer period
                                                               pdTRUE,                              // Timer mode
                                                               &timer,                              // Timer ID
                                                               (void *)0,                           // Timer callback
                                                               &g_timers_desc_table[timer].buffer); // Timer structure
        if (g_timers_desc_table[timer].handle == NULL)
        {
            KernelPanic();
        }

        timer++;
    }
}
/**
 * @fn          Start(timerNo_t timer)
 * @brief       Function that starts a timer
 * @param[in]   timer  The ID of the timer to start
 * @return      Nothing
 */
void Start(timerNo_t timer)
{
    xTimerStart(g_timers_desc_table[timer].handle, 0);
}
/**
 * @fn          Pause(timerNo_t timer)
 * @brief       Function that pauses a timer
 * @param[in]   timer  The ID of the timer to pause
 * @return      Nothing
 *
 * This function does not support timeout.
 */
void Pause(timerNo_t timer)
{
    xTimerStop(g_timers_desc_table[timer].handle, 0);
    g_timers_desc_table[timer].saved_counter = xTimerGetExpiryTime(g_timers_desc_table[timer].handle);
}
/**
 * @fn          Resume(timerNo_t timer)
 * @brief       Function that resumes a timer
 * @param[in]   timer  The ID of the timer to resume
 * @return      Nothing
 */
void Resume(timerNo_t timer)
{
    xTimerStart(g_timers_desc_table[timer].handle, 0);
    // TODO: check if that works
    xTimerChangePeriod(g_timers_desc_table[timer].handle, g_timers_desc_table[timer].saved_counter, 0);
}
/**
 * @fn          Set(timerNo_t timer, tick_t period, timerMode_t mode)
 * @brief       Function that sets the parameters of a timer
 * @param[in]   timer  The ID of the timer
 * @param[in]   period  The new timer period
 * @param[in]   mode  The new timer mode
 * @return      Nothing
 */
void Set(timerNo_t timer, tick_t period, timerMode_t mode)
{
    xTimerChangePeriod(g_timers_desc_table[timer].handle, period, 0);
    vTimerSetReloadMode(g_timers_desc_table[timer].handle, mode);
}
