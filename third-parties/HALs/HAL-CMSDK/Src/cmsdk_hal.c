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

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     tick
 * @brief   HAL reference clock tick used by polling drivers
 */
static volatile uint32_t tick = 0u;

/*************************** Functions Definitions ***************************/

/**
 * @brief   Init HAL
 */
HAL_StatusTypeDef cmsdk_InitHal(void)
{
    return HAL_InitTick();
}

/**
 * @brief  This function configures the HAL Timer
 */
HAL_StatusTypeDef __attribute__((weak)) HAL_InitTick(void)
{
    return HAL_ERROR;
}

/**
 * @brief   Create an active delay
 */
void HAL_Delay(uint32_t delay)
{
    uint32_t start_time = HAL_GetTick();

    while (HAL_GetTick() < (start_time + delay))
    {
        // Do nothing
    }
}

/**
 * @brief   Get the Hal tick
 */
uint32_t HAL_GetTick(void)
{
    return tick;
}

/**
 * @brief   Increments the Hal tick
 */
void HAL_IncTick(void)
{
    tick++;
}