/**
 * @file    miso.c
 * @author  Merlin Kooshmanian & Clement Cognard & Yann Awbi
 * @brief   Source file for MISO Task
 * @date    28/06/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "miso/miso.h"
#include "kernel.h"
#include "pus.h"
#include "services/pus161.h"

/***************************** Macros Definitions ****************************/

#define NB_PUS161_EXECUTION     3u                       /**< Number of pus161 exution functions */
#define MAX_STACK_USAGE         80u                     /**< Maximum stack usage authorized in percent */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              MisoMain(void)
 * @brief           Main of the MISO Task
 */
void IN_MISO_TEXT_SECTION MisoMain(void)
{
    // Initialisation
    uint32_t task_status;
    monitoringSystemUsage_t *system_usage = NULL;
    static pusExecutionTable_t IN_MISO_DATA_SECTION miso_exec_tab[NB_PUS161_EXECUTION] =
    {
        {BUILD_ROUTING_KEY(OBC_APID, 161u, 1u), ExecuteS161SS1, TM_REQUESTED},
        {BUILD_ROUTING_KEY(OBC_APID, 161u, 3u), ExecuteS161SS3, TM_REQUESTED},
        {BUILD_ROUTING_KEY(OBC_APID, 161u, 5u), ExecuteS161SS5, TM_REQUESTED},
    };
    static pusExecutionContext_t IN_MISO_DATA_SECTION miso_tc_context =
    {
        .execution_table = miso_exec_tab,
        .execution_table_size = NB_PUS161_EXECUTION,
        .buffer_tc = TC_PUS161,
        .buffer_tm = TM_PUS161,
        .buffer_ack = TM_PUS1,
    };
    task_status =  InitTCExecutionContext(&miso_tc_context);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitS161(NB_TASKS, &system_usage);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Check system usage
        task_status = GetSystemUsage(system_usage);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);

        // Executes a TC.
        task_status = ExecuteTC(&miso_tc_context);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        // Generate Event
        if (system_usage->max_stack_usage > MAX_STACK_USAGE)
        {
            // Add more events level (Medium Severity & High Severity ??)
            // Generate event (message -> CARNE -> PUS)
            // Generate message to SALAMI (if high severity event)
            // Generate TM (if not severe only)
        }

        SleepPeriodic();
    }
}
