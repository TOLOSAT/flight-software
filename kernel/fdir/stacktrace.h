/**
 * @file    stacktrace.h
 * @author  Théo Bessel
 * @brief   Interface for stack trace handling
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef STACKTRACE_H
#define STACKTRACE_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @struct  exidxEntry_t
 * @brief   Structure that handle exidx raw and decoded entries
 */
typedef struct
{
    uint32_t exidx_fn;    /**< Function in the eidx table */
    uint32_t extab_entry; /**< Entry toward the extab table */
} exidxEntry_t;

/**
 * @brief Represents the minimal context needed to unwind a stack frame.
 */
typedef struct
{
    uint32_t fp; /**< Frame pointer (FP) of the frame */
    uint32_t lr; /**< Link register (LR) of the frame */
} stackContext_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void UnwindStack(stackContext_t last_context, callStack_t *call_stack);

#endif /* STACKTRACE_H */