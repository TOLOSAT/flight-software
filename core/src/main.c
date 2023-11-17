/**
 * @file    main.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for main
 * @date    26/12/2022
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "main.h"
#include "initialisation.h"
#include "os.h"

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
    StartOS();
    while (1)
    {
        // Do Nothing
    }

    return 0;
}
