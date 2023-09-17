/**
 * @file    pus6.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 6 functions (Memory management)
 * @date    08/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus6 PUS Service 6
 * Generic functions used by PUS6 (Memory management)
 * @{
 */

#ifndef PUS6_H
#define PUS6_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

pusStatus_t ExecuteS6SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
pusStatus_t ExecuteS6SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
pusStatus_t BuildS6SS4(pusTM_t *tm, pusTMDumpDataField_t *memory_dump);

#endif /* PUS6_H */

/** 
 * @} 
 * @} 
 */
