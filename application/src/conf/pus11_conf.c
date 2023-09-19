/**
 * @file    pus11_conf.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking configuration for pus11 environment
 * @date    14/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "conf/pus11_conf.h"

/*************************** Variables Definitions ***************************/

/**
 * @var     g_pus11_schedule
 * @brief   Schedule for pus11, containing scheduled TC informations
 */
pusSchedule_t g_pus11_schedule = {0};
