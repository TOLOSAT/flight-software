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
    static pusExecutionTable_t IN_HK_DATA_SECTION pus3_execution_table[NB_PUS3_EXECUTION] = 
    {
        { BUILD_ROUTING_KEY(OBC_APID, 3u, 5u) , ExecuteS3SS5 , TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 3u, 6u) , ExecuteS3SS6 , TM_NOT_REQUESTED },
    };
    deviceNo_t dev_tc_pus3_buffer = 0u;
    deviceNo_t dev_ack_buffer = 0u;

    // Initialisation
    task_status = InitExecutionTable((pusExecutionTable_t *) &pus3_execution_table, NB_PUS3_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceOpen(&dev_tc_pus3_buffer, DEVICE_TYPE_BUFFER, TC_PUS3, DEVICE_NO_EXTRA_INFO);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceOpen(&dev_ack_buffer, DEVICE_TYPE_BUFFER, TM_PUS1, DEVICE_NO_EXTRA_INFO);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Execute incoming TC
        task_status = ExecuteTC((pusExecutionTable_t *)&pus3_execution_table, NB_PUS3_EXECUTION, dev_tc_pus3_buffer, NO_DEVICE, dev_ack_buffer);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}
