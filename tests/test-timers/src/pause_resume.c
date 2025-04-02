/**
 * @file    dummy_tasks.c
 * @author  Matteo Planchet & Nell Truong
 * @brief   Source file with the pause/resume timer test procedure.
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
 * @fn      PauseResumeTask(void)
 * @brief   Function that runs the pause/resume timer test procedure.
 */
void PauseResumeTask(void)
{
    LOG("Init dummy timer\n");

    // Timer Core
    while (1)
    {
        (void)SetTimer(PAUSERESUME_TIMER, (tick_t)1000, TIMER_ONESHOT);

        tick_t initial_time_2 = GetTick();

        (void)StartTimer(PAUSERESUME_TIMER);

        Sleep(700);

        (void)PauseTimer(PAUSERESUME_TIMER);

        // LOG_DECIMAL("Paused after %d ticks.\n", GetTick() - initial_time_2);
        Sleep(300);
        // LOG_DECIMAL("Time after sleep : %d ticks.\n", GetTick() - initial_time_2);

        (void)ResumeTimer(PAUSERESUME_TIMER);

        (void)WaitSignal(SIGNAL_TIMER_ENDED);

        tick_t timer_duration_2 = GetTick() - initial_time_2;

        // Check duration is ok :thusup:
        if (timer_duration_2 == (tick_t)1300)
        {
            LOG_DECIMAL("It worked ! (Timer 2 lasted: %d ticks)\n", timer_duration_2);
        }
        else
        {
            LOG_DECIMAL("Shit happened. (Timer 2 lasted: %d ticks)\n", timer_duration_2);
        }

        Sleep(1000);
    }
}