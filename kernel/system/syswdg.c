/**
 * @file    syswdg.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for system watchdog handling
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/syswdg.h"
#include "core/tasks.h"
#include "drv/others/drv_wdg.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define SYSWDG_PRIORITY   PRIORITY_EXTREME /**< SYSWDG task priority */
#define SYSWDG_STACK_SIZE 1024u            /**< SYSWDG task stack size */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitSYSWDG(void)
 * @brief   Enables System Watchdog
 * @return  Nothing
 */
void InitSYSWDG(void)
{
    static taskHandle_t syswdg_task_handle = { 0 };

    // Then create Syswdg Task
    BaseType_t test_creation =
        xTaskCreate((taskFunction_t)SYSWDGMain, "SYSWDG", SYSWDG_STACK_SIZE / sizeof(StackType_t), NULL, SYSWDG_PRIORITY, &syswdg_task_handle);
    if (test_creation == pdFAIL)
    {
        KernelPanic();
    }
}

/**
 * @fn              SYSWDGMain(void)
 * @brief           Main of the SYSWDG task
 */
void SYSWDGMain(void)
{
    // Initialise watchdog
#if defined(CONFIG_WDG)
    CheckError(InitWatchDog(2u * (uint32_t)CONFIG_SYSWDG_PERIOD_MS));
#endif

    // Initialisation
    tick_t last_wake = xTaskGetTickCount();

    // Task Core
    while (1)
    {
#if defined(CONFIG_WDG)
        // Update WatchDog
        PetWatchDog();
#endif

        // Sleep until next period
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(CONFIG_SYSWDG_PERIOD_MS));
    }
}
