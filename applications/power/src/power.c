/**
 * @file    power.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for POWER Task
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "power.h"
#include "kernel.h"
#include "system_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              PowerMain(void)
 * @brief           Main of the power Task
 */
void PowerMain(void)
{
    // Initialisation

    // Task Core
    while (1)
    {
        SleepPeriodic();
    }
}