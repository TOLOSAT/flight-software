/**
 * @file    kernel_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for kernel
 *
 * @copyright Copyright (c) TOLOSAT 2025
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

/*************************************/
/************** SECTIONS *************/
/*************************************/

/**
 * @def     IN_CONF_TABLES_SECTION
 * @brief   Configuration tables goes to .conf_tables section
 */
#define IN_CONF_TABLES_SECTION   __attribute__((section(".conf_tables")))

/**
 * @def     IN_DESC_TABLES_SECTION
 * @brief   Descriptor tables goes to .conf_tables section
 */
#define IN_DESC_TABLES_SECTION   __attribute__((section(".desc_tables")))

/**
 * @def     IN_TASK_STACKS_SECTION
 * @brief   Task stacks go to .task_stacks section
 */
#define IN_TASK_STACKS_SECTION   __attribute__((section(".task_stacks")))

/**
 * @def     IN_TASK_TCB_SECTION
 * @brief   Task control block go to .task_tcbs section
 */
#define IN_TASK_TCB_SECTION      __attribute__((section(".task_tcbs")))

/**
 * @def     IN_BUFFER_ARRAYS_SECTION
 * @brief   Buffer data go to .buffer_arrays section
 */
#define IN_BUFFER_ARRAYS_SECTION __attribute__((section(".buffer_arrays")))

/**
 * @def     IN_BUFFER_QUEUES_SECTION
 * @brief   Buffer queues go to .buffer_queues section
 */
#define IN_BUFFER_QUEUES_SECTION __attribute__((section(".buffer_queues")))

/**
 * @def     IN_MUTEX_QUEUE_SECTION
 * @brief   Mutex queue go to .mutex_queues section
 */
#define IN_MUTEX_QUEUE_SECTION   __attribute__((section(".mutex_queues")))

/**
 * @def     IN_TIMER_BUFFERS_SECTION
 * @brief   Timer buffers file goes to .tim_buffers section
 */
#define IN_TIMER_BUFFERS_SECTION __attribute__((section(".tim_buffers")))

/**
 * @def     IN_TMPFS_SECTION
 * @brief   Temporary file goes to .tmpfs section
 */
#define IN_TMPFS_SECTION         __attribute__((section(".tmpfs")))

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
    DEVICE_TYPE_BUFFER     = 0u, /**< Device is linked to a buffer */
    DEVICE_TYPE_FILE       = 1u, /**< Device is linked to a file */
    DEVICE_TYPE_PERIPHERAL = 2u, /**< Device is linked to a peripheral */
    DEVICE_TYPE_SYSTEM     = 3u, /**< Device is linked to a system device (e.g. kernel internal data) */
} deviceType_t;

/** @brief Timer reference number type definition */
typedef uint32_t timerNo_t;

/**
 * @enum  timerMode_t
 * @brief   Enum type describing a timer's mode
 */
typedef enum
{
    TIMER_ONESHOT  = 0u, /**< Timer is in ONESHOT mode */
    TIMER_PERIODIC = 1u, /**< Timer is in PERIODIC mode */
} timerMode_t;

/** @brief Signal mask type definition */
typedef uint32_t signalMask_t;

/**
 * @struct  taskUsage_t
 * @brief   Struct type definition of a single task usage data struct
 */
typedef struct
{
    uint8_t task_ref;    /**< @brief Task reference number */
    uint8_t stack_usage; /**< @brief Current stack usage in percent */
    uint8_t time_usage;  /**< @brief Current time usage in percent */
    uint8_t task_mode;   /**< @brief Current task mode */
} taskUsage_t;

/**
 * @struct  systemUsage_t
 * @brief   Struct type definition of system usage data struct
 */
typedef struct
{
    uint8_t idle_time;                           /**< @brief Idle Time in percent */
    uint8_t highest_stack_consumer;              /**< @brief Highest Stack Consumer */
    uint8_t max_stack_usage;                     /**< @brief Max Stack Usage in percent */
    uint8_t number_of_tasks;                     /**< @brief Actual number of tasks */
} systemUsage_t;

/** @brief HK reference number type */
typedef uint32_t hkId_t;

/**
 * @struct  hk_t
 * @brief   Struct type of an housekeeping
 */
typedef struct
{
    hkId_t hkid;    /**< @brief Housekeeping ID */
    uint32_t value; /**< @brief Housekeeping Observable Value */
    time_t time;    /**< @brief Current Time */
} hk_t;

/**
 * @enum    descStatus_t
 * @brief   Enum type for descriptor status
 */
typedef enum
{
    DESC_FREE = 0u, /**< Descriptor is free */
    DESC_USED = 1u, /**< Descriptor is used */
} descStatus_t;

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
 * @typedef softwareState_t
 * @brief   Software state type
 */
typedef uint8_t softwareState_t;

/**
 * @typedef  softwareId_t
 * @brief    Software id
 */
typedef uint8_t softwareId_t;

/**
 * @brief Structure to store saved CPU registers during an error.
 */
typedef struct
{
    uint32_t r[4]; /**< General-purpose registers R0-R3.    */
    uint32_t r12;  /**< Register R12.                       */
    uint32_t lr;   /**< Link register (LR).                 */
    uint32_t pc;   /**< Program counter (PC).               */
    uint32_t xpsr; /**< Program status register (xPSR).     */
} ATTR_PACKED savedRegisters_t;

/**
 * @brief Contains the address and offset of a resolved function call frame.
 */
typedef struct
{
    uint32_t function; /**< Start address of the resolved function. */
    uint32_t offset;   /**< Offset from the function start (PC - functionStart). */
} ATTR_PACKED call_t;

/**
 * @brief Structure to represent the call stack.
 */
typedef struct
{
    uint32_t calls_nb;                 /**< Number of calls */
    call_t calls[CALL_STACK_MAX_SIZE]; /**< Array of calls */
} ATTR_PACKED callStack_t;

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
    uint8_t bnco;                     /**< @brief Padding field named after the best burger in Toulouse. */
    uint32_t boot;                    /**< @brief Boot count */
    uint32_t critical_error;          /**< @brief Critical error count */
    uint32_t cfsr;                    /**< @brief Configurable Fault Status Register. */
    uint32_t hfsr;                    /**< @brief Hard Fault Status Register.         */
    savedRegisters_t registers;       /**< @brief Saved registers */
    callStack_t call_stack;           /**< @brief Call stack */
} ATTR_PACKED context_t;

#endif /* KERNEL_TYPES_H */

/**
 * @}
 */