/**
 * @file main.c
 * @author Merlin Kooshmanian
 * @brief Main source file
 * @date 26/12/2022
 *
 * Last Update : 21/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/
#include <stdio.h>
#include <cmsis_os2.h>

#include "main.h"
#include "init.h"
#include "tasks.h"
#include "buffers.h"

#if defined(STM32F411xE)
#include "stm32f4xx_nucleo_bsp.h"
#endif
#if defined(STM32F103xB)
#include "stm32f1xx_nucleo_bsp.h"
#endif

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************* Functions Definitions *****************************/

/**
 * @fn      main(void)
 * @brief   Main of the code
 * @param   void
 * @return  0
 */
int main(void)
{
    // Initialisation
    init();
    createTasks();
    createBuffers();

    printf("[Main] Hello\n");

    // Function Core
    osKernelStart();
    while (1)
    {
        // Scheduler runs
    }

    return(0);
}

/**
 * @brief  EXTI line detection callback.
 * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == USER_BUTTON_PIN) // If The INT Source Is EXTI Line9 (A9 Pin)
    {
        printf("Hello from interrupt !\n");
    }
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM11 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        HAL_IncTick();
    }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
        // Do nothing
    }
}