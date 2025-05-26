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

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void UnwindStackFromContext(callStack_t *call_stack, call_t last_call);

#endif /* STACKTRACE_H */