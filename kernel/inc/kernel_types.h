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
    DEVICE_TYPE_SYSTEM = 3u,        /**< Device is linked to a system device (e.g. kernel internal data) */
} deviceType_t;

/** @brief Signal mask type definition */
typedef uint32_t signalMask_t;

/**
 * @struct  taskUsage_t
 * @brief   Struct type definition of a single task usage data struct
 */
typedef struct
{
    uint8_t task_ref;       /**< @brief Task reference number */
    uint8_t stack_usage;    /**< @brief Current stack usage in percent */
    uint8_t time_usage;     /**< @brief Current time usage in percent */
    uint8_t task_mode;      /**< @brief Current task mode */
} taskUsage_t;

/**
 * @struct  systemUsage_t
 * @brief   Struct type definition of system usage data struct
 */
typedef struct
{
    uint8_t idle_time;                  /**< @brief Idle Time in percent */
    uint8_t highest_stack_consumer;     /**< @brief Highest Stack Consumer */
    uint8_t max_stack_usage;            /**< @brief Max Stack Usage in percent */
    uint8_t number_of_tasks;            /**< @brief Actual number of tasks */
    taskUsage_t task_usage[NB_TASKS];   /**< @brief System report for each task */
} systemUsage_t;

/** @brief HK reference number type */
typedef uint32_t hkId_t;

/**
 * @struct  hk_t
 * @brief   Struct type of an housekeeping
 */
typedef struct
{
    uint8_t task_ref;       /**< @brief Task Reference Number */
    uint8_t observable;     /**< @brief Housekeeping Observable */
    uint32_t value;         /**< @brief Housekeeping Observable Value */
    time_t time;         /**< @brief Current Time */
} hk_t;

#endif /* KERNEL_TYPES_H */

/**
 * @}
 */