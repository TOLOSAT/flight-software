/**
 * @file    drv_ecc.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for ECC functions
 * @date    15/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_ecc Driver ECC
 * Functions to deal with the RAM ECC peripheral that corrects 
 * bit flip in RAM memory (STM32 H7 only)
 * @{
 */

#ifndef DRV_ECC_H
#define DRV_ECC_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

#if defined(ECC_AVAILABLE)
/** @brief Ecc instance type definition */
typedef RAMECC_HandleTypeDef eccInst_t;

/**
 * @enum    eccRamId_t
 * @brief   Enum defining buffers reference numbers
 */
typedef enum {
    ECC_AXI_SRAM,
    ECC_ITCM,
    ECC_DTCM0,
    ECC_DTCM1,
    ECC_SRAM1_0,
    ECC_SRAM1_1,
    ECC_SRAM2_0,
    ECC_SRAM2_1,
    ECC_SRAM3,
    ECC_SRAM4,
    ECC_BACKUP_SRAM,
    NB_ECCRAM,
} eccRamId_t;

#endif

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern coreStatus_t EccInit(void);

#endif /* DRV_ECC_H */

/** 
 * @} 
 * @} 
 */