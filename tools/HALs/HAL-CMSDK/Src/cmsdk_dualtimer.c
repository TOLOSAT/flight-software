/**
 * @file    cmsdk_dualtimer.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for DUALTIMER CMSDK functions
 * @date    16/06/2024
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
 * @brief Init DualTimer
 */
HAL_StatusTypeDef cmsdk_DualTimerInit(DUALTIM_HandleTypeDef *dualtim)
{
    // First Setup Timer 1
    if (dualtim->mode_1 != DUALTIMER_DISABLED)
    {
        if (dualtim->mode_1 != DUALTIMER_FREERUNNING)
        {
            // Setup Reload
            dualtim->instance->Timer1Load = dualtim->reload_1;

            // Setup Periodic or Oneshot mode
            if (dualtim->mode_1 == DUALTIMER_PERIODIC)
            {
                dualtim->instance->Timer1Control |= CMSDK_DUALTIMER1_CTRL_MODE_Msk;
            }
            else
            {
                dualtim->instance->Timer1Control |= CMSDK_DUALTIMER1_CTRL_ONESHOOT_Msk;
            }
        }
        else
        {
            // Disable Periodic
            dualtim->instance->Timer1Control &= ~CMSDK_DUALTIMER1_CTRL_ONESHOOT_Msk;

            // Disable Oneshot
            dualtim->instance->Timer1Control &= ~CMSDK_DUALTIMER1_CTRL_MODE_Msk;
        }

        // Setup Size
        if (dualtim->size_1 == DUALTIMER_32_BITS)
        {
            dualtim->instance->Timer1Control |= CMSDK_DUALTIMER2_CTRL_SIZE_Msk;
        }
        else
        {
            dualtim->instance->Timer1Control &= ~CMSDK_DUALTIMER2_CTRL_SIZE_Msk;
        }

        // Setup Prescaler
        dualtim->instance->Timer1Control &= ~CMSDK_DUALTIMER1_CTRL_PRESCALE_Msk;
        dualtim->instance->Timer1Control |= (dualtim->prescaler_1 << CMSDK_DUALTIMER1_CTRL_PRESCALE_Pos);
        
        // Then enable interrupt 
        dualtim->instance->Timer1Control |= CMSDK_DUALTIMER1_CTRL_INTEN_Msk;
    }

    // Then Setup Timer 2
    if (dualtim->mode_2 != DUALTIMER_DISABLED)
    {
        if (dualtim->mode_2 != DUALTIMER_FREERUNNING)
        {
            // Setup Reload
            dualtim->instance->Timer2Load = dualtim->reload_2;

            // Setup Periodic or Oneshot mode
            if (dualtim->mode_2 == DUALTIMER_PERIODIC)
            {
                dualtim->instance->Timer2Control |= CMSDK_DUALTIMER2_CTRL_MODE_Msk;
            }
            else
            {
                dualtim->instance->Timer2Control |= CMSDK_DUALTIMER2_CTRL_ONESHOOT_Msk;
            }
        }
        else
        {
            // Disable Periodic
            dualtim->instance->Timer2Control &= ~CMSDK_DUALTIMER2_CTRL_ONESHOOT_Msk;

            // Disable Oneshot
            dualtim->instance->Timer2Control &= ~CMSDK_DUALTIMER2_CTRL_MODE_Msk;
        }

        // Setup Size
        if (dualtim->size_2 == DUALTIMER_32_BITS)
        {
            dualtim->instance->Timer2Control |= CMSDK_DUALTIMER2_CTRL_SIZE_Msk;
        }
        else
        {
            dualtim->instance->Timer1Control &= ~CMSDK_DUALTIMER2_CTRL_SIZE_Msk;
        }

        // Setup Prescaler
        dualtim->instance->Timer2Control &= ~CMSDK_DUALTIMER2_CTRL_PRESCALE_Msk;
        dualtim->instance->Timer2Control |= (dualtim->prescaler_2 << CMSDK_DUALTIMER1_CTRL_PRESCALE_Pos);
        
        // Then enable interrupt 
        dualtim->instance->Timer2Control |= CMSDK_DUALTIMER2_CTRL_INTEN_Msk;
    }

    return HAL_OK;
}

/**
 * @brief Start DualTimer
 */
HAL_StatusTypeDef cmsdk_DualTimerStart(DUALTIM_HandleTypeDef *dualtim, DUALTIM_TimerSelTypeDef sel)
{
    // Enable first timer if selected
    if (((sel == DUALTIMER_TIMER_1) || (sel == DUALTIMER_TIMER_BOTH)) && (dualtim->mode_1 != DUALTIMER_DISABLED))
    {
        // Enable Timer
        dualtim->instance->Timer1Control |= CMSDK_DUALTIMER1_CTRL_EN_Msk;
    }
    
    // Enable second timer if selected
    if (((sel == DUALTIMER_TIMER_2) || (sel == DUALTIMER_TIMER_BOTH)) && (dualtim->mode_2 != DUALTIMER_DISABLED))
    {
        // Enable Timer
        dualtim->instance->Timer2Control |= CMSDK_DUALTIMER2_CTRL_EN_Msk;
    }

    return HAL_OK;
}

/**
 * @brief Stop DualTimer
 */
HAL_StatusTypeDef cmsdk_DualTimerStop(DUALTIM_HandleTypeDef *dualtim, DUALTIM_TimerSelTypeDef sel)
{
    // Disable first timer if selected
    if ((sel == DUALTIMER_TIMER_1) || (sel == DUALTIMER_TIMER_BOTH))
    {
        // Enable Timer
        dualtim->instance->Timer1Control &= ~CMSDK_DUALTIMER1_CTRL_EN_Msk;
    }
    
    // Disable second timer if selected
    if ((sel == DUALTIMER_TIMER_1) || (sel == DUALTIMER_TIMER_BOTH))
    {
        // Enable Timer
        dualtim->instance->Timer2Control &= ~CMSDK_DUALTIMER2_CTRL_EN_Msk;
    }

    return HAL_OK;
}

/**
 * @brief Dual Timer Interrupt Handler
 */
HAL_StatusTypeDef cmsdk_DualTimerIrqHandler(DUALTIM_HandleTypeDef *dualtim)
{
    // Check if the interrupt came from TIMER 1
    if (dualtim->instance->Timer1MIS == CMSDK_DUALTIMER1_MASKINTSTAT_Msk)
    {
        // Clear the interrupt
        dualtim->instance->Timer1IntClr = CMSDK_DUALTIMER1_INTCLR_Msk;

        // Execute callback if any
        if (dualtim->callback)
        {
            dualtim->callback(DUALTIMER_TIMER_1);
        }
    }

    // Check if the interrupt came from TIMER 2
    if (dualtim->instance->Timer2MIS == CMSDK_DUALTIMER2_MASKINTSTAT_Msk)
    {
        // Clear the interrupt
        dualtim->instance->Timer2IntClr = CMSDK_DUALTIMER2_INTCLR_Msk;

        // Execute callback if any
        if (dualtim->callback)
        {
            dualtim->callback(DUALTIMER_TIMER_2);
        }
    }

    return HAL_OK;
}