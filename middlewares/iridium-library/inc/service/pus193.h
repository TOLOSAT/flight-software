/**
 * @file    pus193.h
 * @author  Yann Awbi
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 193 (Iridium) functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef PUS193_H
#define PUS193_H

/******************************* Include Files *******************************/

#include "pus_types.h"
#include "drv/iridium_driver.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t InitS193(iridiumInst_t *iridium_inst);
extern returnCode_t ExecuteS193SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS193SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS193SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS193SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS193SS7(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS193SS8(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS193_H */