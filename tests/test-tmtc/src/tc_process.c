/**
 * @file    tc_process.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_PROCESS Task
 * @date    02/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tc_process.h"
#include "kernel.h"
#include "pus.h"
#include "services/pus1.h"
#include "services/pus6.h"
#include "services/pus9.h"
#include "services/pus17.h"

/***************************** Macros Definitions ****************************/

#define NB_NORMAL_EXECUTION    4u  /**< Number of exution functions */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              TcProcessMain(void)
 * @brief           Main of the TC_PROCESS Task
 */
void TcProcessMain(void)
{
    // Initialisation
    uint32_t task_status;
    static pusExecutionTable_t normal_exec_tab[NB_NORMAL_EXECUTION] =
    {
        {BUILD_ROUTING_KEY(OBC_APID, 6u, 1u)   , ExecuteS6SS1   , TM_NOT_REQUESTED },
        {BUILD_ROUTING_KEY(OBC_APID, 6u, 3u)   , ExecuteS6SS3   , TM_REQUESTED     },
        {BUILD_ROUTING_KEY(OBC_APID, 9u, 128u) , ExecuteS9SS128 , TM_NOT_REQUESTED },
        {BUILD_ROUTING_KEY(OBC_APID, 17u, 1u)  , ExecuteS17SS1  , TM_REQUESTED     },
    };
    static pusExecutionContext_t normal_tc_context =
    {
        .execution_table = normal_exec_tab,
        .execution_table_size = NB_NORMAL_EXECUTION,
        .buffer_tc = TC_NORMAL,
        .buffer_tm = TM_NORMAL,
        .buffer_ack = TM_PUS1,
    };
    task_status = InitTCExecutionContext(&normal_tc_context);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Execute incoming TC
        task_status = ExecuteTC(&normal_tc_context);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        SleepPeriodic();
    }
}
