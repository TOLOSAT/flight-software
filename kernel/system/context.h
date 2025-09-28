/**
 * @file    context.h
 * @author  Théo Bessel
 * @brief   Header file for Context API
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup context Context
 * @brief Interface for saving and restoring kernel context.
 * @{
 */

#ifndef CONTEXT_H
#define CONTEXT_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"
#include "fdir/fdir.h"
#include "system/sysinfo.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void InitContext(void);
extern returnCode_t ReadContext(context_t *context);
extern returnCode_t WriteContext(context_t *context);
extern returnCode_t EraseContext(void);

#endif /* CONTEXT_H */

/**
 * @}
 * @}
 * @}
 */
