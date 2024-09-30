/**
 * @file    power.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for POWER Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "power/power.h"
#include "kernel.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              PowerMain(void)
 * @brief           Main of the power Task
 */
void IN_POWER_TEXT_SECTION PowerMain(void)
{
    // Initialisation

    // Function Core
    while (1)
    {
        SleepPeriodic();
    }
}