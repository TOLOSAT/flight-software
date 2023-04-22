/**
 * @file tasks.c
 * @author Merlin Kooshmanian
 * @brief Source file defining tasks
 * @date 21/04/2023
 * 
 * Last Update : 21/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <cmsis_os2.h>

#include "tasks.h"

#if defined(STM32F411xE)
#include "stm32f4xx_nucleo_bsp.h"
#endif
#if defined(STM32F103xB)
#include "stm32f1xx_nucleo_bsp.h"
#endif

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

void StartBlink01(void *argument);
void StartBlink02(void *argument);

/************************** Variable Definitions *****************************/

osThreadId_t blink01Handle;
osThreadId_t blink02Handle;

const osThreadAttr_t blink01_attributes = {
    .name = "blink01",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 128};

const osThreadAttr_t blink02_attributes = {
    .name = "blink02",
    .priority = (osPriority_t)osPriorityBelowNormal,
    .stack_size = 128};

/************************* Functions Definitions *****************************/

/**
 * @fn uint32_t main(void)
 * @brief Function that creates threads and links them to tasks
 * @param void
 * @return 0
 */
uint32_t createTasks(void)
{
    blink01Handle = osThreadNew(StartBlink01, NULL, &blink01_attributes);
    blink02Handle = osThreadNew(StartBlink02, NULL, &blink02_attributes);

    return(0);
}

/**
 * @brief Function implementing the blink01 thread.
 * @param argument: Not used
 * @retval None
 */
void StartBlink01(void *argument)
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
void StartBlink02(void *argument)
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