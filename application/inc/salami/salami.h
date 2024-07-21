/**
 * @file    salami.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SALAMI Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef SALAMI_H
#define SALAMI_H

/******************************* Include Files *******************************/

#include "application_types.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_SALAMI_TEXT_SECTION    __attribute__((section(".text_salami")))  /**< SALAMI functions goes in the .text_salami */
#define IN_SALAMI_DATA_SECTION    __attribute__((section(".data_salami")))  /**< SALAMI data goes in the .data_salami */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void SalamiMain(void *task_dyn_conf);

#endif /* SALAMI_H */