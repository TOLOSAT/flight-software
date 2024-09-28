/**
 * @file    tc_scheduler.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_SCHEDULER Task
 * @date    02/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tmtc/tc_scheduler.h"
#include "kernel.h"
#include "pus.h"
#include "services/pus1.h"
#include "services/pus11.h"

/***************************** Macros Definitions ****************************/

#define NB_PUS11_EXECUTION    4u  /**< Number of pus11 exution functions */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              TcSchedulerMain(void *task_desc)
 * @brief           Main of the TC_SCHEDULER Task
 * @param[in,out]   task_desc Descriptor of the current task
 */
void IN_TMTC_TEXT_SECTION TcSchedulerMain(void *task_desc)
{
    // Variable Initialisation
    uint32_t task_status;
    static pusExecutionTable_t IN_TMTC_DATA_SECTION pus11_execution_table[NB_PUS11_EXECUTION] = 
    {
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 1u) , ExecuteS11SS1 , TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 2u) , ExecuteS11SS2 , TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 3u) , ExecuteS11SS3 , TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 4u) , ExecuteS11SS4 , TM_NOT_REQUESTED },
    };
    deviceNo_t dev_tc_pus11_buffer = 0u;
    deviceNo_t dev_delayed_tc_buffer = 0u;
    deviceNo_t dev_ack_buffer = 0u;

    // Initialisation
    task_status = InitExecutionTable((pusExecutionTable_t *) &pus11_execution_table, NB_PUS11_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPus11();
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceOpen(&dev_tc_pus11_buffer, DEVICE_TYPE_BUFFER, TC_PUS11, DEVICE_NO_EXTRA_INFO);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceOpen(&dev_delayed_tc_buffer, DEVICE_TYPE_BUFFER, TC_DELAYED, DEVICE_NO_EXTRA_INFO);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceOpen(&dev_ack_buffer, DEVICE_TYPE_BUFFER, TM_PUS1, DEVICE_NO_EXTRA_INFO);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Execute incoming TC
        task_status = ExecuteTC((pusExecutionTable_t *)&pus11_execution_table, NB_PUS11_EXECUTION, dev_tc_pus11_buffer, NO_DEVICE, dev_ack_buffer);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        // Process delayed TC
        task_status = ProcessDelayedTC(dev_delayed_tc_buffer);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}
