/**
 * @file    iridium.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for IRIDIUM Task
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "iridium.h"
#include "kernel.h"
#include "iridium_driver.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              IridiumMain(void)
 * @brief           Main of the IRIDIUM Task
 */
void IridiumMain(void)
{
    // Initialisation

    // Task Core
    while (1)
    {
        SleepPeriodic();
    }
}