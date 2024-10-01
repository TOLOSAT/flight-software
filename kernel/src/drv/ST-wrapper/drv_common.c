/**
 * @file    drv_common.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for drivers common functions (e.g. HAL init)
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_common.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitHal(void)
 * @brief   Function that init the choosen HAL dans sysclock
 * @retval  #KERNEL_ERROR if cannot init HAL or system clock
 * @retval  #KERNEL_SUCCESSFUL else
 *
 * If there is an error it goes to Error Handler
 */
kernelStatus_t InitHal(void)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    HAL_StatusTypeDef test_hal = HAL_Init();
    if (test_hal == HAL_OK)
    {
        bspStatus_t test_bsp = SystemClock_Config();
        if (test_bsp != BSP_SUCCESSFUL)
        {
            return_value = KERNEL_ERROR;
        }
    }
    else
    {
        return_value = KERNEL_ERROR;
    }

    return return_value;
}
