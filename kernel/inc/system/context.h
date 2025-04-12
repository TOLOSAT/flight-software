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

#include "kernel_types.h"
#include "fdir/fdir.h"
#include "system/sysinfo.h"

/***************************** Macros Definitions ****************************/

/**
 * @def      SOFTWARE_STATE_NOMINAL
 * @brief    Nominal state
 */
#define SOFTWARE_STATE_NOMINAL 0u

/**
 * @def      SOFTWARE_STATE_ERROR
 * @brief    Error state
 */
#define SOFTWARE_STATE_ERROR   1u

/************************** Context Types Definitions ************************/

/**
 * @typedef     softwareState_t
 * @brief    Software state type
 */
typedef uint32_t softwareState_t;

/**
 * @typedef  bootCount_t
 * @brief    Boot count type
 */
typedef uint32_t bootCount_t;

/***************************** Types Definitions *****************************/

/**
 * @struct   context_t
 * @brief    Context structure
 */
typedef struct
{
    softwareVersion_t version;  /**< Software version */
    softwareState_t state;      /**< Software state */
    bootCount_t boot;           /**< Boot count */
    bootCount_t failedBoot;     /**< Failed boot count */
    uint32_t cfsr;              /**< Configurable Fault Status Register. */
    uint32_t hfsr;              /**< Hard Fault Status Register.         */
    savedRegisters_t registers; /**< Saved registers */
    callStack_t callStack;      /**< Call stack */
} ATTR_PACKED context_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void InitContext(void);
extern returnCode_t ReadContext(context_t *context);
extern returnCode_t WriteContext(context_t *context);

#endif /* CONTEXT_H */

/**
 * @}
 * @}
 * @}
 */
