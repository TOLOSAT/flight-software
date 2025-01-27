/**
 * @file    ecc.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for ECC functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup ecc ECC
 * @brief Error detection and correction interface.
 * @warning Used only if available on the target.
 * @{
 */

#ifndef ECC_H
#define ECC_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

#if defined(CONFIG_ECC)
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

extern void InitEcc(void);

#endif /* ECC_H */

/**
 * @}
 * @}
 * @}
 */