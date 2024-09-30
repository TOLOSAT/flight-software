/**
 * @file    iridium.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for IRIDIUM Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "iridium/iridium.h"
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
void IN_IRIDIUM_TEXT_SECTION IridiumMain(void)
{
    // Initialisation

    // Function Core
    while (1)
    {
        SleepPeriodic();
    }
}