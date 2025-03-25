/**
 * @file    timer.h
 * @author  Matteo Planchet, Nell Truong
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
 * @brief Timers handling interface.
 * @{
 */

#ifndef TIMER_H
#define TIMER_H

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
} timerDesc_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_timers_conf
 * @brief   Configuration table where all timers' static parameters are stored
 */
extern const timerConf_t g_timers_conf[NB_TIMERS];

/**
 * @var     g_timers_desc_table
 * @brief   Configuration table where all timers' descriptors are stored
 */
extern timerDesc_t g_timers_desc_table[NB_TIMERS];

/*************************** Functions Declarations **************************/

extern void CreateTimers(void);
extern void StartTimer(timerNo_t timer);
extern void PauseTimer(timerNo_t timer);
extern void ResumeTimer(timerNo_t timer);
extern void SetTimer(timerNo_t timer, tick_t period, timerMode_t mode);

#endif /* TIMER_H */

/**
 * @}
 * @}
 * @}
 */
