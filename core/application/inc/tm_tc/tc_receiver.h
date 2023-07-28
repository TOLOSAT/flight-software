/**
 * @file tc_receiver.h
 * @author Merlin Kooshmanian
 * @brief Header file for TC_RECEIVER Task
 * @date 02/07/2023
 * 
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TC_RECEIVER_H
#define TC_RECEIVER_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

#define NB_ROUTES       1u                  /**< Number of routes */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern pusRoutingTable_t g_tc_routing_table[NB_ROUTES];

/*************************** Functions Declarations **************************/

void TcReceiverMain(void *task_dyn_conf);

#endif /* TC_RECEIVER_H */