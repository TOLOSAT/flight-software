/**
 * @file    aocs.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for AOCS Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef AOCS_H
#define AOCS_H

/******************************* Include Files *******************************/

#include "application_types.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_AOCS_TEXT_SECTION    __attribute__((section(".text_aocs")))  /**< AOCS functions goes in the .text_aocs */
#define IN_AOCS_DATA_SECTION    __attribute__((section(".data_aocs")))  /**< AOCS data goes in the .data_aocs */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void AocsMain(void *task_desc);

#endif /* AOCS_H */