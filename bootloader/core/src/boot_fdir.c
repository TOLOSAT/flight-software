/**
 * @file    boot_fdir.c
 * @author  Merlin Kooshmanian
 * @brief   Error Management functions for boot
 * @date    25/03/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "boot_fdir.h"
#include "boot_misc.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void Reset_Handler(void); /**< Reset Handler defined in startup.c file */

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
    if (status != 0u)
    {
        if (sanction == FDIR_ERROR_HANDLER)
        {
            // Go to error handler
            Error_Handler();
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
    HAL_GPIO_WritePin(BLUE_LED_GPIO_PORT, BLUE_LED_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RED_LED_GPIO_PORT, RED_LED_PIN, GPIO_PIN_RESET);
    while (1)
    {
        // Do nothing
    }
}
