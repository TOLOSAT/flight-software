/**
 * @file    power.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for POWER Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef POWER_H
#define POWER_H

/******************************* Include Files *******************************/

#include "application_types.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_POWER_TEXT_SECTION    __attribute__((section(".text_power")))  /**< POWER functions goes in the .text_power */
#define IN_POWER_DATA_SECTION    __attribute__((section(".data_power")))  /**< POWER data goes in the .data_power */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void PowerMain(void);

#endif /* POWER_H */