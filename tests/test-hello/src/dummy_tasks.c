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
    // Initialisation
    ConsolePrint("Init dummy task\n");

    // Function Core
    while (1)
    {
        // Get time
        ConsolePrint("Hello\n");

        SleepPeriodic();
    }
}