/**
 * @file main.c
 * @author Merlin Kooshmanian
 * @brief Main source file
 * @date 26/12/2022
 *
 * Last Update : 21/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <cmsis_os2.h>

#include "main.h"
#include "initialisation.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      main(void)
 * @brief   Flight Software Main is the entrypoint of the code
 * @return  0
 */
int main(void)
{
    // Initialisation
    init();

    // Function Core
    osKernelStart();
    while (1)
    {
        // Scheduler runs
    }

    return 0;
}
