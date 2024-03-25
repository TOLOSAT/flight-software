/**
 * @file    boot_fdir.h
 * @author  Merlin Kooshmanian
 * @brief   Error Management functions for boot
 * @date    25/03/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef BOOT_FDIR_H
#define BOOT_FDIR_H

/******************************* Include Files *******************************/

#include <stdint.h>

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @enum    errorsSanction_t
 * @brief   Type of sanctions enum definition
 */
typedef enum
{
    FDIR_NO_SANCTION     = 0u,    /**< No sanction needed */
    FDIR_HALT_TASK       = 1u,    /**< Error needs current task to be halted (if possible) */
    FDIR_ERROR_HANDLER   = 2u,    /**< Error needs a reset of the system, but before system will save some data */
    FDIR_SYSTEM_RESET    = 3u,    /**< Error needs an immediate a reset of the system */
} errorsSanction_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

void CheckErrors(uint32_t status, errorsSanction_t sanction);
void Error_Handler(void);

#endif /* BOOT_FDIR_H */