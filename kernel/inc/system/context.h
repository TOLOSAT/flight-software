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
 * @typedef softwareState_t
 * @brief   Software state type
 */
typedef uint32_t softwareState_t;

/**
 * @typedef  bootCount_t
 * @brief    Boot count type
 */
typedef uint32_t bootCount_t;

/**
 * @typedef  errorCount_t
 * @brief    Error count type
 */
typedef uint32_t errorCount_t;

/***************************** Types Definitions *****************************/

/**
 * @struct   context_t
 * @brief    Context structure
 */
typedef struct
{
    softwareVersion_t version;  /**< @brief Software version */
    softwareState_t state;      /**< @brief Software state */
    uint8_t software_id;        /**< @brief Software ID */
    bootCount_t boot;           /**< @brief Boot count */
    errorCount_t critical_error;      /**< @brief Failed boot count */
    uint32_t cfsr;              /**< @brief Configurable Fault Status Register. */
    uint32_t hfsr;              /**< @brief Hard Fault Status Register.         */
    savedRegisters_t registers; /**< @brief Saved registers */
    callStack_t call_stack;     /**< @brief Call stack */
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
