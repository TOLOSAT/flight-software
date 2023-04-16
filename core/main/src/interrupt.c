/**
 * @file interrupts.c
 * @author Merlin Kooshmanian
 * @brief Source file containing all the interrupts
 * @date 16/04/2023
 * 
 * Last Update : 16/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/
#include "interrupts.h"
#include "stm32f4xx_nucleo_bsp.h"

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void){
  while (1){

  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void){
  while (1){

  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void){
  while (1){

  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void){
  while (1){

  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void){
  while (1){

  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void){

}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void){

}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void){

}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/**
  * @brief This function handles EXTernal Interrupt 0 handler.
  */
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
}