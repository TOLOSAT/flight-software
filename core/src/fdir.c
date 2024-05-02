/**
 * @file    fdir.c
 * @author  Merlin Kooshmanian
 * @brief   Error Management functions
 * @date    05/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core_basics.h"
#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void Reset_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);

/*************************** Variables Definitions ***************************/

uint32_t g_error_counter = 0u;

/*************************** Functions Definitions ***************************/

/**
 * @fn          CheckErrors(uint32_t status, errorsSanction_t sanction)
 * @brief       This function check if an error occured and execute the sanction
 * @param[in]   status Return value of a function.
 * @param[in]   sanction The sanction that has to be performed in order to solve the problem
 */
void IN_CORE_TEXT_SECTION CheckErrors(uint32_t status, errorsSanction_t sanction)
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
            ErrorHandler();
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
                ErrorHandler();
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
 * @fn      ErrorHandler(void)
 * @brief   This function is executed in case of error occurrence.
 * @warning Real FDIR has to be done.
 * 
 * Normally this function has to save some context (what happened before error). 
 * But currently no real fdir has been done. For debugging purposes, Error handler 
 * is just a while loop that hangs processor indefinitly.
 */
void IN_CORE_TEXT_SECTION ErrorHandler(void)
{
    __disable_irq();
    while (1)
    {
        // Do nothing
    }
}

/*************************** Interruption Handlers ***************************/

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
    while (1)
    {
    }
}
