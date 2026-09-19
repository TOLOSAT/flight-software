/**
 * @file    iridium.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for IRIDIUM Task
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

#ifndef IRIDIUM_H
#define IRIDIUM_H

/******************************* Include Files *******************************/

#include "common_types.h"
#include "drivers/iridium9603.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/**
 * @var     g_iridium_inst
 * @brief   Iridium instance declaration
 */
extern iridiumInst_t g_iridium_inst;

/*************************** Functions Declarations **************************/

/**
 * @fn              IridiumMain(void)
 * @brief           Main of the IRIDIUM Task
 */
extern void IridiumMain(void);

#endif /* IRIDIUM_H */