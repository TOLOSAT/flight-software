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
    // Check return value
    switch (status)
    {
    case HAL_OK:
        return_value = RET_SUCCESSFUL;
        break;
    case HAL_TIMEOUT:
        return_value = RET_TIMEOUT;
        break;
    case HAL_BUSY:
        return_value = RET_NOT_AVAILABLE;
        break;
    default:
        KernelPanic();
        break;
    }

    return return_value;
}
