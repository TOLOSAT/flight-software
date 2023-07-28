/**
 * @file time_management.h
 * @author Merlin Kooshmanian
 * @brief Header file for Time Management
 * @date 23/07/2023
 *
 * Last Update : 23/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup time_management Time Management
 * Functions that allows to format time according to CUC
 * @{
 */

#ifndef TIME_MANAGEMENT_H
#define TIME_MANAGEMENT_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

pusStatus_t GetCUCTime(cucTime_t *cuc_time);
pusStatus_t SetCUCTime(cucTime_t *cuc_time);

#endif /* TIME_MANAGEMENT_H */

/** 
 * @} 
 * @} 
 */