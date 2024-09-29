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
 * @fn              HkMain(void *task_desc)
 * @brief           Main of the HK_MGMT Task
 * @param[in,out]   task_desc Descriptor of the current task
 */
void IN_HK_TEXT_SECTION HkMain(void *task_desc)
{
    // Variable Initialisation
    uint32_t task_status;
    static pusExecutionTable_t IN_HK_DATA_SECTION hk_exec_tab[NB_PUS3_EXECUTION] =
    {
        { BUILD_ROUTING_KEY(OBC_APID, 3u, 5u) , ExecuteS3SS5 , TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 3u, 6u) , ExecuteS3SS6 , TM_NOT_REQUESTED },
    };
    static pusExecutionContext_t IN_HK_DATA_SECTION hk_tc_context =
    {
        .execution_table = hk_exec_tab,
        .execution_table_size = NB_PUS3_EXECUTION,
        .buffer_tc = TC_PUS3,
        .buffer_tm = NO_BUFFER,
        .buffer_ack = TM_PUS1,
    };

    // Initialisation
    task_status =  InitTCExecutionContext(&hk_tc_context);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Execute incoming TC
        task_status = ExecuteTC(&hk_tc_context);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}
