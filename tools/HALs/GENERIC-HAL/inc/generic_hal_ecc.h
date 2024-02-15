/**
 * @file    generic_hal_ecc.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for GENERIC HAL ECC functions
 * @date    15/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup generic_hal GENERIC HAL
 * @{
 * @defgroup drv_ecc Driver ECC
 * Functions to deal with the RAM ECC peripheral that corrects 
 * bit flip in RAM memory (STM32 H7 only)
 * @{
 */

#ifndef generic_hal_ECC_H
#define generic_hal_ECC_H

/******************************* Include Files *******************************/

#include "generic_hal_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

#if defined(ECC_AVAILABLE)
/** @brief Ecc instance type definition */
typedef RAMECC_HandleTypeDef eccInst_t;
#endif

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

halStatus_t EccInit(void);

#endif /* generic_hal_ECC_H */

/** 
 * @} 
 * @} 
 */