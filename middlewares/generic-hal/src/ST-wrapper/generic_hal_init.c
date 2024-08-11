/**
 * @file    generic_hal_init.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC HAL Initialisation functions
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitHal(void)
 * @brief   Function that init the choosen HAL dans sysclock
 * @retval  #GEN_HAL_ERROR if cannot init HAL or system clock
 * @retval  #GEN_HAL_SUCCESSFUL else
 *
 * If there is an error it goes to Error Handler
 */
halStatus_t IN_GENERIC_HAL_TEXT_SECTION InitHal(void)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    HAL_StatusTypeDef test_hal = HAL_Init();
    if (test_hal == HAL_OK)
    {
        bspStatus_t test_bsp = SystemClock_Config();
        if (test_bsp != BSP_SUCCESSFUL)
        {
            return_value = GEN_HAL_ERROR;
        }
    }
    else
    {
        return_value = GEN_HAL_ERROR;
    }

    return return_value;
}
