/**
 * @file    timers.h
 * @author  Matteo Planchet
 * @author  Nell Truong
 * @brief   Header file defining timers
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup core Core
 * @{
 * @defgroup timers Timers
 * @brief Software timers handling interface.
 * @{
 */

#ifndef SW_TIMERS_H
#define SW_TIMERS_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "core/os.h"
#include "core/tasks.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief Timer Handle type */
typedef TimerHandle_t timerHandle_t;

/** @brief Static timer buffer type */
typedef StaticTimer_t timerBuffer_t;

/**
 * @struct  timerConf_t
 * @brief   Struct type definition of a timer
 */
typedef struct
{
    timerNo_t timer; /**< @brief Timer reference number as it is declared in TIMERS_ENUM */
    taskNo_t owner;  /**< @brief Task reference number of the owner */
} timerConf_t;

/**
 * @struct  timerDesc_t
 * @brief   Struct type of a timer dynamic parameters
 */
typedef struct
{
    timerHandle_t handle; /**< @brief Timer handle */
    timerBuffer_t buffer; /**< @brief Timer buffer */
    tick_t saved_counter; /**< @brief Saved timer counter on pause and used by resume */
    taskNo_t owner;       /**< @brief Task reference number of the owner */
} timerDesc_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_timers_conf
 * @brief   Configuration table where all timers' static parameters are stored
 */
extern const timerConf_t g_timers_conf[CONFIG_MAX_NB_TIMERS];

/**
 * @var     g_timers_desc_table
 * @brief   Configuration table where all timers' descriptors are stored
 */
extern timerDesc_t g_timers_desc_table[CONFIG_MAX_NB_TIMERS];

/*************************** Functions Declarations **************************/

extern void CreateTimers(void);
extern returnCode_t StartTimer(timerNo_t timer);
extern returnCode_t PauseTimer(timerNo_t timer);
extern returnCode_t ResumeTimer(timerNo_t timer);
extern returnCode_t SetTimer(timerNo_t timer, tick_t period, timerMode_t mode);

#endif /* SW_TIMERS_H */

/**
 * @}
 * @}
 * @}
 */
