/**
 * @file monitoring.c
 * @author Merlin Kooshmanian
 * @brief Monitoring functions
 * @date 31/05/2023
 *
 * Last Update : 31/05/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <cmsis_os2.h>
#include <FreeRTOS.h>

#include "monitoring.h"
#include "tolosat_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

static void TIM3_Init(void);

/************************** Variable Definitions *****************************/

/**
 * @var     htim3
 * @brief   Timer instance used for the high frequency tick
 */
TIM_HandleTypeDef htim3;

/**
 * @var     ulHighFrequencyTimerTicks
 * @brief   High Frequency tick used for runtime calculation
 */
volatile unsigned long ulHighFrequencyTimerTicks;

/************************* Functions Definitions *****************************/

/**
 * @fn      InitMonitoring(void)
 * @brief   Enables TAPAS monitoring
 */
void InitMonitoring(void)
{
    TIM3_Init();
}

/**
 * @fn      configureTimerForRunTimeStats(void)
 * @brief   Configures runtime statistics variables
 */
void configureTimerForRunTimeStats(void)
{
    ulHighFrequencyTimerTicks = 0;
    HAL_TIM_Base_Start_IT(&htim3);
}

/**
 * @fn      getRunTimeCounterValue(void)
 * @brief   Increment runtime counter
 */
unsigned long getRunTimeCounterValue(void)
{
    return ulHighFrequencyTimerTicks;
}

/**
 * @brief This function handles TIM3 global interrupt.
 */
void TIM3_IRQHandler(void)
{
    // Needed for freertos stats
    ulHighFrequencyTimerTicks++;
    HAL_TIM_IRQHandler(&htim3);
}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void TIM3_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 0;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 1000;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
}