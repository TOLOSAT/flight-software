/**
 * @file    dummy_tasks.c
 * @author  Matteo Planchet & Nell Truong
 * @brief   Source file with the oneshot timer test procedure.
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "dummy_tasks.h"
#include "kernel.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      OneshotTask(void)
 * @brief   Function that runs the oneshot timer test procedure.
 */
void OneshotTask(void)
{
    LOG("Init dummy timer\n");

    // Timer Core
    while (1)
    {
        (void)SetTimer(ONESHOT_TIMER, (tick_t)1000, TIMER_ONESHOT);

        tick_t initial_time = GetTick();

        (void)StartTimer(ONESHOT_TIMER);

        (void)WaitSignal(SIGNAL_TIMER_ENDED);

        // Check timer duration
        tick_t timer_duration = GetTick() - initial_time;

        // Check duration is ok :thusup:
        if (timer_duration == (tick_t)1000)
        {
            LOG_DECIMAL("It worked ! (Timer 1 lasted: %d ticks)\n", timer_duration);
        }
        else
        {
            LOG_DECIMAL("Shit happened. (Timer 1 lasted: %d ticks)\n", timer_duration);
        }

        Sleep(1000);
    }
}