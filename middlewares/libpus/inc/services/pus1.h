/**
 * @file    pus1.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 1 functions (Request verification)
 * @date    06/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus1 PUS Service 1
 * Generic functions used by PUS1 (Request verification)
 * @{
 */

#ifndef PUS1_H
#define PUS1_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

#define S1SS1_DATA_SIZE 4u /**< Size of PUS S1SS1 data field */
#define S1SS2_DATA_SIZE 5u /**< Size of PUS S1SS2 data field */
#define S1SS7_DATA_SIZE 4u /**< Size of PUS S1SS7 data field */
#define S1SS8_DATA_SIZE 5u /**< Size of PUS S1SS8 data field */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern pusStatus_t BuildS1SS1(const pusTC_t *tc, pusTM_t *acceptance_tm);
extern pusStatus_t BuildS1SS2(const pusTC_t *tc, pusTM_t *acceptance_tm, pusAcceptanceError_t acceptance_error);
extern pusStatus_t BuildS1SS7(const pusTC_t *tc, pusTM_t *execution_tm);
extern pusStatus_t BuildS1SS8(const pusTC_t *tc, pusTM_t *execution_tm, pusExecutionError_t execution_error);

#endif /* PUS1_H */

/** 
 * @} 
 * @} 
 */