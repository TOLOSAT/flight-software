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
static void TIMER0_Callback(void);

/*************************** Variables Definitions ***************************/

extern uint32_t SystemCoreClock;

volatile uint32_t tick = 0u;
static TIM_HandleTypeDef hal_timer_inst = {0};

/*************************** Functions Definitions ***************************/

/**
 * @brief   Init HAL
 */
HAL_StatusTypeDef cmsdk_InitHal(void)
{
    // Setup the timer information
    hal_timer_inst.instance = CMSDK_TIMER0;
    hal_timer_inst.reload = SystemCoreClock / HAL_TIMER_FREQ;
    hal_timer_inst.mode = TIMER_PERIODIC;
    hal_timer_inst.callback = &TIMER0_Callback;
    
    // Init the timer
    cmsdk_TimerInit(&hal_timer_inst);

    // Enable the interrupt
    NVIC_EnableIRQ(TIMER0_IRQn);

    // Start the timer
    cmsdk_TimerStart(&hal_timer_inst);

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

void TIMER0_Handler(void)
{
    cmsdk_TimerIrqHandler(&hal_timer_inst);
}

static void TIMER0_Callback(void)
{
    tick++;
}