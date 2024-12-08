/**
 * @file    fdir.h
 * @author  Merlin Kooshmanian
 * @author  Théo Bessel
 * @brief   Header file for Failure Detection, Identification and Recovery (FDIR).
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
#include "stacktrace.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @brief Structure to store saved CPU registers during an error.
 */
typedef struct
{
    uint32_t r[4];                  /**< General-purpose registers R0-R3.    */
    uint32_t r12;                   /**< Register R12.                       */
    uint32_t lr;                    /**< Link register (LR).                 */
    uint32_t pc;                    /**< Program counter (PC).               */
    uint32_t xpsr;                  /**< Program status register (xPSR).     */
} ATTR_PACKED savedRegisters_t;

/**
 * @brief General debug information captured during an error.
 */
typedef struct
{
    savedRegisters_t* registers;    /**< Pointer to saved CPU registers.     */
    uint32_t cfsr;                  /**< Configurable Fault Status Register. */
    uint32_t hfsr;                  /**< Hard Fault Status Register.         */
    callStack_t call_stack;         /**< Captured call stack.                */
} debugInfo_t;

/*************************** Variables Declarations **************************/

extern void InitFDIR(void);
extern void CheckError(returnCode_t retcode);
extern void ErrorHandler(void);

/*************************** Functions Declarations **************************/

#endif /* FDIR_H */

/** 
 * @}
 * @}
 * @}
 */