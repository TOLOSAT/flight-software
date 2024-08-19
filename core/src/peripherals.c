/**
 * @file    peripherals.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining peripherals
 * @date    19/08/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

coreStatus_t IN_CORE_TEXT_SECTION InitPeripherals(void)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core

    return return_value;
}

coreStatus_t IN_CORE_TEXT_SECTION LockPeripherals(peripheralNo_t peripheral)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    (void)(peripheral);

    return return_value;
}

coreStatus_t IN_CORE_TEXT_SECTION UnlockPeripherals(peripheralNo_t peripheral)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    (void)(peripheral);

    return return_value;
}