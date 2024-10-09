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

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void CheckError(returnCode_t retcode);
extern void ErrorHandler(void);

#endif /* FDIR_H */

/** 
 * @}
 * @}
 * @}
 */