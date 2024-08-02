/**
 * @file    art_pi_bsp.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for bsp init
 * @date    27/07/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "art_pi_bsp.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_BSP_TEXT_SECTION     __attribute__((section(".text_bsp"))) /**< BSP functions goes in the .text_bsp */
#define IN_BSP_DATA_SECTION     __attribute__((section(".data_bsp"))) /**< BSP data goes in the .data_bsp */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      BSPLateInit(void)
 * @brief   This function will initialise some BSP specifique peripherals
 * @retval  1 if an error occured
 * @retval  0 else
 */
uint32_t IN_BSP_TEXT_SECTION BSPLateInit(void)
{
    return 0u;
}
