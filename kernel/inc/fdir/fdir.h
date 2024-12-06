/**
 * @file    fdir.h
 * @author  Théo Bessel & Merlin Kooshmanian
 * @brief   Interface for Failure Detection, Identification and Recovery (FDIR).
 */

#ifndef FDIR_H
#define FDIR_H

/******************************* Include Files *******************************/

#include "stacktrace.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @brief Structure to store saved CPU registers during an error.
 */
typedef struct __attribute__((packed))
{
    uint32_t r[4];                  /**< General-purpose registers R0-R3.    */
    uint32_t r12;                   /**< Register R12.                       */
    uint32_t lr;                    /**< Link register (LR).                 */
    uint32_t pc;                    /**< Program counter (PC).               */
    uint32_t xpsr;                  /**< Program status register (xPSR).     */
} savedRegisters_t;

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
extern void SaveRegisters(debugInfo_t* debug_info);
extern void PrepareUnwind(call_t* last_call);

/*************************** Functions Declarations **************************/

#endif /* FDIR_H */
