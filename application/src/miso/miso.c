/**
 * @file    miso.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for MISO Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

// To be cleaned !!!!!
#include "miso/miso.h"
#include "core_basics.h"
#include "fdir.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"
#include "FreeRTOS.h"
#include "../../../tools/OS/FreeRTOS/include/task.h"
#include "timers.h"
#include "string.h"
#include <stdio.h>
#include "../../core/inc/io_instances.h"
#include "services/pus161.h"
#include "tc_execution.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/* ================== get the idle time of the system ================== 
* @brief           get the idle time of the system
* @return          uint32_t : idle time
========================================================================*/
uint32_t getIdleTime(){
    uint32_t IdleTime  = ulTaskGetIdleRunTimePercent();
    //TODO : supprimer le printf
    printf("Idle Time : %ld\n", IdleTime);
    return IdleTime;
}

/* ================== get the stack usage of the system ==================
* @brief           get the maximum stack usage of the system
* @return          uint32 : stack usage
========================================================================*/
uint32_t getStackUsage()
{
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime;
    uint32_t maxStackUsage = 100000000;

    /* Take a snapshot of the number of tasks in case it changes while this
    function is executing. */
    uxArraySize = uxTaskGetNumberOfTasks();

    /* Allocate a TaskStatus_t structure for each task.  An array could be
    allocated statically at compile time. */
    pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

    if( pxTaskStatusArray != NULL )
    {
        /* Generate raw status information about each task. */
        uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );

        /* Check the stack for each task */
        for( x = 0; x < uxArraySize; x++ )
        {
            if (pxTaskStatusArray[x].usStackHighWaterMark < maxStackUsage)
            {
                maxStackUsage = pxTaskStatusArray[x].usStackHighWaterMark;
            }
        }

        /* The array is no longer needed, free the memory it consumes. */
        vPortFree( pxTaskStatusArray );   
    }

    return maxStackUsage;
}


/*************************** Variables Definitions ***************************/

/**
 * @var     g_pus161_execution_table
 * @brief   Execution table for incomming pus 161 TC 
 * @warning Keys must be ordered from smallest to largest
 */

pusExecutionTable_t g_pus161_execution_table[NB_PUS161_EXECUTION] = 
{
    { BUILD_ROUTING_KEY(OBC_APID, 161u, 1u) , ExecuteS161SS1 , TM_NOT_REQUESTED },
};

/*************************** Functions Definitions ***************************/

/**
 * @fn              MisoMain(void *task_dyn_conf)
 * @brief           Main of the MISO Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void IN_MISO_TEXT_SECTION MisoMain(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status, stackUsage, idleTime;

    // Initialisation
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = CheckExecutionTable((pusExecutionTable_t *) &g_pus161_execution_table, NB_PUS161_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);   


    // Function Core
    while (1)
    {
        //Check TC presence
        // Execute incoming TC
        
        const tcExecutionBasicBuffers_t basic_buffers = {TC_PUS161, NO_BUFFER_REF, TM_PUS161};
        task_status = ExecuteTC((pusExecutionTable_t *)&g_pus161_execution_table, NB_PUS161_EXECUTION, basic_buffers);
        CheckErrors(task_status, FDIR_NO_SANCTION);
        


        //Check stack usage
        //TODO : généraliser à toutes les taches
        stackUsage = getStackUsage();
        printf("Stack Usage : %ld\n", stackUsage); //TODO : supprimer le printf
        if (stackUsage == 1)
        {
            //overflow
        }
        else if (stackUsage < 20)
        {
            //overflow proche
        }

        //Check runtimes
        idleTime = getIdleTime();
        printf("Idle Time : %ld\n", idleTime); //TODO : supprimer le printf

        //Generate report
        if ((idleTime > 80) || (stackUsage < 20))
        {
            //TODO : Generate report
        }

        //TODO : Life signal

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
        
    }
}