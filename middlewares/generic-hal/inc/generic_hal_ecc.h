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

#ifndef GENERIC_HAL_ECC_H
#define GENERIC_HAL_ECC_H

/******************************* Include Files *******************************/

#include "generic_hal_types.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_ECC_TEXT_SECTION     __attribute__((section(".text_ecc")))   /**< ECC functions goes in the .text_ecc */
#define IN_ECC_DATA_SECTION     __attribute__((section(".data_ecc")))   /**< ECC data goes in the .data_ecc */

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

halStatus_t EccInit(void);

#endif /* GENERIC_HAL_ECC_H */

/** 
 * @} 
 * @} 
 */