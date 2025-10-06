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

/**
 * @def     IN_CONFIG_SECTION
 * @brief   Configuration tables goes to .config section
 */
#define IN_CONFIG_SECTION __attribute__((section(".config")))

/***************************** Types Definitions *****************************/

/*************************************/
/*************** TASKS ***************/
/*************************************/

/** @brief Task reference number type definition */
typedef uint32_t taskNo_t;

/** @brief Task Name type */
typedef const char taskName_t;

/** @brief Task Function type */
typedef void (*taskFunction_t)(void *arg);

/** @brief Task Stack Size type */
typedef uint32_t taskStackSize_t;

/** @brief Task stack type */
typedef uint32_t taskStack_t;

/** @brief Task Priority type */
typedef uint32_t taskPriority_t;

/**
 * @enum    taskPrivilege_t
 * @brief   Task Privilege
 */
typedef enum
{
    TASK_UNPRIVILEGED = 0u, /**< Task is not priviledged */
    TASK_PRIVILEGED   = 1u, /**< Task is priviledged */
} taskPrivilege_t;

/**
 * @struct  taskConf_t
 * @brief   Struct type of a task configuration
 */
typedef struct
{
    taskNo_t task;              /**< @brief Task reference number */
    taskName_t *name;           /**< @brief Task name only for debugging purposes */
    taskFunction_t function;    /**< @brief Task main function */
    taskPriority_t priority;    /**< @brief Task priority */
    taskStackSize_t stack_size; /**< @brief Task stack size in bits */
    tick_t default_period;      /**< @brief Task default period in ticks */
    taskPrivilege_t privilege;  /**< @brief Task privilege (applicable only if the MPU is activated) */
    taskStack_t *p_stack;       /**< @brief Pointer to task stack */
} taskConf_t;

/*************************************/
/************** BUFFERS **************/
/*************************************/

/** @brief Buffer reference number type definition */
typedef uint32_t bufferNo_t;

/**
 * @struct  bufferConf_t
 * @struct  bufferConf_t
 * @brief   Struct type definition of a buffer
 */
typedef struct
{
    bufferNo_t buffer; /**< @brief Buffer reference number */
    taskNo_t sender;   /**< @brief Task reference number of the sender */
    taskNo_t receiver; /**< @brief Task reference number of the receiver */
    length_t max_size; /**< @brief Maximum message size the buffer can handle */
    length_t max_nb;   /**< @brief Maximum number of message the buffer can handle */
} bufferConf_t;

/*************************************/
/************** MUTEXES **************/
/*************************************/

/** @brief Mutex reference number type definition */
typedef uint32_t mutexNo_t;

/**
 * @struct  mutexConf_t
 * @brief   Struct type of a mutex configuration
 */
typedef struct
{
    mutexNo_t mutex; /**< @brief Mutex reference number */
} mutexConf_t;

/*************************************/
/*************** FILES ***************/
/*************************************/

/** @brief File reference number type definition */
typedef uint32_t fileNo_t;

/** @brief FS file Name type */
typedef const char fsfileName_t;

/** @brief FS file access mode type */
typedef uint8_t fsfileAccessMode_t;

/**
 * @struct  fsFileConf_t
 * @brief   Struct type of a file configuration
 */
typedef struct
{
    fileNo_t file;                  /**< @brief File reference number */
    fsfileName_t *name;             /**< @brief File name */
    fsfileAccessMode_t access_mode; /**< @brief File access mode */
} fsFileConf_t;

/*************************************/
/*************** TIMERS **************/
/*************************************/

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

/**
 * @struct  timerConf_t
 * @brief   Struct type definition of a timer
 */
typedef struct
{
    timerNo_t timer; /**< @brief Timer reference number as it is declared in TIMERS_ENUM */
    taskNo_t owner;  /**< @brief Task reference number of the owner */
} timerConf_t;

/*************************************/
/************** DEVICES **************/
/*************************************/

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

/**
 * @enum    descStatus_t
 * @brief   Enum type for descriptor status
 */
typedef enum
{
    DESC_FREE = 0u, /**< Descriptor is free */
    DESC_USED = 1u, /**< Descriptor is used */
} descStatus_t;

/*************************************/
/************** SIGNALS **************/
/*************************************/

/** @brief Signal mask type definition */
typedef uint32_t signalMask_t;

/*************************************/
/*************** USAGE ***************/
/*************************************/

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
    uint8_t idle_time;              /**< @brief Idle Time in percent */
    uint8_t highest_stack_consumer; /**< @brief Highest Stack Consumer */
    uint8_t max_stack_usage;        /**< @brief Max Stack Usage in percent */
    uint8_t number_of_tasks;        /**< @brief Actual number of tasks */
} systemUsage_t;

/*************************************/
/*************** DEBUG ***************/
/*************************************/

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

/*************************************/
/************** CONTEXT **************/
/*************************************/

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