/**
 * @file    pus160.c
 * @author  Théo Bessel
 * @brief   Source file for PUS 160 functions (System management)
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "kernel.h"
#include "tm_management.h"
#include "services/pus160.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

// static returnCode_t BuildS160SS1(pusTM_t *tm);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          ExecuteS160SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that ...
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for ... TM
 */
returnCode_t ExecuteS160SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value      = RET_SUCCESSFUL;

    (void)(tc);
    (void)(tm);
    *error_code = PUS_EXECUTION_NO_ERROR;

    LOG("[TM/TC] ExecuteS160SS1\n");

    return return_value;
}

// /**
//  * @fn          BuildS160SS1(pusTM_t *tm)
//  * @brief       Function that send S160SS1 TM
//  * @param[out]  tm TM that will be sent
//  * @param[in]   memory_dump Data dumped that will be send
//  */
// static returnCode_t BuildS160SS1(pusTM_t *tm)
// {
//     returnCode_t return_value = RET_SUCCESSFUL;

//     // BuildTM(...)

//     return return_value;
// }