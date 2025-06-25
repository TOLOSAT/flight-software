/**
 * @file    dummy_tasks.h
 * @author  Merlin Kooshmanian
 * @brief   Header file with dummy tasks
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef DUMMY_TASKS_H
#define DUMMY_TASKS_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "drv/iridium_driver.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern iridiumInst_t g_iridium_inst;

/*************************** Functions Declarations **************************/

extern void DummyMainTask(void);

#endif /* DUMMY_TASKS_H */