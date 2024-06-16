/**
 * @file    generic_hal_timer.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC TIMER functions
 * @date    04/06/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void TIMER0_Callback(void);

/*************************** Variables Definitions ***************************/

/**
 * @var     monitoring_timer
 * @brief   Monitoring timer instance used for FreeRTOS monitoring
 */
static timerInst_t IN_TIM_DATA_SECTION monitoring_timer;

/**
 * @var     monitoring_tick
 * @brief   Tick for freertos monitoring
 */
static volatile uint64_t IN_TIM_DATA_SECTION monitoring_tick;

/*************************** Functions Definitions ***************************/

/**
 * @fn          HalDelay(uint32_t delay)
 * @brief       This function does using active wait using the HAL main timer tick. 
 * @param[in]   delay Amount of time in microseconds the wait will lasts.
 * @note        Redefinition of HAL_Delay().
 * @warning     Do not use this function inside a thread, please prefer the OS API
 */
void IN_TIM_TEXT_SECTION HalDelay(uint32_t delay)
{
    cmsdk_HalDelay(delay);
}

/**
 * @fn      HalGetTick(void)
 * @brief   This function returns the HAL main timer tick. 
 * @note    Redefinition of HAL_GetTick().
 * @warning Do not use this function inside a thread, please prefer the OS API
 */
uint32_t IN_TIM_TEXT_SECTION HalGetTick(void)
{
    return cmsdk_HalGetTick();
}

/******************* Monitoring Timer Functions Definitions ******************/

/**
 * @brief Monitoring Timer Initialization Function
 */
halStatus_t IN_TIM_TEXT_SECTION InitMonitoringTimer(void)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Setup the timer information
    monitoring_timer.instance = CMSDK_TIMER0;
    monitoring_timer.reload = 1000;
    monitoring_timer.mode = TIMER_PERIODIC;
    monitoring_timer.callback = &TIMER0_Callback;
    
    // Init the timer
    cmsdk_TimerInit(&monitoring_timer);

    // Enable the interrupt
    NVIC_EnableIRQ(TIMER0_IRQn);

    return return_value;
}

/**
 * @brief This function start Monitoring Timer
 */
void IN_TIM_TEXT_SECTION StartMonitoringTimer(void)
{
    cmsdk_TimerStart(&monitoring_timer);
}

/**
 * @brief This function get the current value of the monitoring tick
 */
uint64_t IN_TIM_TEXT_SECTION GetMonitoringTick(void)
{
    return monitoring_tick;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @brief TIMER0 Interrupt Handler
 */
void TIMER0_Handler(void)
{
    cmsdk_TimerIrqHandler(&monitoring_timer);
}

/**
 * @brief TIMER0 Interrupt Callback
 */
static void TIMER0_Callback(void)
{
    monitoring_tick++;
}
