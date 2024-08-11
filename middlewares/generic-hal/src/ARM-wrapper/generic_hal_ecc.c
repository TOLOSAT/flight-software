/**
 * @file    generic_hal_ecc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC HAL ECC functions
 * @date    15/02/2024
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
 * @fn      EccInit(void)
 * @brief   This function init ECC
 * @retval  #GEN_HAL_SUCCESSFUL always
 */
halStatus_t IN_GENERIC_HAL_TEXT_SECTION EccInit(void)
{
    // Do nothing because ECC is not available
    return GEN_HAL_SUCCESSFUL;
}
