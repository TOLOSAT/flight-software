/**
 * @file    carne.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for CARNE Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef CARNE_H
#define CARNE_H

/******************************* Include Files *******************************/

#include <stdint.h>

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_CARNE_TEXT_SECTION    __attribute__((section(".text_carne")))  /**< CARNE functions goes in the .text_carne */
#define IN_CARNE_DATA_SECTION    __attribute__((section(".data_carne")))  /**< CARNE data goes in the .data_carne */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void CarneMain(void *task_dyn_conf);

#endif /* CARNE_H */