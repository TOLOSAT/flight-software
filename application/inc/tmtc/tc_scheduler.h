/**
 * @file    tc_scheduler.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TC_SCHEDULER Task
 * @date    02/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TC_SCHEDULER_H
#define TC_SCHEDULER_H

/******************************* Include Files *******************************/

#include "application_types.h"
#include "pus_common.h"

/***************************** Macros Definitions ****************************/

#define NB_PUS11_EXECUTION    4u  /**< Number of pus11 exution functions */

// Section placement macros
#define IN_TMTC_TEXT_SECTION    __attribute__((section(".text_tmtc")))  /**< TMTC functions goes in the .text_tmtc */
#define IN_TMTC_DATA_SECTION    __attribute__((section(".data_tmtc")))  /**< TMTC data goes in the .data_tmtc */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern pusExecutionTable_t g_pus11_execution_table[NB_PUS11_EXECUTION];

/*************************** Functions Declarations **************************/

extern void TcSchedulerMain(void *task_desc);

#endif /* TC_SCHEDULER_H */