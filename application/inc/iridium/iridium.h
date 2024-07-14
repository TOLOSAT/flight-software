/**
 * @file    iridium.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for IRIDIUM Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef IRIDIUM_H
#define IRIDIUM_H

/******************************* Include Files *******************************/

#include "application_types.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_IRIDIUM_TEXT_SECTION    __attribute__((section(".text_iridium")))  /**< IRIDIUM functions goes in the .text_iridium */
#define IN_IRIDIUM_DATA_SECTION    __attribute__((section(".data_iridium")))  /**< IRIDIUM data goes in the .data_iridium */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void IridiumMain(void *task_dyn_conf);

#endif /* IRIDIUM_H */