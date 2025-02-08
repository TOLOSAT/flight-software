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
 * @return  Nothing
 */
void InitHal(void)
{
    // Init HAL
    HAL_StatusTypeDef status = cmsdk_InitHal();
    if (status != HAL_OK)
    {
        KernelPanic();
    }
}
