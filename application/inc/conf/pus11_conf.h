/**
 * @file    pus11_conf.h
 * @author  Merlin Kooshmanian
 * @brief   Header file stocking configuration for pus11 environment
 * @date    14/09/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef PUS11_CONF_H
#define PUS11_CONF_H

/******************************* Include Files *******************************/

#include "services/pus11.h"
#include "pus_tools/schedule_management.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern pusSchedule_t g_pus11_schedule;
extern pus11DataTable_t g_pus11_data_table;

#endif /* PUS11_CONF_H */