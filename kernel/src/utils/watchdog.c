/**
 * @file    watchdog.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for watchdog functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "utils/watchdog.h"
#include "core/tasks.h"
#include "drv/peripherals.h"
#include "utils/console.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

#define WATCHDOG_PERIOD_MS  500u                /**< Watchdog task period */ 
#define WATCHDOG_PRIORITY   PRIORITY_EXTREME    /**< Watchdog task priority */ 
#define WATCHDOG_STACK_SIZE 512u                /**< Watchdog task stack size */ 

/*************************** Variables Definitions ***************************/

/**
 * @var     watchdog_task_handle
 * @brief   Watchdog task handle struct
 */
static taskHandle_t watchdog_task_handle = {0};

/**
 * @var     watchdog_task_stack
 * @brief   Watchdog task control block
 */
static taskStack_t watchdog_task_stack[WATCHDOG_STACK_SIZE/sizeof(taskStack_t)] __attribute__((aligned(WATCHDOG_STACK_SIZE))) = {0};

/**
 * @var     watchdog_task_tcb
 * @brief   Watchdog task stack
 */
static taskTCB_t watchdog_task_tcb = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitWatchdog(void)
 * @brief       Initialise the watchdog task
 * @retval      RET_ERROR if watchdog task initialisation failed
 * @retval      RET_SUCCESSFUL else
 */
returnCode_t InitWatchdog(void)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    watchdog_task_handle = xTaskCreateStatic((taskFunction_t)WatchdogMain, "WatchdogMain",
                                            WATCHDOG_STACK_SIZE / sizeof(StackType_t),
                                            NULL,WATCHDOG_PRIORITY, watchdog_task_stack, 
                                            &watchdog_task_tcb);

    if (watchdog_task_handle == NULL)
    {
        return_value = RET_ERROR;
    }

    return return_value;
}

/**
 * @fn              WatchdogMain(void)
 * @brief           Main of the Watchdog Task
 */
void WatchdogMain(void)
{
    // Initialisation
    tick_t last_wake = xTaskGetTickCount();

    // Function Core
    while (1)
    {
        // Console printing
        ConsolePrint("Hello\n");

        // Blink status LED
        (void)PeripheralIoctl(USER_LED, GPIO_IOCTL_TOGGLE, NULL, 0u);

        // Pet the dog
        // TO DO : add watchdog support

        // Sleep until next period
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(WATCHDOG_PERIOD_MS));
    }
}