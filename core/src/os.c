/**
 * @file    os.c
 * @author  Merlin Kooshmanian
 * @brief   OS API source file
 * @date    27/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#if defined(ART_PI) || defined(NUCLEO_H745ZI)
#include "stm32h7xx.h"
#elif defined(NUCLEO_F411RE) || defined(DISCOVERY_F407VG)
#include "stm32f4xx.h"
#else
#error "Board is not supported"
#endif

#include "core_basics.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

extern void SysTick_Handler(void);
extern void xPortSysTickHandler(void);
extern void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);
extern void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize);
#if defined(configCHECK_FOR_STACK_OVERFLOW) && (configCHECK_FOR_STACK_OVERFLOW > 1)
extern void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
#endif
#if defined(configUSE_MALLOC_FAILED_HOOK) && (configUSE_MALLOC_FAILED_HOOK == 1)
extern void vApplicationMallocFailedHook(void);
#endif

/*************************** Functions Definitions ***************************/

/**
 * @fn      StartOS(void)
 * @brief   Function that starts the OS
 * @return  Nothing
 */
void StartOS(void)
{
    vTaskStartScheduler();
}

/**
 * @fn      SysTick_Handler(void)
 * @brief   SysTick handler used by the OS
 * @return  Nothing
 */
void SysTick_Handler(void)
{
#if defined(configUSE_TICKLESS_IDLE) && (configUSE_TICKLESS_IDLE == 0)
    // Clear overflow flag
    SysTick->CTRL;
#endif

    // Check if scheduler has started before incrementing SysTick
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        /* Call tick handler */
        xPortSysTickHandler();
    }
}

/**
 * @fn      vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
 * @brief   This function is used to allocate memory to Idle Task when scheduler is started
 * @return  Nothing
 *
 * Os specific function that need to be provided if static allocation is used
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    /* Idle task control block and stack */
    static StaticTask_t Idle_TCB;
    static StackType_t Idle_Stack[configMINIMAL_STACK_SIZE]; // cppcheck-suppress misra-c2012-18.8; False positive because configTIMER_TASK_STACK_DEPTH is a constant

    *ppxIdleTaskTCBBuffer = &Idle_TCB;
    *ppxIdleTaskStackBuffer = &Idle_Stack[0];
    *pulIdleTaskStackSize = (uint32_t)configMINIMAL_STACK_SIZE;
}

/**
 * @fn      vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
 * @brief   This function is used to allocate memory to timer tasks when they are created
 * @return  Nothing
 *
 * Os specific function that need to be provided if static allocation is used
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    /* Timer task control block and stack */
    static StaticTask_t Timer_TCB;
    static StackType_t Timer_Stack[configTIMER_TASK_STACK_DEPTH]; // cppcheck-suppress misra-c2012-18.8; False positive because configTIMER_TASK_STACK_DEPTH is a constant

    *ppxTimerTaskTCBBuffer = &Timer_TCB;
    *ppxTimerTaskStackBuffer = &Timer_Stack[0];
    *pulTimerTaskStackSize = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}

#if defined(configCHECK_FOR_STACK_OVERFLOW) && (configCHECK_FOR_STACK_OVERFLOW > 1)
/**
 * @fn      vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
 * @brief   This function is executed if a task runs out of stack
 * @return  Nothing
 *
 * Os specific function that need to be provided if stack overflow hook is used
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
#endif

#if defined(configUSE_MALLOC_FAILED_HOOK) && (configUSE_MALLOC_FAILED_HOOK == 1)
/**
 * @fn      vApplicationMallocFailedHook(void)
 * @brief   This function is executed when a malloc failed to attribute memory
 * @return  Nothing
 *
 * Os specific function that need to be provided if malloc failed hook is used
 */
void vApplicationMallocFailedHook(void)
{
    // Function Core
    while (1)
    {
        /* Do Nothing */
    }
}
#endif