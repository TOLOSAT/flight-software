/**
 * @file fdir.c
 * @author Merlin Kooshmanian
 * @brief Error Management functions
 * @date 05/05/2023
 *
 * Last Update : 05/05/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <cmsis_os2.h>
#include <FreeRTOS.h>
#include <task.h>

#include "fdir.h"
#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void Reset_Handler(void); /**< Reset Handler defined in startup.c file */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          CheckErrors(uint32_t status, errorsSanction_t sanction)
 * @brief       This function check if an error occured and execute the sanction
 * @param[in]   status Return value of a function.
 * @param[in]   sanction The sanction that has to be performed in order to solve the problem
 */
void CheckErrors(uint32_t status, errorsSanction_t sanction)
{
    if(status != 0u)
    {
        if(sanction == ERROR_HANDLER)
        {
            Error_Handler();
        }
        else if (sanction == RESET_HANDLER)
        {
            Reset_Handler();
        }
        else
        {
            /* No Sanction needed */
        }
    }
    else
    {
        /* No errors */
    }
}

/**
 * @brief  This function is executed in case of error occurrence.
 */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
        // Do nothing
    }
}

/**
 * @brief  This function is executed if a task runs out of stack
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName)
{
    // Unused Parameters
    (void)xTask;
    (void)pcTaskName;

    // Function Core
    while(1)
    {
        /* Do Nothing */
    }
}