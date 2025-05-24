/**
 * @file    common_types.h
 * @author  Merlin Kooshmanian
 * @brief   Common types and macros for the system
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

/******************************* Include Files *******************************/

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/***************************** Macros Definitions ****************************/

/*************************************/
/************* ATTRIBUTES ************/
/*************************************/

/**
 * @def     ATTR_PACKED
 * @brief   Force struct not to have padding
 */
#define ATTR_PACKED            __attribute__((packed))

/**
 * @def     ATTR_BYTE_ALIGNED
 * @brief   Force struct to be byte aligned
 */
#define ATTR_BYTE_ALIGNED      __attribute__((packed, aligned(1)))

/**
 * @def     ATTR_INLINE
 * @brief   Force function to be inlined
 */
#define ATTR_INLINE            inline __attribute__((always_inline))

/**
 * @def     ATTR_NAKED
 * @brief   Force function to be naked
 */
#define ATTR_NAKED             __attribute__((naked))

/**
 * @def     ATTR_PURE
 * @brief   Force function to be pure
 */
#define ATTR_PURE              __attribute__((pure))

/**
 * @def     ATTR_EXCEPTION
 * @brief   Exception handler required attributes
 */
#define ATTR_EXCEPTION         ATTR_NAKED

/**
 * @def     ATTR_SYSCALL
 * @brief   Syscalls required attributes
 */
#define ATTR_SYSCALL           IN_SYSCALL_SECTION ATTR_NAKED

/*************************************/
/************** SECTIONS *************/
/*************************************/

/**
 * @def     IN_SYSCALL_SECTION
 * @brief   Syscalls goes to .syscalls section
 */
#define IN_SYSCALL_SECTION     __attribute__((section(".syscalls")))

/**
 * @def     IN_DMABUFF_SECTION
 * @brief   DMA buffers goes to .dmabuff section
 */
#define IN_DMABUFF_SECTION     __attribute__((section(".dmabuff")))

/*************************************/
/*************** INFOS ***************/
/*************************************/

/**
 * @def      SOFTWARE_STATE_NOMINAL
 * @brief    Nominal state
 */
#define SOFTWARE_STATE_NOMINAL 0u

/**
 * @def      SOFTWARE_STATE_SAFE
 * @brief    Error state
 */
#define SOFTWARE_STATE_SAFE    1u

/***************************** Types Definitions *****************************/

/**
 * @enum    returnCode_t
 * @brief   General return codes for system components
 */
typedef enum
{
    RET_SUCCESSFUL    = 0u, /**< Operation successful */
    RET_ERROR         = 1u, /**< Operation failed */
    RET_INVALID_PARAM = 2u, /**< Invalid parameter */
    RET_NOT_AVAILABLE = 3u, /**< Resource is not available */
    RET_TIMEOUT       = 4u, /**< Operation timed out */
} returnCode_t;

/** @brief Time type definition */
typedef uint64_t time_t;

/** @brief Task tick type */
typedef uint32_t tick_t;

/** @brief Length type definition */
typedef uint32_t length_t;

/** @brief Data type definition */
typedef uint8_t *data_t;

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
 * @typedef     softwareState_t
 * @brief    Software state type
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

/**
 * @typedef  softwareVersion_t
 * @brief    Software version type
 */
typedef uint8_t softwareId_t;

/**
 * @struct   context_t
 * @brief    Context structure
 */
typedef struct
{
    softwareVersion_t version;        /**< @brief Software version */
    softwareState_t state;            /**< @brief Software state */
    softwareId_t safe_software_id;    /**< @brief Safe Software ID */
    softwareId_t nominal_software_id; /**< @brief Nominal Software ID */
    bootCount_t boot;                 /**< @brief Boot count */
    errorCount_t critical_error;      /**< @brief Failed boot count */
} ATTR_PACKED context_t;

#endif /* COMMON_TYPES_H */