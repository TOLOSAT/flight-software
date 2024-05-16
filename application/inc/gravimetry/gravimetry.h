/**
 * @file    gravimetry.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for GRAVIMETRY Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef GRAVIMETRY_H
#define GRAVIMETRY_H

/******************************* Include Files *******************************/

#include <stdint.h>

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_GRAVIMETRY_TEXT_SECTION    __attribute__((section(".text_gravimetry")))  /**< GRAVIMETRY functions goes in the .text_gravimetry */
#define IN_GRAVIMETRY_DATA_SECTION    __attribute__((section(".data_gravimetry")))  /**< GRAVIMETRY data goes in the .data_gravimetry */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void GravimetryMain(void *task_dyn_conf);

#endif /* GRAVIMETRY_H */