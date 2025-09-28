/**
 * @file    fdir.h
 * @author  Merlin Kooshmanian
 * @author  Théo Bessel
 * @brief   Header file for Failure Detection, Identification and Recovery (FDIR).
 *
 * @copyright Copyright (c) TOLOSAT 2025
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

#include "autoconf.h"
#include "kernel_types.h"
#include "stacktrace.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @brief General debug information captured during an error.
 */
typedef struct
{
    savedRegisters_t *registers; /**< Pointer to saved CPU registers.     */
    uint32_t cfsr;               /**< Configurable Fault Status Register. */
    uint32_t hfsr;               /**< Hard Fault Status Register.         */
    callStack_t call_stack;      /**< Captured call stack.                */
} debugInfo_t;

/*************************** Variables Declarations **************************/

extern void InitFDIR(void);
extern void CheckError(returnCode_t retcode);
extern void ErrorHandler(void);
extern void KernelPanic(void);
extern void SystemReset(void);

/*************************** Functions Declarations **************************/

#endif /* FDIR_H */

/**
 * @}
 * @}
 * @}
 */