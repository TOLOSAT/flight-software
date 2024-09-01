/**
 * @file    drv_ecc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for ECC functions
 * @date    15/02/2024
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
 * @fn      EccInit(void)
 * @brief   This function init ECC
 * @retval  #CORE_SUCCESSFUL always
 */
coreStatus_t IN_CORE_TEXT_SECTION EccInit(void)
{
    // Do nothing because ECC is not available
    return CORE_SUCCESSFUL;
}
