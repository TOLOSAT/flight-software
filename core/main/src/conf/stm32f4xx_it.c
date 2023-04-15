/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/***************************** Include Files *********************************/
#include "stm32f4xx_it.h"
#include "stm32f407g-discovery_bsp.h"

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
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(B1_Pin);
}