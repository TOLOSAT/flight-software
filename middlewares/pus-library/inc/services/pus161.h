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

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t InitS161(uint8_t number_of_task, monitoringSystemUsage_t **p_pus161_data);
extern returnCode_t ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t BuildS161SS2(pusTM_t *tm, uint8_t idle_time);
extern returnCode_t ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t BuildS161SS4(pusTM_t *tm, uint8_t highest_stack_consumer, uint8_t max_stack_usage);
extern returnCode_t ExecuteS161SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t BuildS161SS6(pusTM_t *tm, monitoringSystemUsage_t *system_usage);

#endif /* PUS161_H */

/** 
 * @}
 * @}
 * @}
 */