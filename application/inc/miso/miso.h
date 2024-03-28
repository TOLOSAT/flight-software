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
#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_MISO_TEXT_SECTION    __attribute__((section(".text_miso")))  /**< MISO functions goes in the .text_miso */
#define IN_MISO_DATA_SECTION    __attribute__((section(".data_miso")))  /**< MISO data goes in the .data_miso */

#define NB_PUS161_EXECUTION    3u  /**< Number of pus161 exution functions */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/
extern pusExecutionTable_t g_pus161_execution_table[NB_PUS161_EXECUTION];

/*************************** Functions Declarations **************************/

uint32_t getIdleTime();
uint32_t getStackUsage();
<<<<<<< HEAD
=======

char* getState();

>>>>>>> b6da180 (ajout tc get system state)
void MisoMain(void *task_dyn_conf);

#endif /* MISO_H */