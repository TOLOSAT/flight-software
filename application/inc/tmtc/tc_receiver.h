/**
 * @file    tc_receiver.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TC_RECEIVER Task
 * @date    02/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TC_RECEIVER_H
#define TC_RECEIVER_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

#define NB_ROUTES   10u  /**< Number of routes */

// Section placement macros
#define IN_TMTC_TEXT_SECTION    __attribute__((section(".text_tmtc")))  /**< TMTC functions goes in the .text_tmtc */
#define IN_TMTC_DATA_SECTION    __attribute__((section(".data_tmtc")))  /**< TMTC data goes in the .data_tmtc */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern pusRoutingTable_t g_tc_routing_table[NB_ROUTES];

/*************************** Functions Declarations **************************/

extern void TcReceiverMain(void *task_dyn_conf);

#endif /* TC_RECEIVER_H */