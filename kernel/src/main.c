/**
 * @file    main.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for main
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "main.h"
#include "core/initialisation.h"
#include "core/os.h"

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

    // Start Operating System
    StartOS();
    while (1)
    {
        // Do Nothing
    }

    return 0;
}
