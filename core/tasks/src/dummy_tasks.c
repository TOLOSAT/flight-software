/**
 * @file dummy_tasks.h
 * @author Merlin Kooshmanian
 * @brief Header file with dummy tasks
 * @date 26/04/2023
 * 
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include "tasks.h"
#include "cmsis_os2.h"

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
 * @brief Function implementing the blink01 thread.
 * @param argument: Not used
 * @retval None
 */
void StartBlink01(void *argument __attribute__((unused)))
{
    /* Infinite loop */
    while (1)
    {
        HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
        osDelay(500);
    }
    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}

/**
 * @brief Function implementing the blink02 thread.
 * @param argument: Not used
 * @retval None
 */
void StartBlink02(void *argument __attribute__((unused)))
{
    /* Infinite loop */
    while (1)
    {
        HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
        osDelay(600);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}