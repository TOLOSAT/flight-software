/**
 * @file    thermal.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for THERMAL Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef THERMAL_H
#define THERMAL_H

/******************************* Include Files *******************************/

#include "application_types.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_THERMAL_TEXT_SECTION    __attribute__((section(".text_thermal")))  /**< THERMAL functions goes in the .text_thermal */
#define IN_THERMAL_DATA_SECTION    __attribute__((section(".data_thermal")))  /**< THERMAL data goes in the .data_thermal */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void ThermalMain(void *task_desc);

#endif /* THERMAL_H */