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

#include "application_types.h"
#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_MISO_TEXT_SECTION    __attribute__((section(".text_miso")))  /**< MISO functions goes in the .text_miso */
#define IN_MISO_DATA_SECTION    __attribute__((section(".data_miso")))  /**< MISO data goes in the .data_miso */

#define NB_PUS161_EXECUTION    3u  /**< Number of pus161 exution functions */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern pusExecutionTable_t g_miso_execution_table[NB_PUS161_EXECUTION];

/*************************** Functions Declarations **************************/

extern void MisoMain(void *task_desc);

#endif /* MISO_H */