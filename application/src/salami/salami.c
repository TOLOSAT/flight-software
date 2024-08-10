/**
 * @file    salami.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for SALAMI Task
 * @date    28/06/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "salami/salami.h"
#include "core.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              SalamiMain(void *task_desc)
 * @brief           Main of the SALAMI Task
 * @param[in,out]   task_desc Descriptor of the current task
 */
void IN_SALAMI_TEXT_SECTION SalamiMain(void *task_desc)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        ConsolePrint("Hello\n");
        (void)GpioToggle(&user_led_inst);

        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}