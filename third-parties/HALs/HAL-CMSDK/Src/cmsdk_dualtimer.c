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

/**
 * @def     IS_DUALTIM_SEL
 * @brief   Check if the dual timer selection corresponds to a timer
 */
#define IS_DUALTIM_SEL(SEL) (((SEL) == HAL_DUALTIMER_TIMER_1) || ((SEL) == HAL_DUALTIMER_TIMER_2) || ((SEL) == HAL_DUALTIMER_TIMER_BOTH))

/**
 * @def     IS_DUALTIM_MODE
 * @brief   Check if the dual timer mode corresponds to a timer
 */
#define IS_DUALTIM_MODE(MODE) \
    (((MODE) == HAL_DUALTIMER_DISABLED) || ((MODE) == HAL_DUALTIMER_ONESHOT) || ((MODE) == HAL_DUALTIMER_PERIODIC) || ((MODE) == HAL_DUALTIMER_FREERUNNING))

/**
 * @def     IS_DUALTIM_PRESCALER
 * @brief   Check if the dual timer prescaler corresponds to a timer
 */
#define IS_DUALTIM_PRESCALER(PRESCAL) \
    (((PRESCAL) == HAL_DUALTIMER_PRESCALER_1) || ((PRESCAL) == HAL_DUALTIMER_PRESCALER_16) || ((PRESCAL) == HAL_DUALTIMER_PRESCALER_256))

/**
 * @def     IS_DUALTIM_SIZE
 * @brief   Check if the dual timer size corresponds to a timer
 */
