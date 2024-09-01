/**
 * @file    nucleo_f411re_bsp.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for bsp init
 * @date    27/07/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "nucleo_f411re_bsp.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_BSP_TEXT_SECTION     __attribute__((section(".text_bsp")))   /**< BSP functions goes in the .text_bsp */
#define IN_BSP_DATA_SECTION     __attribute__((section(".data_bsp")))   /**< BSP data goes in the .data_bsp */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      SystemClock_Config
 * @brief   System Clock Configuration
 */
bspStatus_t IN_BSP_TEXT_SECTION SystemClock_Config(void)
{
    // Variable Initialisation
    bspStatus_t return_value = BSP_SUCCESSFUL;
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Function Core
    /** Initializes the RCC Oscillator Powers */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK)
    {
        /** Initializes the CPU, AHB and APB buses clocks */
        RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | 
                                      RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
        if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
        { 
            return_value = BSP_ERROR;
        }
    }
    else
    {
        return_value = BSP_ERROR;
    }

    return return_value;
}

/**
 * @fn      BSPLateInit(void)
 * @brief   This function will initialise some BSP specifique peripherals
 * @retval  #BSP_ERROR if an error occured
 * @retval  #BSP_SUCCESSFUL else
 */
bspStatus_t IN_BSP_TEXT_SECTION BSPLateInit(void)
{
    return BSP_SUCCESSFUL;
}
