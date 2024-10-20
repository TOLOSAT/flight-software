/**
 * @file    thermal.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for THERMAL Task
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "thermal.h"
#include "kernel.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              ThermalMain(void)
 * @brief           Main of the thermal Task
 */
void ThermalMain(void)
{
    // Initialisation

    // Function Core
    while (1)
    {
        SleepPeriodic();
    }
}