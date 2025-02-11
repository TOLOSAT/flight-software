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

volatile uint32_t tick = 0u;

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

/**
 * @brief   Increments the Hal tick
 */
void cmsdk_HalIncTick(void)
{
    tick++;
}