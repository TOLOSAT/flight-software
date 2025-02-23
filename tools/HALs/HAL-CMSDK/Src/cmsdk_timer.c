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
 * @fn          cmsdk_TimerInit(TIM_HandleTypeDef *tim)
 * @brief       Init Timer
 * @param[in]   tim Timer handle struct
 * @retval      #HAL_ERROR if tim is a null pointer or reload is zero
 * @retval      #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_TimerInit(TIM_HandleTypeDef *tim)
{
    HAL_StatusTypeDef status = HAL_OK;

    if ((tim != NULL) && (tim->reload != 0u))
    {
        // Setup reload value
        tim->instance->RELOAD = tim->reload;

        // Enable interrupt bit
        tim->instance->CTRL |= CMSDK_TIMER_CTRL_IRQEN_Msk;
    }
    else
    {
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @fn          cmsdk_TimerStart(TIM_HandleTypeDef *tim)
 * @brief       Start Timer
 * @param[in]   tim Timer handle struct
 * @retval      #HAL_ERROR if tim is a null pointer
 * @retval      #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_TimerStart(TIM_HandleTypeDef *tim)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (tim != NULL)
    {
        // Setup the timer to the reload value
        tim->instance->VALUE = tim->instance->RELOAD;

        // Enable Timer
        tim->instance->CTRL |= CMSDK_TIMER_CTRL_EN_Msk;
    }
    else
    {
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @fn          cmsdk_TimerStop(TIM_HandleTypeDef *tim)
 * @brief       Stop Timer
 * @param[in]   tim Timer handle struct
 * @retval      #HAL_ERROR if tim is a null pointer
 * @retval      #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_TimerStop(TIM_HandleTypeDef *tim)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (tim != NULL)
    {
        // Disable Timer
        tim->instance->CTRL &= ~CMSDK_TIMER_CTRL_EN_Msk;
    }
    else
    {
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief Timer Interrupt Handler
 */
void cmsdk_TimerIrqHandler(TIM_HandleTypeDef *tim)
{
    if (tim != NULL)
    {
        // Clear the interrupt
        tim->instance->INTCLEAR = CMSDK_TIMER_INTCLEAR_Msk;

        // Disable timer if oneshot mode
        if (tim->mode == TIMER_ONESHOT)
        {
            tim->instance->CTRL &= ~CMSDK_TIMER_CTRL_EN_Msk;
        }

        // Execute callback if any
        if (tim->callback != NULL)
        {
            tim->callback();
        }
    }
}