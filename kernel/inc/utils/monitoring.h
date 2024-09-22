/**
 * @file    monitoring.h
 * @author  Merlin Kooshmanian
 * @brief   Monitoring functions
 * @date    31/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup utils Utils
 * @{
 * @defgroup monitoring Monitoring
 * @brief Provides software monitoring handling interface.
 * @{
 */

#ifndef MONITORING_H
#define MONITORING_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

// TO DO : bring back the monitoring from PUS161 to here and do a proper check at init for MAX NB TASK
#define MON_MAX_TASK_NB 16u /**< Maximum number of task that can be monitored */

/***************************** Types Definitions *****************************/

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

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern kernelStatus_t InitMonitoring(void);
extern kernelStatus_t GetSystemUsage(monitoringSystemUsage_t *system_usage);

#endif /* MONITORING_H */

/** 
 * @}
 * @}
 * @}
 */