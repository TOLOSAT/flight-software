/**
 * @file    timer.c
 * @author  Matteo Planchet, Nell Truong
 * @brief   Source file defining timer
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "core/timer.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

void CreateTimers(void)
{
    timerNo_t timer = 1u;

    // Create statically every timer
    while (timer <= NB_TIMERS)
    {
        // Create timer
        g_timers_desc_table[timer].handle = xTimerCreateStatic(g_timers_conf[timer].name,          // Timer name
                                                               g_timers_conf[timer].period,        // Timer period
                                                               g_timers_conf[timer].mode,          // Timer mode
                                                               (void *)timer,                      // Timer ID
                                                               TimerCallback,                      // Timer callback
                                                               &g_timers_desc_table[timer].timer); // Timer structure
        if (g_timers_desc_table[timer].handle == NULL)
        {
            KernelPanic();
        }

        timer++;
    }
}
void Start(timerNo_t timer)
{
    xTimerStart(g_timers_desc_table[timer].handle, 0);
}
void Pause(timerNo_t timer)
{
    xTimerStop(g_timers_desc_table[timer].handle, 0);
    g_timers_desc_table[timer].saved_counter = xTimerGetExpiryTime(g_timers_desc_table[timer].handle);
}
void Resume(timerNo_t timer)
{
    xTimerStart(g_timers_desc_table[timer].handle, 0);
    // TODO: check if that works
    xTimerChangePeriod(g_timers_desc_table[timer].handle, g_timers_desc_table[timer].saved_counter, 0);
}
void Set(timerNo_t timer, tick_t period, timerMode_t mode)
{
    xTimerChangePeriod(g_timers_desc_table[timer].handle, period, 0);
    xTimerChangeTimerMode(g_timers_desc_table[timer].handle, mode, 0);
}
