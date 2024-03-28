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

/**
 * @brief Retrieves the idle time of the system.
 *
 * This function returns the amount of time that the system has been idle, measured in milliseconds.
 *
 * @return The idle time in milliseconds.
 */
uint32_t getIdleTime();

/**
 * @brief Retrieves the stack usage of the system.
 *
 * @return The max stack usage in bytes.
 */
uint32_t getStackUsage();

/**
 * @brief Retrieves the current state of the system.
 *
 * @return A pointer to a string representing the state.
 */
char* getState();

/**
 * @brief The main function for the Miso task.
 *
 * @param task_dyn_conf A pointer to the dynamic configuration of the task.
 */
void MisoMain(void *task_dyn_conf);

#endif /* MISO_H */