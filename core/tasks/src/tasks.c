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
    for(uint32_t task_index = 0; task_index < NB_TASKS; task_index++){
        g_normal_tasks_ids[task_index] = osThreadNew(g_normal_tasks[task_index].task_handler, g_normal_tasks[task_index].handler_argument, &g_normal_tasks[task_index].task_attribute);
        if(g_normal_tasks_ids[task_index] == NULL){
            UsageFault_Handler();
        }
    }
    return(0);
}