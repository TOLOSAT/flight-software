/**
 * @file tc_receiver.h
 * @author Merlin Kooshmanian
 * @brief Header file for TC_RECEIVER Task
 * @date 28/06/2023
 * 
 * Last Update : 28/06/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TC_RECEIVER_H
#define TC_RECEIVER_H

/******************************* Include Files *******************************/

#include <stdint.h>

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

void TcReceiverMain(void *task_dyn_conf);

#endif /* TC_RECEIVER_H */