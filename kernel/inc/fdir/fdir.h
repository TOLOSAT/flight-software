/**
 * @file    fdir.h
 * @author  Merlin Kooshmanian
 * @brief   Error Management functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup fdir FDIR
 * @{
 * @defgroup fdir-handling FDIR Handling
 * @brief Failure Detection, Identification and Recovery (FDIR) handling interface.
 * @{
 */

#ifndef FDIR_H
#define FDIR_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

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

extern void CheckErrors(returnCode_t retcode, errorsSanction_t sanction);
extern void ErrorHandler(void);

#endif /* FDIR_H */

/** 
 * @}
 * @}
 * @}
 */