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
#include "timer.h"

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

        // Initialisation
        CreateTimers();

        // set timer
        SetTimer(
            TIMER_1,
            (tick_t) 1000,
            TIMER_SINGLE_SHOT
            );

        // check time (in ticks)
        tick_t initial_time = xTaskGetTickCount();

        // launch timer
        StartTimer(TIMER_1);
        // timer ends

        // check time
        tick_t timer_duration = xTaskGetTickCount() - initial_time;

        // check duration is ok :thusup:
        if (timer_duration == (tick_t) 1000) {
            LOG("It worked !\n");
        } else {
            LOG("Shit happened.\n");
        }

    }
}