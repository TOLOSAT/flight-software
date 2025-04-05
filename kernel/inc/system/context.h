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
#include "system/sysinfo.h"

/***************************** Macros Definitions ****************************/

/************************** Context Types Definitions ************************/

/**
 * @enum     softwareState_t
 * @brief    Software state enumeration
 */
typedef enum
{
    SOFTWARE_STATE_NOMINAL = 0u, /**< Software is in nominal state */
    SOFTWARE_STATE_ERROR   = 1u, /**< Software is in error state */
} softwareState_t;

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
    softwareVersion_t version; /**< Software version */
    softwareState_t state;     /**< Software state */
    bootCount_t boot;          /**< Boot count */
    bootCount_t failedBoot;    /**< Failed boot count */
} ATTR_PACKED context_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void InitContext(void);
extern returnCode_t ReadContext(context_t *context);
extern returnCode_t WriteContext(context_t context);

#endif /* CONTEXT_H */

/**
 * @}
 * @}
 * @}
 */
