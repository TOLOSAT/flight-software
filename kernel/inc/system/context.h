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

/***************************** Macros Definitions ****************************/

/************************** Context Types Definitions ************************/

/**
 * @struct   softwareVersion_t
 * @brief    Software version structure
 */
typedef struct
{
    uint8_t major; /**< Major version */
    uint8_t minor; /**< Minor version */
    uint8_t patch; /**< Patch version */
    uint8_t flag;  /**< Additional informations */
} ATTR_PACKED softwareVersion_t;

/**
 * @enum     softwareState_t
 * @brief    Software state enumeration
 */
typedef enum
{
    SOFTWARE_STATE_NOMINAL = 0, /**< Software is in nominal state */
    SOFTWARE_STATE_ERROR   = 1, /**< Software is in error state */
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
} ATTR_PACKED context_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void SaveContext(context_t *context);
extern context_t ReadContext(void);

/*********************** Getters and Setters Declarations ********************/

extern softwareVersion_t GetContextSoftwareVersion(void);
extern softwareState_t GetContextSoftwareState(void);
extern bootCount_t GetContextBootCount(void);
extern void SetContextSoftwareVersion(softwareVersion_t version);
extern void SetContextSoftwareState(softwareState_t state);
extern void SetContextBootCount(bootCount_t boot);

#endif /* CONTEXT_H */

/**
 * @}
 * @}
 * @}
 */
