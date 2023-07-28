/**
 * @file pus17.c
 * @author Merlin Kooshmanian
 * @brief Header file for PUS 17 functions
 * @date 12/07/2023
 *
 * Last Update : 12/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus17 PUS Service 17
 * Generic functions used by PUS17
 * @{
 */

#ifndef PUS17_H
#define PUS17_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

pusStatus_t ExecuteS17SS1(pusTC_t *tc, pusTM_t *tm);

#endif /* PUS17_H */

/** 
 * @} 
 * @} 
 */