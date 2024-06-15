/**
 * @file    generic_hal_init.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC HAL Initialisation functions
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static halStatus_t SystemClock_Config(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitHal(void)
 * @brief   Function that init the choosen HAL dans sysclock
 * @retval  #GEN_HAL_ERROR if cannot init HAL or system clock
 * @retval  #GEN_HAL_SUCCESSFUL else
 *
 * If there is an error it goes to Error Handler
 */
halStatus_t IN_INIT_TEXT_SECTION InitHal(void)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;
    HAL_StatusTypeDef test_val;

    // Function Core
    test_val = HAL_Init();
    if (test_val == HAL_OK)
    {
        return_value = SystemClock_Config();
    }
    else
    {
        return_value = GEN_HAL_ERROR;
    }

    return return_value;
}

/**
 * @fn      SystemClock_Config
 * @brief   System Clock Configuration
 */
static halStatus_t IN_INIT_TEXT_SECTION SystemClock_Config(void)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Function Core
    /** Initializes the RCC Oscillator Powers */
    CLOCK_SPECIFIC_INIT_PWR();

    /** Initializes the RCC Oscillators */
    CLOCK_SPECIFIC_INIT_OSC(RCC_OscInitStruct);

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK)
    {
        /** Initializes the CPU, AHB and APB buses clocks */
        CLOCK_SPECIFIC_INIT_BUS(RCC_ClkInitStruct);
    }
    else
    {
        return_value = GEN_HAL_ERROR;
    }

    return return_value;
}