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
 * @brief   Function that init the choosen HAL and sysclock
 * @retval  #RET_ERROR if cannot init HAL or system clock
 * @retval  #RET_SUCCESSFUL else
 *
 * If there is an error it goes to Error Handler
 */
returnCode_t InitHal(void)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    HAL_StatusTypeDef status = cmsdk_InitHal();
    if (status != HAL_OK)
    {
        return_value = RET_ERROR;
    }

    return return_value;
}
