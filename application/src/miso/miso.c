/**
 * @file    miso.c
 * @author  Merlin Kooshmanian & Clement Cognard
 * @brief   Source file for MISO Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "miso/miso.h"
#include "core_basics.h"
#include "tc_execution.h"
#include "miso/pus161.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static uint32_t getIdleTime(void);
static UBaseType_t getStackUsage(void);
// static char* getState(void);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_miso_execution_table
 * @brief   Execution table for incomming pus 161 TC 
 * @warning Keys must be ordered from smallest to largest
 */

pusExecutionTable_t g_miso_execution_table[NB_PUS161_EXECUTION] = 
{
    { BUILD_ROUTING_KEY(OBC_APID, 161u, 1u) , ExecuteS161SS1 , TM_NOT_REQUESTED },
    { BUILD_ROUTING_KEY(OBC_APID, 161u, 3u) , ExecuteS161SS3 , TM_NOT_REQUESTED },
    { BUILD_ROUTING_KEY(OBC_APID, 161u, 5u) , ExecuteS161SS5 , TM_NOT_REQUESTED },
};

/**
 * @var     systemState
 * @brief   System state
 */
char systemState[100];

/*************************** Functions Definitions ***************************/

/**
 * @fn              MisoMain(void *task_dyn_conf)
 * @brief           Main of the MISO Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void IN_MISO_TEXT_SECTION MisoMain(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;
    pus161Data_t pus161_data;

    // Initialisation
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = CheckExecutionTable((pusExecutionTable_t *) &g_miso_execution_table, NB_PUS161_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);   

    // Initialise PUS161
    task_status = InitS161(&pus161_data);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Check stack usage
        pus161_data.max_stack_usage = getStackUsage();

        // Check runtimes
        pus161_data.idle_time = getIdleTime();

        // Executes a TC.
        const tcExecutionBasicBuffers_t basic_buffers = {TC_PUS161, NO_BUFFER_REF, TM_PUS161};
        task_status = ExecuteTC((pusExecutionTable_t *)&g_miso_execution_table, NB_PUS161_EXECUTION, basic_buffers);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        // Generate report
        if ((pus161_data.idle_time < 10) || (pus161_data.max_stack_usage > (configMINIMAL_STACK_SIZE-20)))
        {
            // TODO : Generate report
        }

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);   
    }
}

/**
 * @fn      getIdleTime(void)
 * @brief   Retrieves the idle time of the system.
 * @return  The idle time as a uint32_t value.
 *
 * This function returns the percentage of time that the system has been idle.
 */
static uint32_t getIdleTime(void)
{
    uint32_t IdleTime  = ulTaskGetIdleRunTimePercent();
    return IdleTime;
}

/**
 * @fn      getStackUsage(void)
 * @brief   Retrieves the maximum stack usage among all tasks in the system.
 * @return  The maximum stack usage among all tasks in the system.
 *
 * This function takes a snapshot of the current state of all tasks and calculates
 * the maximum stack usage among them.
 */
static UBaseType_t getStackUsage(void) 
{
    TaskStatus_t pxTaskStatusArray[NB_TASKS*2];
    UBaseType_t uxArraySize, x;

    // Take a snapshot of all task states.
    uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, NB_TASKS*2, NULL );

    // Retrieve the task with the highest stack usage.
    UBaseType_t maxStackUsage = 0;
    for( x = 0; x < uxArraySize; x++ )
    {
        if (pxTaskStatusArray[ x ].usStackHighWaterMark > maxStackUsage)
        {
            maxStackUsage = pxTaskStatusArray[ x ].usStackHighWaterMark;
        }
    }
    return maxStackUsage;
}

// Need to be written properly
// /**
//  * @fn getState(void)
//  * @brief Returns the system state as a string.
//  * @return A pointer to a string representing the system state.
//  * 
//  * The system state includes information about each task in the system, such as task name, current state,
//  * stack high watermark, task number, base priority, current priority, and run time counter.
//  */
// static char* getState(void) 
// {
//     TaskStatus_t pxTaskStatusArray[NB_TASKS*2];
//     uxTaskGetSystemState( pxTaskStatusArray, NB_TASKS*2, NULL );
//     for (int i = 0; i < NB_TASKS; i++)
//     {
//         sprintf(systemState, "%s,%d,%d,%lu,%lu,%lu,%lu", pxTaskStatusArray[i].pcTaskName, 
//                                                         pxTaskStatusArray[i].eCurrentState,
//                                                         pxTaskStatusArray[i].usStackHighWaterMark,
//                                                         (unsigned long)pxTaskStatusArray[i].xTaskNumber,
//                                                         (unsigned long)pxTaskStatusArray[i].uxBasePriority, 
//                                                         (unsigned long)pxTaskStatusArray[i].uxCurrentPriority,
//                                                         (unsigned long)pxTaskStatusArray[i].ulRunTimeCounter);
//     }
//     return systemState;
// }
