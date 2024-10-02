/**
 * @file    housekeeping.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for HK_MGMT Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "housekeeping/housekeeping.h"
#include "kernel.h"
#include "pus.h"
#include "services/pus1.h"
#include "services/pus3.h"

/***************************** Macros Definitions ****************************/

#define NB_PUS3_EXECUTION    2u  /**< Number of pus3 exution functions */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              HkMain(void)
 * @brief           Main of the HK_MGMT Task
 */
void HkMain(void)
{
    // Initialisation
    uint32_t task_status;
    static pusExecutionTable_t hk_exec_tab[NB_PUS3_EXECUTION] =
    {
        { BUILD_ROUTING_KEY(OBC_APID, 3u, 5u) , ExecuteS3SS5 , TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 3u, 6u) , ExecuteS3SS6 , TM_NOT_REQUESTED },
    };
    static pusExecutionContext_t hk_tc_context =
    {
        .execution_table = hk_exec_tab,
        .execution_table_size = NB_PUS3_EXECUTION,
        .buffer_tc = TC_PUS3,
        .buffer_tm = NO_BUFFER,
        .buffer_ack = TM_PUS1,
    };
    task_status =  InitTCExecutionContext(&hk_tc_context);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Execute incoming TC
        task_status = ExecuteTC(&hk_tc_context);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        SleepPeriodic();
    }
}
