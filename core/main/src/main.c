/**
 * @file main.c
 * @author Merlin Kooshmanian
 * @brief Main source file
 * @date 26/12/2022
 * 
 * Last Update : 16/04/2023
 * @copyright Copyright (c) TOLOSAT 2022
 */

/***************************** Include Files *********************************/
#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "stm32f4xx_nucleo_bsp.h"
#include "init.h"
#include "cmsis_os2.h"

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

void StartBlink01(void *argument);
void StartBlink02(void *argument);

/************************** Variable Definitions *****************************/

volatile state_t state;
osThreadId_t blink01Handle;
osThreadId_t blink02Handle;

/************************* Functions Definitions *****************************/

uint32_t main(void){
  //Initialisation des variables
  state = STATE_1;

  //Initialisation
  init();

  const osThreadAttr_t blink01_attributes = {
    .name = "blink01",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  blink01Handle = osThreadNew(StartBlink01, NULL, &blink01_attributes);

  /* definition and creation of blink02 */
  const osThreadAttr_t blink02_attributes = {
    .name = "blink02",
    .priority = (osPriority_t) osPriorityBelowNormal,
    .stack_size = 128
  };
  blink02Handle = osThreadNew(StartBlink02, NULL, &blink02_attributes);

  osKernelStart();

  while(1)
  {

  }
}

/**
  * @brief  Function implementing the blink01 thread.
  * @param  argument: Not used 
  * @retval None
  */
void StartBlink01(void *argument)
{
  /* Infinite loop */
  while(1)
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
  while(1)
  {
    HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
    osDelay(600);
  }

  // In case we accidentally exit from task loop
  osThreadTerminate(NULL);
}


/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == USER_BUTTON_PIN) // If The INT Source Is EXTI Line9 (A9 Pin)
    {
      // A chaque pression du bouton on 
      if(state < STATE_3){
        state++;
      }
      else{
        state = STATE_1;
      }
      // On temporise avec une boucle for (HAL Delay non 
      // fonctionnel) pour eviter les rebondissements du bouton
      for(uint32_t i; i < 1000000; i++); 
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
  if (htim->Instance == TIM11) {
    HAL_IncTick();
  }
}