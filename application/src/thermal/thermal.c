/**
 * @file    thermal.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for THERMAL Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "thermal/thermal.h"
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