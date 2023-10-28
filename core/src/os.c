/**
 * @file    os.c
 * @author  Merlin Kooshmanian
 * @brief   OS API source file
 * @date    27/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "os.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

extern void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
extern void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize);

/*************************** Functions Definitions ***************************/

#if defined(SysTick)
#undef SysTick_Handler

extern void SysTick_Handler(void);
extern void xPortSysTickHandler(void);

/**
 * @brief SysTick handler implementation that also clears overflow flag.
 */
void SysTick_Handler(void)
{
#if (configUSE_TICKLESS_IDLE == 0)
    /* Clear overflow flag */
    SysTick->CTRL;
#endif

    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        /* Call tick handler */
        xPortSysTickHandler();
    }
}

#endif /* SysTick */

/**
 * @brief vApplicationGetIdleTaskMemory
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    /* Idle task control block and stack */
    static StaticTask_t Idle_TCB;
    static StackType_t Idle_Stack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &Idle_TCB;
    *ppxIdleTaskStackBuffer = &Idle_Stack[0];
    *pulIdleTaskStackSize = (uint32_t)configMINIMAL_STACK_SIZE;
}

/**
 * @brief vApplicationGetTimerTaskMemory
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    /* Timer task control block and stack */
    static StaticTask_t Timer_TCB;
    static StackType_t Timer_Stack[configTIMER_TASK_STACK_DEPTH];

    *ppxTimerTaskTCBBuffer = &Timer_TCB;
    *ppxTimerTaskStackBuffer = &Timer_Stack[0];
    *pulTimerTaskStackSize = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}

/**
 * @brief This function is executed if a task runs out of stack
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // Unused Parameters
    (void)xTask;
    (void)pcTaskName;

    // Function Core
    while (1)
    {
        /* Do Nothing */
    }
}