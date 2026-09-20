/**
 * @file    power.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for POWER Task
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
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
 * @copydoc PowerMain
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
