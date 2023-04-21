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
#include "stm32f4xx_nucleo_bsp.h"
#include "init.h"
#include "tasks.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

typedef enum state
{
    STATE_1 = 0,
    STATE_2,
    STATE_3,
    NB_STATE,
} state_t;

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

volatile state_t state;

/************************* Functions Definitions *****************************/

/**
 * @fn int main(void)
 * @brief Main of the code
 * @param void
 * @return 0
 */
int main(void)
{
    // Variables initialisation
    state = STATE_1;

    // Initialisation
    init();

    createTasks();

    osKernelStart();

    while (1)
    {
        // Scheduler runs
    }

    return 0;
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
        // If you press the button states increments
        if (state < STATE_3)
        {
            state++;
        }
        else
        {
            state = STATE_1;
        }

        // Fake delay because HAL delay does not work there
        for (uint32_t i; i < 1000000; i++);

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
    if (htim->Instance == TIM11)
    {
        HAL_IncTick();
    }
}