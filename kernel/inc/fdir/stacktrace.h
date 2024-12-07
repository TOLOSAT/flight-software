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
typedef struct __attribute__((packed))
{
    uint32_t exidx_entry;       /**< Entry in the eidx table */
    uint32_t exidx_fn;          /**< Function in the eidx table */
    uint32_t decoded_entry;     /**< Decoded entry (after prel31 decoding) */
    uint32_t decoded_fn;        /**< Decoded function (after prel31 decoding) */
} exidxEntry_t;

/**
 * @brief Structure to store details of a single stack frame.
 */
typedef struct __attribute__((packed))
{
    uint32_t lr;    /**< Link register (LR) of the frame */
    uint32_t fp;    /**< Frame pointer (FP) of the frame */
} call_t;

/**
 * @brief Structure to represent the call stack.
 */
typedef struct __attribute__((packed))
{
    uint32_t size;                      /**< Number of valid frames */
    call_t calls[CALL_STACK_MAX_SIZE];  /**< Array of captured frames */
} callStack_t;

/*************************** Variables Declarations **************************/

/**
 * @brief Start address of `.ARM.exidx` section.
 */
extern uint32_t __exidx_start;

/**
 * @brief End address of `.ARM.exidx` section.
 */
extern uint32_t __exidx_end;

/**
 * @brief Start address of `.ARM.extab` section.
 */
extern uint32_t __extab_start;

/**
 * @brief End address of `.ARM.extab` section.
 */
extern uint32_t __extab_end;

/*************************** Functions Declarations **************************/

extern void UnwindStack(callStack_t* call_stack, call_t last_call);

#endif /* STACKTRACE_H */