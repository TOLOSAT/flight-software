/**
 * @file    miso.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for MISO Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef MISO_H
#define MISO_H

/******************************* Include Files *******************************/

#include <stdint.h>

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_MISO_TEXT_SECTION    __attribute__((section(".text_miso")))  /**< MISO functions goes in the .text_miso */
#define IN_MISO_DATA_SECTION    __attribute__((section(".data_miso")))  /**< MISO data goes in the .data_miso */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void MisoMain(void *task_dyn_conf);

#endif /* MISO_H */