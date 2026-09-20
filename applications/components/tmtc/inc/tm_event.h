/**
 * @file    tm_event.h
 * @author  Arthur Morain, Matteo Planchet
 * @brief   Header file for building event-related TMs
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
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

/**
 * @fn    ReportEvent(severityLevel_t level)
 * @brief Event reporting callback.
 * @param level Severity level of the error.
 */
extern void ReportEvent(severityLevel_t level);

#endif /* TM_EVENT_H */