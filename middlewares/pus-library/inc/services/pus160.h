/**
 * @file    pus160.h
 * @author  Théo Bessel
 * @brief   Header file for PUS 160 functions (System Management)
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup pus160 PUS Service 160
 * @brief PUS service 160 (System Management) implementation
 * @{
 */

#ifndef PUS160_H
#define PUS160_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @struct  softwareSelection_t
 * @brief   Structure to store the mandatory information for a software selection.
 * @see     ExecuteS160SS2
 */
typedef struct
{
    softwareId_t software_id;       /**< Software ID to reboot to */
    softwareState_t software_state; /**< Software state to reboot to */
} ATTR_PACKED softwareSelection_t;

/**
 * @struct  pus160Context_t
 * @brief   Struct type for pus160 context
 */
typedef struct
{
    uint32_t nb_tasks;           /**< Number of task in the system */
    deviceNo_t dev_reboot;       /**< Device for rebooting the system */
    deviceNo_t dev_context;      /**< Device for reading system context */
    deviceNo_t dev_system_usage; /**< Device for reading system usage */
    deviceNo_t dev_task_usages;  /**< Device for reading task usages */
} pus160Context_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t InitS160(pus160Context_t *pus160_context);
extern returnCode_t ExecuteS160SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS160SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS160SS17(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS160SS19(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS160SS21(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS160SS23(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS160SS33(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS160SS35(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS160SS37(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS160_H */

/**
 * @}
 * @}
 * @}
 */
