/**
 * @file    pus5.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 5 functions (Event reporting)
 * @date    06/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus5 PUS Service 5
 * Generic functions used by PUS5 (Event reporting)
 * @{
 */

#ifndef PUS5_H
#define PUS5_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

pusStatus_t BuildS5SS1234(pusTM_t *tm, pusEventSeverity_t severity, eventReport_t *report);

#endif /* PUS5_H */

/** 
 * @} 
 * @} 
 */