#define IS_DUALTIM_SIZE(SIZE) (((SIZE) == HAL_DUALTIMER_32_BITS) || ((SIZE) == HAL_DUALTIMER_16_BITS))

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          cmsdk_DualTimerInit(DUALTIM_HandleTypeDef *dualtim)
 * @brief       Init DualTimer
 * @param[in]   dualtim Dual timer handle struct
 * @retval      #HAL_ERROR if dualtim is a null pointer
 * @retval      #HAL_ERROR if dualtim options (mode, timer, and prescaler) are incorrects
 * @retval      #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_DualTimerInit(DUALTIM_HandleTypeDef *dualtim)
{
    HAL_StatusTypeDef status = HAL_OK;

    if ((dualtim != NULL) && (IS_DUALTIM_MODE(dualtim->mode_1)) && (IS_DUALTIM_MODE(dualtim->mode_2)) && (IS_DUALTIM_PRESCALER(dualtim->prescaler_1))
        && (IS_DUALTIM_PRESCALER(dualtim->prescaler_2)) && (IS_DUALTIM_SIZE(dualtim->size_1)) && (IS_DUALTIM_SIZE(dualtim->size_2)))
    {
        // First Setup Timer 1
        if (dualtim->mode_1 != HAL_DUALTIMER_DISABLED)
        {
            if (dualtim->mode_1 != HAL_DUALTIMER_FREERUNNING)
            {
                // Setup Reload
                dualtim->instance->T1LOAD = dualtim->reload_1;

                // Setup Periodic or Oneshot mode
                if (dualtim->mode_1 == HAL_DUALTIMER_PERIODIC)
                {
                    dualtim->instance->T1CTRL |= CMSDK_DUALTIMER_CTRL_MODE_Msk;
                }
                else
                {
                    dualtim->instance->T1CTRL |= CMSDK_DUALTIMER_CTRL_ONESHOOT_Msk;
                }
            }
            else
            {
                // Disable Periodic
                dualtim->instance->T1CTRL &= ~CMSDK_DUALTIMER_CTRL_ONESHOOT_Msk;

                // Disable Oneshot
                dualtim->instance->T1CTRL &= ~CMSDK_DUALTIMER_CTRL_MODE_Msk;
            }

            // Setup Size
            if (dualtim->size_1 == HAL_DUALTIMER_32_BITS)
            {
                dualtim->instance->T1CTRL |= CMSDK_DUALTIMER_CTRL_SIZE_Msk;
            }
            else
            {
                dualtim->instance->T1CTRL &= ~CMSDK_DUALTIMER_CTRL_SIZE_Msk;
            }

            // Setup Prescaler
            dualtim->instance->T1CTRL &= ~CMSDK_DUALTIMER_CTRL_PRESCALE_Msk;
            dualtim->instance->T1CTRL |= (dualtim->prescaler_1 << CMSDK_DUALTIMER_CTRL_PRESCALE_Pos);

            // Then enable interrupt
            dualtim->instance->T1CTRL |= CMSDK_DUALTIMER_CTRL_INTEN_Msk;
        }

        // Then Setup Timer 2
        if (dualtim->mode_2 != HAL_DUALTIMER_DISABLED)
        {
            if (dualtim->mode_2 != HAL_DUALTIMER_FREERUNNING)
            {
                // Setup Reload
                dualtim->instance->T2LOAD = dualtim->reload_2;

                // Setup Periodic or Oneshot mode
                if (dualtim->mode_2 == HAL_DUALTIMER_PERIODIC)
                {
                    dualtim->instance->T2CTRL |= CMSDK_DUALTIMER_CTRL_MODE_Msk;
                }
                else
                {
                    dualtim->instance->T2CTRL |= CMSDK_DUALTIMER_CTRL_ONESHOOT_Msk;
                }
            }
            else
            {
                // Disable Periodic
                dualtim->instance->T2CTRL &= ~CMSDK_DUALTIMER_CTRL_ONESHOOT_Msk;

                // Disable Oneshot
                dualtim->instance->T2CTRL &= ~CMSDK_DUALTIMER_CTRL_MODE_Msk;
            }

            // Setup Size
            if (dualtim->size_2 == HAL_DUALTIMER_32_BITS)
            {
                dualtim->instance->T2CTRL |= CMSDK_DUALTIMER_CTRL_SIZE_Msk;
            }
            else
            {
                dualtim->instance->T2CTRL &= ~CMSDK_DUALTIMER_CTRL_SIZE_Msk;
            }

            // Setup Prescaler
            dualtim->instance->T2CTRL &= ~CMSDK_DUALTIMER_CTRL_PRESCALE_Msk;
            dualtim->instance->T2CTRL |= (dualtim->prescaler_2 << CMSDK_DUALTIMER_CTRL_PRESCALE_Pos);

            // Then enable interrupt
            dualtim->instance->T2CTRL |= CMSDK_DUALTIMER_CTRL_INTEN_Msk;
        }
    }
    else
    {
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @fn          cmsdk_DualTimerStart(DUALTIM_HandleTypeDef *dualtim, DUALTIM_TimerSelTypeDef sel)
 * @brief       Start DualTimer
 * @param[in]   dualtim Dual timer handle struct
 * @param[in]   sel Timer selection
 * @retval      #HAL_ERROR if dualtim is a null pointer
 * @retval      #HAL_ERROR selection correspond to any timer
 * @retval      #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_DualTimerStart(DUALTIM_HandleTypeDef *dualtim, DUALTIM_TimerSelTypeDef sel)
{
    HAL_StatusTypeDef status = HAL_OK;

    if ((dualtim != NULL) && (IS_DUALTIM_SEL(sel)))
    {
        // Enable first timer if selected
        if (((sel == HAL_DUALTIMER_TIMER_1) || (sel == HAL_DUALTIMER_TIMER_BOTH)) && (dualtim->mode_1 != HAL_DUALTIMER_DISABLED))
        {
            // Enable Timer
            dualtim->instance->T1CTRL |= CMSDK_DUALTIMER_CTRL_EN_Msk;
        }

        // Enable second timer if selected
        if (((sel == HAL_DUALTIMER_TIMER_2) || (sel == HAL_DUALTIMER_TIMER_BOTH)) && (dualtim->mode_2 != HAL_DUALTIMER_DISABLED))
        {
            // Enable Timer
            dualtim->instance->T2CTRL |= CMSDK_DUALTIMER_CTRL_EN_Msk;
        }
    }
    else
    {
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @fn          cmsdk_DualTimerStop(DUALTIM_HandleTypeDef *dualtim, DUALTIM_TimerSelTypeDef sel)
 * @brief       Stop DualTimer
 * @param[in]   dualtim Dual timer handle struct
 * @param[in]   sel Timer selection
 * @retval      #HAL_ERROR if dualtim is a null pointer
 * @retval      #HAL_ERROR selection correspond to any timer
 * @retval      #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_DualTimerStop(DUALTIM_HandleTypeDef *dualtim, DUALTIM_TimerSelTypeDef sel)
{
    HAL_StatusTypeDef status = HAL_OK;

    if ((dualtim != NULL) && (IS_DUALTIM_SEL(sel)))
    {
        // Disable first timer if selected
        if ((sel == HAL_DUALTIMER_TIMER_1) || (sel == HAL_DUALTIMER_TIMER_BOTH))
        {
            // Enable Timer
            dualtim->instance->T1CTRL &= ~CMSDK_DUALTIMER_CTRL_EN_Msk;
        }

        // Disable second timer if selected
        if ((sel == HAL_DUALTIMER_TIMER_2) || (sel == HAL_DUALTIMER_TIMER_BOTH))
        {
            // Enable Timer
            dualtim->instance->T2CTRL &= ~CMSDK_DUALTIMER_CTRL_EN_Msk;
        }
    }
    else
    {
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief Dual Timer Interrupt Handler
 */
void cmsdk_DualTimerIrqHandler(DUALTIM_HandleTypeDef *dualtim)
{
    if (dualtim != NULL)
    {
        // Check if the interrupt came from TIMER 1
        if (dualtim->instance->T1MIS == CMSDK_DUALTIMER_MIS_Msk)
        {
            // Clear the interrupt
            dualtim->instance->T1INTCLR = CMSDK_DUALTIMER_INTCLR_Msk;

            // Execute callback if any
            if (dualtim->callback != NULL)
            {
                dualtim->callback(HAL_DUALTIMER_TIMER_1);
            }
        }

        // Check if the interrupt came from TIMER 2
        if (dualtim->instance->T2MIS == CMSDK_DUALTIMER_MIS_Msk)
        {
            // Clear the interrupt
            dualtim->instance->T2INTCLR = CMSDK_DUALTIMER_INTCLR_Msk;

            // Execute callback if any
            if (dualtim->callback != NULL)
            {
                dualtim->callback(HAL_DUALTIMER_TIMER_2);
            }
        }
    }
}