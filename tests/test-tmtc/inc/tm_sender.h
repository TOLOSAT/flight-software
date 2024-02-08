/**
 * @file    tm_sender.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TM_SENDER Task
 * @date    02/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TM_SENDER_H
#define TM_SENDER_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "buffers.h"

/***************************** Macros Definitions ****************************/

#define NB_ENTRY_BUFFERS    3u                  /**< Maximum number of input buffers */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern const bufferRef_t g_tm_sender_buffer_entry[NB_ENTRY_BUFFERS];

/*************************** Functions Declarations **************************/

extern void TmSenderMain(void *task_dyn_conf);

#endif /* TM_SENDER_H */