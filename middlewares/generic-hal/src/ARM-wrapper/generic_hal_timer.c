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

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          HalDelay(uint32_t delay)
 * @brief       This function does using active wait using the HAL main timer tick. 
 * @param[in]   delay Amount of time in microseconds the wait will lasts.
 * @note        Redefinition of HAL_Delay().
 * @warning     Do not use this function inside a thread, please prefer the OS API
 */
void HalDelay(uint32_t delay)
{
    cmsdk_HalDelay(delay);
}

/**
 * @fn      HalGetTick(void)
 * @brief   This function returns the HAL main timer tick. 
 * @note    Redefinition of HAL_GetTick().
 * @warning Do not use this function inside a thread, please prefer the OS API
 */
uint32_t HalGetTick(void)
{
    return cmsdk_HalGetTick();
}
