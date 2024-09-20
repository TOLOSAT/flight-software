/**
 * @file    pus161.h
 * @author  Clement Cognard & Merlin Kooshmanian
 * @brief   Header file for PUS 161 functions (MISO)
 * @date    08/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup pus161 PUS Service 161
 * @brief PUS service 161 (Internal Software Monitoring) implementation
 * @{
 */

#ifndef PUS161_H
#define PUS161_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief PUS161 task data type redefinition */
typedef monitoringTaskInfo_t pus161TaskInfo_t;

/** @brief PUS161 data type redefinition */
typedef monitoringSystemUsage_t pus161Data_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern pusStatus_t InitS161(uint8_t number_of_task, pus161Data_t **p_pus161_data);
extern pusStatus_t ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern pusStatus_t BuildS161SS2(pusTM_t *tm, uint8_t idle_time);
extern pusStatus_t ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern pusStatus_t BuildS161SS4(pusTM_t *tm, uint8_t highest_stack_consumer, uint8_t max_stack_usage);
extern pusStatus_t ExecuteS161SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern pusStatus_t BuildS161SS6(pusTM_t *tm, pus161Data_t *system_usage);

#endif /* PUS161_H */

/** 
 * @}
 * @}
 * @}
 */