/**
 * @file    stacktrace.h
 * @author  Théo Bessel
 * @brief   Interface for stack trace handling
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef STACKTRACE_H
#define STACKTRACE_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

#define CALL_STACK_MAX_SIZE 20u /**< Maximum size for the call stack (arbitrary) */

/***************************** Types Definitions *****************************/

/**
 * @struct  exidxEntry_t
 * @brief   Structure that handle exidx raw and decoded entries
 */
typedef struct
{
    uint32_t exidx_fn;     /**< Function in the eidx table */
    uint32_t extab_entry;  /**< Entry toward the extab table */
} exidxEntry_t;

/**
 * @brief Structure to store details of a single stack frame.
 */
typedef struct
{
    uint32_t lr;    /**< Link register (LR) of the frame */
    uint32_t fp;    /**< Frame pointer (FP) of the frame */
} ATTR_PACKED call_t;

/**
 * @brief Structure to represent the call stack.
 */
typedef struct
{
    uint32_t last_idx;                  /**< Index of the last frame */
    call_t calls[CALL_STACK_MAX_SIZE];  /**< Array of captured frames */
} ATTR_PACKED callStack_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void UnwindStackFromContext(callStack_t* call_stack, call_t last_call);

#endif /* STACKTRACE_H */