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
 * @fn              MisoMain(void *task_desc)
 * @brief           Main of the MISO Task
 * @param[in,out]   task_desc Descriptor of the current task
 */
void IN_MISO_TEXT_SECTION MisoMain(void *task_desc)
{
    // Variable Initialisation
    uint32_t task_status;
    static pusExecutionTable_t IN_MISO_DATA_SECTION miso_execution_table[NB_PUS161_EXECUTION] =
    {
        {BUILD_ROUTING_KEY(OBC_APID, 161u, 1u), ExecuteS161SS1, TM_REQUESTED},
        {BUILD_ROUTING_KEY(OBC_APID, 161u, 3u), ExecuteS161SS3, TM_REQUESTED},
        {BUILD_ROUTING_KEY(OBC_APID, 161u, 5u), ExecuteS161SS5, TM_REQUESTED},
    };
    monitoringSystemUsage_t *system_usage = NULL;

    // Initialisation
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = CheckExecutionTable((pusExecutionTable_t *)&miso_execution_table, NB_PUS161_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Initialise PUS161
    task_status = InitS161(NB_TASKS, &system_usage);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Check system usage
        task_status = GetSystemUsage(system_usage);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);

        // Executes a TC.
        task_status = ExecuteTC((pusExecutionTable_t *)&miso_execution_table, NB_PUS161_EXECUTION, TC_PUS161, TM_PUS161, TM_PUS1);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        // Generate Event
        if (system_usage->max_stack_usage > MAX_STACK_USAGE)
        {
            // Add more events level (Medium Severity & High Severity ??)
            // Generate event (message -> CARNE -> PUS)
            // Generate message to SALAMI (if high severity event)
            // Generate TM (if not severe only)
        }

        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}
