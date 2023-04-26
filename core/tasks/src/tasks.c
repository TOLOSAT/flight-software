/**
 * @file tasks.c
 * @author Merlin Kooshmanian
 * @brief Source file defining tasks
 * @date 21/04/2023
 * 
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include "tasks.h"

#if defined(STM32F411xE)
#include "stm32f4xx_nucleo_bsp.h"
#endif
#if defined(STM32F103xB)
#include "stm32f1xx_nucleo_bsp.h"
#endif

#include "conf/tasks_conf.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

extern void UsageFault_Handler(void);

/************************** Variable Definitions *****************************/

extern uint32_t   nb_tasks;
extern taskDef_t  g_normal_tasks[NB_TASKS];
extern taskId_t   g_normal_tasks_ids[NB_TASKS];

/************************* Functions Definitions *****************************/

/**
 * @fn uint32_t createTasks(void)
 * @brief Function that creates threads and links them to tasks
 * @param void
 * @return 0
 */
uint32_t createTasks(void)
{
    // Variable Initialisation
    uint32_t return_value = TASKS_SUCCESSFUL;

    // Function Core
    for(taskRef_t task = 0; task < NB_TASKS; task++){
        g_normal_tasks_ids[task] = osThreadNew(g_normal_tasks[task].task_handler, g_normal_tasks[task].handler_argument, &g_normal_tasks[task].task_attribute);
        if(g_normal_tasks_ids[task] == NULL){
            UsageFault_Handler();
        }
    }

    return(return_value);
}

/**
 * @fn uint32_t suspendTask(void)
 * @brief Function that allow to suspend an active task
 * @param task Reference of the task (in TASKS_ENUM) 
 * @retval TASKS_SUCCESSFUL if halt is successful
 * @retval TASKS_ERROR if halt cannot be performed
 */
uint32_t suspendTask(taskRef_t task){
    // Variable Initialisation
    uint32_t return_value = TASKS_SUCCESSFUL;
    osStatus_t test_value = osOK;

    // Function Core
    test_value = osThreadSuspend(g_normal_tasks_ids[task]);
    if( test_value == osOK){
        return_value = TASKS_SUCCESSFUL;
    }
    else{
        return_value = TASKS_ERROR;
    }
    return(return_value);
}


/**
 * @fn uint32_t resumeTask(void)
 * @brief Function that allow to resume a suspended tasks
 * @param taskRef_t Reference of the task (in TASKS_ENUM) 
 * @retval TASKS_SUCCESSFUL if resume is successful
 * @retval TASKS_ERROR if resume cannot be performed
 */
uint32_t resumeTask(taskRef_t task){
    // Variable Initialisation
    uint32_t return_value = TASKS_SUCCESSFUL;
    osStatus_t test_value = osOK;

    // Function Core
    test_value = osThreadResume(g_normal_tasks_ids[task]);
    if( test_value == osOK){
        return_value = TASKS_SUCCESSFUL;
    }
    else{
        return_value = TASKS_ERROR;
    }
    return(return_value);
}