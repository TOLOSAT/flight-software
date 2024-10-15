/**
 * @file    kernel_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for kernel
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 */

#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

/******************************* Include Files *******************************/

#include "common_types.h"
#include "kernel_constants.h"
#include "autoconf.h"

/***************************** Macros Definitions ****************************/

// Table placement
#define IN_CONF_TABLES_SECTION      __attribute__((section(".conf_tables")))        /**< Conf table goes to .conf_tables section */
#define IN_DESC_TABLES_SECTION      __attribute__((section(".desc_tables")))        /**< Descriptor table goes to .desc_tables section */

/***************************** Types Definitions *****************************/

/** @brief Task reference number type definition */
typedef uint32_t taskNo_t;

/** @brief Task Priority type */
typedef uint32_t taskPriority_t;

/** @brief Mutex reference number type definition */
typedef uint32_t mutexNo_t;

/** @brief Buffer reference number type definition */
typedef uint32_t bufferNo_t;

/** @brief File reference number type definition */
typedef uint32_t fileNo_t;

/** @brief Device reference number type definition */
typedef uint32_t deviceNo_t;

/** 
 * @enum    deviceType_t
 * @brief   Device type typedef enum
 */
typedef enum
{
    DEVICE_TYPE_BUFFER = 0u,        /**< Device is linked to a buffer */
    DEVICE_TYPE_FILE = 1u,          /**< Device is linked to a file */
    DEVICE_TYPE_PERIPHERAL = 2u,    /**< Device is linked to a peripheral */
} deviceType_t;

#endif /* KERNEL_TYPES_H */

/**
 * @}
 */