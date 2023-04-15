/**
 * @file main.c
 * @author Merlin Kooshmanian
 * @brief Blink sans IDE en bare-metal
 * @version 0.1
 * @date 26/12/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/***************************** Include Files *********************************/
#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "stm32f407g-discovery_bsp.h"
#include "init.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

typedef enum state
{
   STATE_DEFAULT = 0,
   STATE_BLINKY,
   STATE_CHASER,
   NB_STATE,
} state_t;

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

volatile state_t state;

/************************* Functions Definitions *****************************/

uint32_t main(void){
  //Initialisation des variables
  uint32_t fooVar = 0;

  //Initialisation
  init();
  
  /*Set LEDs default state*/
  HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_SET);

  while(1){
    /*Toggle LEDs*/
    switch(state){
      case STATE_DEFAULT :
        break;
      case STATE_BLINKY :
        HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
        HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
        break;
      case STATE_CHASER :
        switch(fooVar){
          case 0 :
            HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
            fooVar++;
            break;
          case 1 :
            HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
            fooVar++;
            break;
          case 2 :
            HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
            fooVar++;
            break;
          case 3 :
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
            fooVar = 0;
            break;
        }
        break;
    }

    printf("Etat : %d\n", state);

    HAL_Delay(300);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == B1_Pin) // If The INT Source Is EXTI Line9 (A9 Pin)
    {
      // A chaque pression du bouton on 
      if(state < STATE_CHASER){
        state++;
      }
      else{
        state = STATE_DEFAULT;
      }
      // On temporise avec une boucle for (HAL Delay non 
      // fonctionnel) pour eviter les rebondissements du bouton
      for(uint32_t i; i < 1000000; i++); 
    }
}