/**
 * @file    drv_common.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for drivers common functions (e.g. HAL init)
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core.h"
#include "drv/drv_types.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitHal(void)
 * @brief   Function that init the choosen HAL and sysclock
 * @retval  #CORE_ERROR if cannot init HAL or system clock
 * @retval  #CORE_SUCCESSFUL else
 *
 * If there is an error it goes to Error Handler
 */
coreStatus_t InitHal(void)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    HAL_StatusTypeDef status = cmsdk_InitHal();
    if (status != HAL_OK)
    {
        return_value = CORE_ERROR;
    }

    return return_value;
}
