/**
 * @file    cmsdk_hal.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for basic CMSDK HAL functions
 * @date    09/06/2024
 * 
 * Largely inspired by the Zephyr driver and STM32 HAL style.
 */

/******************************* Include Files *******************************/

#include "cmsdk_hal.h"

/***************************** Macros Definitions ****************************/

#define HAL_TIMER_FREQ 1000u /* 1kHz timer freq */

/*************************** Functions Declarations **************************/

extern void TIMER0_Handler(void);
static void DUALTIMER_Callback(DUALTIM_TimerSelTypeDef sel);

/*************************** Variables Definitions ***************************/

extern uint32_t SystemCoreClock;

volatile uint32_t tick = 0u;
static DUALTIM_HandleTypeDef hal_timer_inst = {0};

/*************************** Functions Definitions ***************************/

/**
 * @brief   Init HAL
 */
HAL_StatusTypeDef cmsdk_InitHal(void)
{
    // Setup the timer information
    hal_timer_inst.instance = CMSDK_DUALTIMER;
    hal_timer_inst.mode_1 = DUALTIMER_PERIODIC;
    hal_timer_inst.size_1 = DUALTIMER_32_BITS;
    hal_timer_inst.prescaler_1 = DUALTIMER_PRESCALER_1;
    hal_timer_inst.reload_1 = (SystemCoreClock / HAL_TIMER_FREQ) - 1u;
    hal_timer_inst.mode_2 = DUALTIMER_DISABLED;
    hal_timer_inst.callback = &DUALTIMER_Callback;
    
    // Init the timer
    cmsdk_DualTimerInit(&hal_timer_inst);

    // Enable the interrupt
    NVIC_EnableIRQ(DUALTIMER_IRQn);

    // Start the timer
    cmsdk_DualTimerStart(&hal_timer_inst, DUALTIMER_TIMER_1);

    return HAL_OK;
}

/**
 * @brief   Create an active delay
 */
void cmsdk_HalDelay(uint32_t delay)
{
    uint32_t start_time = cmsdk_HalGetTick();

    while (cmsdk_HalGetTick() < (start_time + delay))
    {
        // Do nothing
    }
}

/**
 * @brief   Get the Hal tick
 */
uint32_t cmsdk_HalGetTick(void)
{
    return tick;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @brief DUALTIMER Interrupt Handler
 */
void DUALTIMER_Handler(void)
{
    cmsdk_DualTimerIrqHandler(&hal_timer_inst);
}

/**
 * @brief DUALTIMER Interrupt Callback
 */
static void DUALTIMER_Callback(DUALTIM_TimerSelTypeDef sel)
{
    if (sel == DUALTIMER_TIMER_1)
    {
        tick++;
    }
}