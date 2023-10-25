/**
 * @file    fdir.c
 * @author  Merlin Kooshmanian
 * @brief   Error Management functions
 * @date    05/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "tasks.h"
#include "fdir.h"
#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void Reset_Handler(void); /**< Reset Handler defined in startup.c file */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);

/*************************** Variables Definitions ***************************/

uint32_t g_error_counter = 0u;

/*************************** Functions Definitions ***************************/

/**
 * @fn          CheckErrors(uint32_t status, errorsSanction_t sanction)
 * @brief       This function check if an error occured and execute the sanction
 * @param[in]   status Return value of a function.
 * @param[in]   sanction The sanction that has to be performed in order to solve the problem
 */
void CheckErrors(uint32_t status, errorsSanction_t sanction)
{
    if (status != 0u)
    {
        // Increment the error counter
        g_error_counter ++;

        // Check which sanction we have to take
        if (sanction == FDIR_SYSTEM_RESET)
        {
            // Reset processor
            NVIC_SystemReset();
            Reset_Handler();
        }
        else if (sanction == FDIR_ERROR_HANDLER)
        {
            // Go to error handler
            Error_Handler();
        }
        else if (sanction == FDIR_HALT_TASK)
        {
            taskHandle_t current_task_handle = NULL;
            current_task_handle = xTaskGetCurrentTaskHandle();
            // Check if we are in a task
            if (current_task_handle != NULL)
            {
                // If we are in the task we suspend it
                vTaskSuspend(current_task_handle);
            }
            else
            {
                // If we are not in a task then we go to error handler by default
                Error_Handler();
            }
        }
        else
        {
            // No Sanction needed
        }
    }
    else
    {
        // No errors
    }
}

/**
 * @fn      Error_Handler(void)
 * @brief   This function is executed in case of error occurrence.
 * @warning Real FDIR has to be done.
 * 
 * Normally this function has to save some context (what happened before error). 
 * But currently no real fdir has been done. For debugging purposes, Error handler 
 * is just a while loop that hangs processor indefinitly.
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
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // Unused Parameters
    (void)xTask;
    (void)pcTaskName;

    // Function Core
    while (1)
    {
        /* Do Nothing */
    }
}