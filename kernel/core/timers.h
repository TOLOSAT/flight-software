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

/**
 * @def     TIMER_CONF(timer_no)
 * @brief   Get timer conf from g_timers_conf_table
 */
#define TIMER_CONF(timer_no) (g_timers_conf_table[(timer_no) - 1u])

/**
 * @def     TIMER_DESC(timer_no)
 * @brief   Get timer conf from g_timers_desc_table
 */
#define TIMER_DESC(timer_no) (g_timers_desc_table[(timer_no) - 1u])

/**
 * @def     IS_A_VALID_TIMER(timer_no)
 * @brief   Indicates if the timer_no is valid
 */
#define IS_A_VALID_TIMER(timer_no) \
    (((timer_no) != (timerNo_t)NO_TIMER) && ((timer_no) < (timerNo_t)CONFIG_MAX_NB_TIMERS) && (TIMER_DESC(timer_no).status == DESC_USED))

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
    timerNo_t timer;             /**< @brief Timer reference number as it is declared in TIMERS_ENUM */
    taskNo_t owner;              /**< @brief Task reference number of the owner */
    timerBuffer_t *p_tim_buffer; /**< @brief Pointer to the timer buffer */
} timerConf_t;

/**
 * @struct  timerDesc_t
 * @brief   Struct type of a timer dynamic parameters
 */
typedef struct
{
    descStatus_t status;  /**< @brief Indicates if the descriptor is free or used */
    timerHandle_t handle; /**< @brief Timer handle */
    tick_t saved_counter; /**< @brief Saved timer counter on pause and used by resume */
    taskNo_t owner;       /**< @brief Task reference number of the owner */
} timerDesc_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_timers_conf_table
 * @brief   Configuration table where all timers' static parameters are stored
 */
extern const timerConf_t g_timers_conf_table[CONFIG_MAX_NB_TIMERS];

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
