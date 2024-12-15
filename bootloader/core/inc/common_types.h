/**
 * @file    common_types.h
 * @author  Merlin Kooshmanian
 * @brief   Common types and macros for the system
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/***************************** Macros Definitions ****************************/

// Attributes
#define ATTR_PACKED         __attribute__((packed))                 /**< Force struct not to have padding */
#define ATTR_BYTE_ALIGNED   __attribute__((packed, aligned(1)))     /**< Force struct to be byte aligned */
#define ATTR_INLINE         inline __attribute__((always_inline))   /**< Force function to be inlined */
#define ATTR_NAKED          __attribute__((naked))                  /**< Force function to be naked */
#define ATTR_PURE           __attribute__((pure))                   /**< Force function to be pure */
#define ATTR_EXCEPTION      ATTR_NAKED                              /**< Exception handler required attributes */
#define ATTR_SYSCALL        IN_SYSCALL_SECTION ATTR_NAKED           /**< Syscalls required attributes */

// Assertion
#define ASSERT_SIZE(object, size)   static_assert((sizeof(object) == (size)), "Object has not the expected size !");    /**< Ensure objects have the expected size */

// Section
#define IN_SYSCALL_SECTION  __attribute__((section(".syscalls")))   /**< Syscalls goes to .syscalls section */
#define IN_DMABUFF_SECTION  __attribute__((section(".dmabuff")))    /**< DMA buffers goes to .dmabuff section */

/***************************** Types Definitions *****************************/

/** 
 * @enum    returnCode_t
 * @brief   General return codes for system components
 */
typedef enum
{
    RET_SUCCESSFUL     = 0u,    /**< Operation successful */
    RET_ERROR          = 1u,    /**< Operation failed */
    RET_INVALID_PARAM  = 2u,    /**< Invalid parameter */
    RET_NOT_AVAILABLE  = 3u,    /**< Resource is not available */
    RET_TIMEOUT        = 4u,    /**< Operation timed out */
} returnCode_t;

/** @brief Time type definition */
typedef uint64_t time_t;

/** @brief Task tick type */
typedef uint32_t tick_t;

/** @brief Length type definition */
typedef uint32_t length_t;

/** @brief Data type definition */
typedef uint8_t* data_t;

#endif /* COMMON_TYPES_H */
