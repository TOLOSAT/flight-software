/**
 * @file    kernel_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for kernel
 * @date    06/05/2024
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
#include "autoconf.h"

/***************************** Macros Definitions ****************************/

// Table placement
#define IN_CONF_TABLES_SECTION      __attribute__((section(".conf_tables")))        /**< Conf table goes to .conf_tables section */
#define IN_DESC_TABLES_SECTION      __attribute__((section(".desc_tables")))        /**< Descriptor table goes to .desc_tables section */

/***************************** Types Definitions *****************************/

/** 
 * @enum    kernelStatus_t
 * @brief   Kernel functions specific returns 
 */
typedef enum
{
    KERNEL_SUCCESSFUL     = 0u,    /**< Function succeed */
    KERNEL_ERROR          = 1u,    /**< Function failed */
    KERNEL_INVALID_PARAM  = 2u,    /**< Function parameter is not valid */
    KERNEL_TIMEOUT        = 3u,    /**< Function returned a timeout */
    KERNEL_BUSY           = 4u,    /**< Function's ressource is busy */
} kernelStatus_t;

/** @brief Task reference number type definition */
typedef uint32_t taskNo_t;

/** @brief Mutex reference number type definition */
typedef uint32_t mutexNo_t;

/** @brief Buffer reference number type definition */
typedef uint32_t bufferNo_t;

/** @brief Device reference number type definition */
typedef uint32_t deviceNo_t;

/** @brief File reference number type definition */
typedef uint32_t fileNo_t;

#endif /* KERNEL_TYPES_H */

/**
 * @}
 */