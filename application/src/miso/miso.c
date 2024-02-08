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

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

//handle for Periodic Stats Task

//test task
static uint32_t getIdleTime();

/**
 * @fn              MisoMain(void *task_dyn_conf)
 * @brief           Main of the MISO Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void IN_MISO_TEXT_SECTION MisoMain(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);


    // Function Core
    while (1)
    {
        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}

static uint32_t getIdleTime(){
    uint32_t IdleTime  = ulTaskGetIdleRunTimePercent();
    //TODO : supprimer le printf
    printf("Idle Time : %ld\n", IdleTime);
    return IdleTime;
}