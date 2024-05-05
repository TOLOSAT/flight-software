/**
 * @file    generic_hal_init.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for GENERIC HAL Initialisation functions
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef GENERIC_HAL_INIT_H
#define GENERIC_HAL_INIT_H

/******************************* Include Files *******************************/

#include "generic_hal_types.h"

/***************************** Macros Definitions ****************************/

#define IN_INIT_TEXT_SECTION    __attribute__((section(".text_init")))  /**< INIT functions goes in the .text_init */
#define IN_INIT_DATA_SECTION    __attribute__((section(".data_init")))  /**< INIT data goes in the .data_init */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

halStatus_t InitHal(void);

#endif /* GENERIC_HAL_INIT_H */