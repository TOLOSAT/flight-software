/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "dummy_tasks.h"
#include "kernel.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask(void)
 * @brief   Function that runs the dummy main task.
 */
void DummyMainTask(void)
{
    LOG("Init dummy timer\n");

    // Timer Core
    while (1)
    {
        // set timer
        SetTimer(DUMMY_TIMER, (tick_t)1000, TIMER_ONESHOT);

        // check time (in ticks)
        tick_t initial_time = GetTick();

        // launch timer
        StartTimer(DUMMY_TIMER);
        // timer ends
        WaitSignal(SIGNAL_TIMER_ENDED);

        // check time
        tick_t timer_duration = GetTick() - initial_time;

        // check duration is ok :thusup:
        if (timer_duration == (tick_t)1000)
        {
            LOG_DECIMAL("It worked ! (lasted: %d ticks)\n", timer_duration);
        }
        else
        {
            LOG_DECIMAL("Shit happened. (lasted: %d ticks)\n", timer_duration);
        }

        Sleep(1000);
    }
}