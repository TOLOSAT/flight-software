/**
 * @file    common_types.h
 * @author  Merlin Kooshmanian
 * @brief   Common types and macros for the system
 * @date    02/10/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

/***************************** Macros Definitions ****************************/

#define BYTE_ALIGNED                __attribute__((packed, aligned(1)))     /**< Force byte alignment for struct */
#define ASSERT_SIZE(object, size)   static_assert((sizeof(object) == (size)), "Object has not the expected size !");    /**< Ensure objects have the expected size */

#define IN_DMABUFF_SECTION          __attribute__((section(".dmabuff")))    /**< Temporary file goes to .dmabuff section */

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
