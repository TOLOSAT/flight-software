/**
 * @file    pus161.h
 * @author  Clement Cognard & Merlin Kooshmanian
 * @brief   Header file for PUS 161 functions (MISO)
 * @date    08/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef PUS161_H
#define PUS161_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

#define PUS161_MAX_TASK_NB      32u /**< Maximum number of tasks supported by PUS161 for a system report */

/***************************** Types Definitions *****************************/

/** 
 * @struct  pus161TaskInfo_t
 * @brief   Struct type definition of PUS161 data
 */
typedef struct 
{
    uint8_t task_ref;       /**< @brief Task reference number */
    uint8_t stack_usage;    /**< @brief Current stack usage in percent */
    uint8_t time_usage;     /**< @brief Current time usage in percent */
    uint8_t task_mode;      /**< @brief Current task mode */
} pus161TaskInfo_t;

/** 
 * @struct  pus161Data_t
 * @brief   Struct type definition of PUS161 data
 */
typedef struct 
{
    uint8_t idle_time;                                  /**< @brief Idle Time in percent */
    uint8_t highest_stack_consumer;                     /**< @brief Highest Stack Consumer */
    uint8_t max_stack_usage;                            /**< @brief Max Stack Usage in percent */
    uint8_t number_of_tasks;                            /**< @brief Actual number of tasks */
    pus161TaskInfo_t system_report[PUS161_MAX_TASK_NB]; /**< @brief System report for each task */
} pus161Data_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

pusStatus_t InitS161(pus161Data_t *system_usage);
pusStatus_t ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
pusStatus_t BuildS161SS2(pusTM_t *tm, uint8_t idle_time);
pusStatus_t ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
pusStatus_t BuildS161SS4(pusTM_t *tm, uint8_t highest_stack_consumer, uint8_t max_stack_usage);
pusStatus_t ExecuteS161SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
pusStatus_t BuildS161SS6(pusTM_t *tm, pus161Data_t *system_usage);

#endif /* PUS161_H */
