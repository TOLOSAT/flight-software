/**
 * @file pus1.h
 * @author Merlin Kooshmanian
 * @brief Header file for PUS 1 functions
 * @date 06/07/2023
 *
 * Last Update : 06/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus1 PUS Service 1
 * Generic functions used by PUS1
 * @{
 */

#ifndef PUS1_H
#define PUS1_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

pusStatus_t BuildS1SS1(pusTC_t *tc, pusTM_t *acceptance_tm);
pusStatus_t BuildS1SS2(pusTC_t *tc, pusTM_t *acceptance_tm, pusAcceptanceError_t acceptance_error);
pusStatus_t BuildS1SS7(pusTC_t *tc, pusTM_t *execution_tm);
pusStatus_t BuildS1SS8(pusTC_t *tc, pusTM_t *execution_tm, pusAcceptanceError_t acceptance_error);

#endif /* PUS1_H */

/** 
 * @} 
 * @} 
 */