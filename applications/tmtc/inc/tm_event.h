/**
 * @file    tm_event.h
 * @author  Arthur Morain, Matteo Planchet
 * @brief   Header file for building event-related TMs
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

#ifndef TM_EVENT_H
#define TM_EVENT_H

/******************************* Include Files *******************************/

#include "common_types.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void ReportEvent(severityLevel_t level);

#endif /* TM_EVENT_H */