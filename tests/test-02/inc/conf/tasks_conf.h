/**
 * @file tasks_conf.h
 * @author Merlin Kooshmanian
 * @brief Header file stocking configuration table for tasks
 * @date 02/07/2023
 * 
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TASKS_CONF_H
#define TASKS_CONF_H

/***************************** Include Files *********************************/

#include <stdint.h>
#include "tasks.h"

/************************** Constant Definitions *****************************/

/**
 * @enum    TASKS_ENUM
 * @brief   Enum defining tasks reference numbers
 */
enum TASKS_ENUM {
    /* TM/TC Flow */
    TC_RECEIVER,
    TM_SENDER,
    TC_SCHEDULER,
    TC_PROCESS,
    NB_TASKS,
};

#endif /* TASKS_H */