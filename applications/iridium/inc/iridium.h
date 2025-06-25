/**
 * @file    iridium.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for IRIDIUM Task
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef IRIDIUM_H
#define IRIDIUM_H

/******************************* Include Files *******************************/

#include "common_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern iridiumInst_t g_iridium_inst;

/*************************** Functions Declarations **************************/

extern void IridiumMain(void);

#endif /* IRIDIUM_H */