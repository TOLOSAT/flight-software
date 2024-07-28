/**
 * @file    tc_process.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TC_PROCESS Task
 * @date    02/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TC_PROCESS_H
#define TC_PROCESS_H

/******************************* Include Files *******************************/

#include "application_types.h"
#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

#define NB_NORMAL_EXECUTION    4u  /**< Number of exution functions */

// Section placement macros
#define IN_TMTC_TEXT_SECTION    __attribute__((section(".text_tmtc")))  /**< TMTC functions goes in the .text_tmtc */
#define IN_TMTC_DATA_SECTION    __attribute__((section(".data_tmtc")))  /**< TMTC data goes in the .data_tmtc */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern pusExecutionTable_t g_normal_execution_table[NB_NORMAL_EXECUTION];

/*************************** Functions Declarations **************************/

extern void TcProcessMain(void *task_desc);

#endif /* TC_PROCESS_H */