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

/******************************* Include Files *******************************/

#include <stdint.h>
#include "tasks.h"

/***************************** Macros Definitions ****************************/

/**
 * @enum    TASKS_ENUM
 * @brief   Enum defining tasks reference numbers
 */
enum TASKS_ENUM {
    /* Internal Software */
    SALAMI_TASK = 0,
    MISO_TASK,
    CARNE_TASK,
    /* TM/TC Flow */
    TC_RECEIVER,
    TM_SENDER,
    TC_SCHEDULER,
    TC_PROCESS,
    HK_TASK,
    /* Avionics */
    AOCS_TASK,
    POWER_TASK,
    THERMAL_TASK,
    /* Payloads */
    GRAVIMETRY_TASK,
    IRIDIUM_TASK,
    NB_TASKS,
};

#endif /* TASKS_H */