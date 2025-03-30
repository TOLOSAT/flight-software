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
#define ATTR_PACKED        __attribute__((packed))

/**
 * @def     ATTR_BYTE_ALIGNED
 * @brief   Force struct to be byte aligned
 */
#define ATTR_BYTE_ALIGNED  __attribute__((packed, aligned(1)))

/**
 * @def     ATTR_INLINE
 * @brief   Force function to be inlined
 */
#define ATTR_INLINE        inline __attribute__((always_inline))

/**
 * @def     ATTR_NAKED
 * @brief   Force function to be naked
 */
#define ATTR_NAKED         __attribute__((naked))

/**
 * @def     ATTR_PURE
 * @brief   Force function to be pure
 */
#define ATTR_PURE          __attribute__((pure))

/**
 * @def     ATTR_EXCEPTION
 * @brief   Exception handler required attributes
 */
#define ATTR_EXCEPTION     ATTR_NAKED

/**
 * @def     ATTR_SYSCALL
 * @brief   Syscalls required attributes
 */
#define ATTR_SYSCALL       IN_SYSCALL_SECTION ATTR_NAKED

/*************************************/
/************** SECTIONS *************/
/*************************************/

/**
 * @def     IN_SYSCALL_SECTION
 * @brief   Syscalls goes to .syscalls section
 */
#define IN_SYSCALL_SECTION __attribute__((section(".syscalls")))

/**
 * @def     IN_DMABUFF_SECTION
 * @brief   DMA buffers goes to .dmabuff section
 */
#define IN_DMABUFF_SECTION __attribute__((section(".dmabuff")))

/*************************************/
/************* UTILITIES *************/
/*************************************/

/**
 * @def     STR
 * @brief   Convert a macro to a string
 */
#define STR(x)             #x

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

/**************************** Assertion Definition ***************************/

/**
 * @def     ASSERT_SIZE
 * @brief   Ensure objects have the expected size
 */
#define ASSERT_SIZE(object, size) static_assert((sizeof(object) == (size)), "Object has not the expected size !");

#endif /* COMMON_TYPES_H */