/**
 * @file    drv_common.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for drivers common functions (e.g. HAL init)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_common.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitHal(void)
 * @brief   Function that init the choosen HAL and sysclock
 *
 * If there is an error it goes to KernelPanic
 */
void InitHal(void)
{
    // Function Core
    HAL_StatusTypeDef test_hal = HAL_Init();

    // Check return value
    if (test_hal != HAL_OK)
    {
        KernelPanic();
    }
}
