/**
 * @file    boot_fdir.c
 * @author  Merlin Kooshmanian
 * @brief   Error Management functions for boot
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "boot_fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void Reset_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      ErrorHandler(void)
 * @brief   This function is executed in case of error occurrence.
 * @warning Real FDIR has to be done.
 *
 * Normally this function has to save some context (what happened before error).
 * But currently no real fdir has been done. For debugging purposes, Error handler
 * is just a while loop that hangs processor indefinitly.
 */
void ErrorHandler(void)
{
    // Disable IRQ
    __disable_irq();

    // Warn that there is an error
    HAL_GPIO_WritePin(LED_STATUS_PORT, LED_STATUS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_ERROR_PORT, LED_ERROR_PIN, GPIO_PIN_RESET);

    // Infinite Loop
    while (1)
    {
        // Do nothing
    }
}

/*************************** Interruption Handlers ***************************/

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
    while (1)
    {
    }
}
