/**
 * @file    cmsdk_timer.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TIMER CMSDK functions
 * @date    09/06/2024
 * 
 * Largely inspired by the Zephyr driver and STM32 HAL style.
 */

/******************************* Include Files *******************************/

#include "cmsdk_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @brief Init Timer
 */
HAL_StatusTypeDef cmsdk_TimerInit(TIM_HandleTypeDef *tim)
{
    // Check reload value is correct
    if (tim->reload != 0u)
    {
        // Setup reload value
        tim->instance->RELOAD = tim->reload;

        // Enable interrupt bit
        tim->instance->CTRL |= CMSDK_TIMER_CTRL_IRQEN_Msk;

        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}

/**
 * @brief Start Timer
 */
HAL_StatusTypeDef cmsdk_TimerStart(TIM_HandleTypeDef *tim)
{
    // Setup the timer to the reload value
    tim->instance->VALUE = tim->instance->RELOAD;

    // Enable Timer
    tim->instance->CTRL |= CMSDK_TIMER_CTRL_EN_Msk;

    return HAL_OK;
}

/**
 * @brief Stop Timer
 */
HAL_StatusTypeDef cmsdk_TimerStop(TIM_HandleTypeDef *tim)
{
    // Disable Timer
    tim->instance->CTRL &= ~CMSDK_TIMER_CTRL_EN_Msk;

    return HAL_OK;
}

/**
 * @brief Timer Interrupt Handler
 */
HAL_StatusTypeDef cmsdk_TimerIrqHandler(TIM_HandleTypeDef *tim)
{   
    // Clear the interrupt
    tim->instance->INTCLEAR = CMSDK_TIMER_INTCLEAR_Msk;

    // Disable timer if oneshot mode
    if (tim->mode == TIMER_ONESHOT)
    {
        tim->instance->CTRL &= ~CMSDK_TIMER_CTRL_EN_Msk;
    }

    // Execute callback if any
    if (tim->callback)
    {
        tim->callback();
    }

    return HAL_OK;
}