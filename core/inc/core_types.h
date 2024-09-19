/**
 * @file    core_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for core
 * @date    06/05/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 */

#ifndef CORE_TYPES_H
#define CORE_TYPES_H

/******************************* Include Files *******************************/

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "autoconf.h"

/***************************** Macros Definitions ****************************/

#if !defined(BYTE_ALIGNED) && !defined(ASSERT_SIZE)
#define BYTE_ALIGNED                __attribute__((packed, aligned(1)))                                                 /**< Preprocessor function that force byte alignment for struct */
#define ASSERT_SIZE(object, size)   static_assert((sizeof(object) == (size)), "Object has not the expected size !");    /**< Preprocessor function that ensure objects have the expected size */
#endif

/***************************** Types Definitions *****************************/

/** 
 * @enum    coreStatus_t
 * @brief   Core functions specific returns 
 */
typedef enum
{
    CORE_SUCCESSFUL     = 0u,    /**< Function succeed */
    CORE_ERROR          = 1u,    /**< Function failed */
    CORE_INVALID_PARAM  = 2u,    /**< Function parameter is not valid */
    CORE_TIMEOUT        = 3u,    /**< Function returned a timeout */
    CORE_BUSY           = 4u,    /**< Function's ressource is busy */
} coreStatus_t;

/** @brief Time type definition */
typedef uint64_t time_t;

#endif /* CORE_TYPES_H */

/**
 * @}
 */