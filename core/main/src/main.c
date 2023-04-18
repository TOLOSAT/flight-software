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

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

typedef enum state
{
   STATE_SLOW = 0,
   STATE_NORMAL,
   STATE_FAST,
   NB_STATE,
} state_t;

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

volatile state_t state;

/************************* Functions Definitions *****************************/

uint32_t main(void){
  //Initialisation des variables
  state = STATE_NORMAL;

  //Initialisation
  init();
  
  /*Set LEDs default state*/
  HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);

  while(1){
    printf("Etat : %d\n", state);

    /*Toggle LEDs*/
    HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);

    switch(state){
      case STATE_SLOW :
        HAL_Delay(1000);
        break;
      case STATE_NORMAL :
        HAL_Delay(500);
        break;
      case STATE_FAST :
        HAL_Delay(100);
        break;
    }
  }
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
      if(state < STATE_FAST){
        state++;
      }
      else{
        state = STATE_SLOW;
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