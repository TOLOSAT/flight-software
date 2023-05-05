/**
 * @file tasks_conf.h
 * @author Merlin Kooshmanian
 * @brief Header file stocking configuration table for tasks
 * @date 26/04/2023
 * 
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TASKS_CONF_H
#define TASKS_CONF_H

/***************************** Include Files *********************************/

#include <stdint.h>
#include "tasks.h"

/************************** Constant Definitions *****************************/

enum TASKS_ENUM {
    MAIN_TASK = 0,
    BLINK01_TASK,
    BLINK02_TASK,
    NB_TASKS,
};

#endif /* TASKS_H */