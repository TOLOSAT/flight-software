/**
 * @file    oneshot.c
 * @author  Matteo Planchet & Nell Truong
 * @brief   Source file with the oneshot timer test procedure.
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "oneshot.h"
#include "kernel.h"
#include "system_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      OneshotMain(void)
 * @brief   Function that runs the oneshot timer test procedure.
 */
void OneshotMain(void)
{
    LOG("Init dummy timer\n");

    // Timer Core
    while (1)
    {
        (void)SetTimer(ONESHOT_TIMER, 1000u, TIMER_ONESHOT);

        tick_t initial_time = GetTick();

        (void)StartTimer(ONESHOT_TIMER);

        (void)WaitSignal(SIGNAL_TIMER_ENDED);

        // Check timer duration
        tick_t timer_duration = GetTick() - initial_time;

        // Check duration is ok :thusup:
        if (timer_duration == 1000u)
        {
            LOG_DECIMAL("It worked ! (Timer 1 lasted: %d ticks)\n", timer_duration);
        }
        else
        {
            LOG_DECIMAL("Shit happened. (Timer 1 lasted: %d ticks)\n", timer_duration);
        }

        Sleep(1000u);
    }
}