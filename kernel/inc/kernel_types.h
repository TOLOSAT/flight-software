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

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "autoconf.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_KERNEL_TEXT_SECTION        __attribute__((section(".text_kernel")))          /**< KERNEL functions goes in the .text_kernel */
#define IN_KERNEL_DATA_SECTION        __attribute__((section(".data_kernel")))          /**< KERNEL data goes in the .data_kernel */

// Table placement
#define IN_CONF_TABLES_SECTION      __attribute__((section(".conf_tables")))        /**< Conf table goes to .conf_tables section */
#define IN_DESC_TABLES_SECTION      __attribute__((section(".desc_tables")))        /**< Descriptor table goes to .desc_tables section */

// Other specific sections
#define IN_DMABUFF_SECTION          __attribute__((section(".dmabuff")))            /**< Temporary file goes to .dmabuff section */
#define IN_TMPFS_SECTION            __attribute__((section(".tmpfs")))              /**< Temporary file goes to .tmpfs section */
#define IN_TASK_STACKS_SECTION      __attribute__((section(".task_stacks")))        /**< Task stacks go to .task_stacks section */
#define IN_TASK_TCB_SECTION         __attribute__((section(".task_tcbs")))          /**< Task control block go to .task_tcbs section */
#define IN_BUFFER_ARRAYS_SECTION    __attribute__((section(".buffer_arrays")))      /**< Buffer data go to .buffer_arrays section */
#define IN_BUFFER_ENTITIES_SECTION  __attribute__((section(".buffer_entities")))    /**< Buffer data go to .buffer_entities section */
#define IN_MUTEX_QUEUE_SECTION      __attribute__((section(".mutex_queues")))       /**< Mutex queue go to .mutex_queues section */

#if !defined(BYTE_ALIGNED) && !defined(ASSERT_SIZE)
#define BYTE_ALIGNED                __attribute__((packed, aligned(1)))                                                 /**< Preprocessor function that force byte alignment for struct */
#define ASSERT_SIZE(object, size)   static_assert((sizeof(object) == (size)), "Object has not the expected size !");    /**< Preprocessor function that ensure objects have the expected size */
#endif

/***************************** Types Definitions *****************************/

/*******************************/
/******** GENERIC TYPES ********/
/*******************************/

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

/*******************************/
/********** CORE TYPES *********/
/*******************************/

/** @brief Time type definition */
typedef uint64_t time_t;

/*******************************/
/******* MONITORING TYPES ******/
/*******************************/

// TO DO : bring back the monitoring from PUS161 to here and do a proper check at init for MAX NB TASK
#define MON_MAX_TASK_NB 16u /**< Maximum number of task that can be monitored */

/** 
 * @struct  monitoringTaskInfo_t
 * @brief   Struct type definition of task info data struct
 */
typedef struct 
{
    uint8_t task_ref;       /**< @brief Task reference number */
    uint8_t stack_usage;    /**< @brief Current stack usage in percent */
    uint8_t time_usage;     /**< @brief Current time usage in percent */
    uint8_t task_mode;      /**< @brief Current task mode */
} monitoringTaskInfo_t;

/** 
 * @struct  monitoringSystemUsage_t
 * @brief   Struct type definition of system usage data struct
 */
typedef struct 
{
    uint8_t idle_time;                              /**< @brief Idle Time in percent */
    uint8_t highest_stack_consumer;                 /**< @brief Highest Stack Consumer */
    uint8_t max_stack_usage;                        /**< @brief Max Stack Usage in percent */
    uint8_t number_of_tasks;                        /**< @brief Actual number of tasks */
    monitoringTaskInfo_t system_report[MON_MAX_TASK_NB];   /**< @brief System report for each task */
} monitoringSystemUsage_t;

#endif /* KERNEL_TYPES_H */

/**
 * @}
 */