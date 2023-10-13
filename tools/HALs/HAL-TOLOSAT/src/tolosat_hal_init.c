/**
 * @file    tolosat_hal_init.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT HAL Initialisation functions
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static halStatus_t SystemClock_Config(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitHal(void)
 * @brief   Function that init the choosen HAL dans sysclock
 * @retval  #THAL_ERROR if cannot init HAL or system clock
 * @retval  #THAL_SUCCESSFUL else
 *
 * If there is an error it goes to HardFault Handler
 */
halStatus_t InitHal(void)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;
    HAL_StatusTypeDef test_val;

    // Function Core
    test_val = HAL_Init();
    if(test_val == HAL_OK)
    {
        return_value = SystemClock_Config();
    }
    else
    {
        return_value = THAL_ERROR;
    }

    return return_value;
}

/**
 * @fn      SystemClock_Config
 * @brief   System Clock Configuration
 */
static halStatus_t SystemClock_Config(void)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Function Core
    /** Initializes the RCC Oscillator Powers */
    TAPAS_CLOCK_SPECIFIC_INIT_PWR();

    /** Initializes the RCC Oscillators */
    TAPAS_CLOCK_SPECIFIC_INIT_OSC(RCC_OscInitStruct);

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK)
    {
        /** Initializes the CPU, AHB and APB buses clocks */
        TAPAS_CLOCK_SPECIFIC_INIT_BUS(RCC_ClkInitStruct);
    }
    else
    {
        return_value = THAL_ERROR;
    }

    return return_value;
}