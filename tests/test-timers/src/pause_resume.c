/**
 * @file    pause_resume.c
 * @author  Matteo Planchet & Nell Truong
 * @brief   Source file with the pause/resume timer test procedure.
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "pause_resume.h"
#include "kernel.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      PauseResumeMain(void)
 * @brief   Function that runs the pause/resume timer test procedure.
 */
void PauseResumeMain(void)
{
    LOG("Init dummy timer\n");

    // Timer Core
    while (1)
    {
        (void)SetTimer(PAUSERESUME_TIMER, 1000u, TIMER_ONESHOT);

        tick_t initial_time_2 = GetTick();

        (void)StartTimer(PAUSERESUME_TIMER);

        Sleep(700u);

        (void)PauseTimer(PAUSERESUME_TIMER);

        Sleep(300u);

        (void)ResumeTimer(PAUSERESUME_TIMER);

        (void)WaitSignal(SIGNAL_TIMER_ENDED);

        tick_t timer_duration_2 = GetTick() - initial_time_2;

        // Check duration is ok :thusup:
        if (timer_duration_2 == 1300u)
        {
            LOG_DECIMAL("It worked ! (Timer 2 lasted: %d ticks)\n", timer_duration_2);
        }
        else
        {
            LOG_DECIMAL("Shit happened. (Timer 2 lasted: %d ticks)\n", timer_duration_2);
        }

        Sleep(1000u);
    }
}