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

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_TMTC_TEXT_SECTION    __attribute__((section(".text_tmtc")))  /**< TMTC functions goes in the .text_tmtc */
#define IN_TMTC_DATA_SECTION    __attribute__((section(".data_tmtc")))  /**< TMTC data goes in the .data_tmtc */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void TmSenderMain(void);

#endif /* TM_SENDER_H */