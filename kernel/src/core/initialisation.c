/**
 * @file    initialisation.c
 * @author  Merlin Kooshmanian
 * @brief   Source file initialising tools and HAL
 * @date    21/01/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core/initialisation.h"
#include "core/tasks.h"
#include "core/buffers.h"
#include "fs/fs.h"
#include "fdir/fdir.h"
#include "drv/drv_common.h"
#include "drv/drv_rtc.h"
#include "drv/peripherals.h"
#include "utils/console.h"
#include "utils/sys_info.h"
#include "utils/ecc.h"
#include "utils/monitoring.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void InitCache(void);
static void EnableFaultHandlers(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      init(void)
 * @brief   Function that initialise tools and HAL
 */
void init(void)
{
    // First Enable Fault Handlers
    EnableFaultHandlers();

    // Cache Initialisation
    InitCache();

    // HAL Initialisation
    CheckErrors(InitHal(), FDIR_ERROR_HANDLER);

    // BSP Late Initialisation
    CheckErrors(BSPLateInit(), FDIR_ERROR_HANDLER);

    // Peripherals Initialisation
    CheckErrors(InitPeripherals(), FDIR_ERROR_HANDLER);

    // RTC Initialisation
    CheckErrors(InitRtc(), FDIR_ERROR_HANDLER);

    // Start ECC
    CheckErrors(InitEcc(), FDIR_ERROR_HANDLER);

    // Monitor Initialisation
    CheckErrors(InitMonitoring(), FDIR_ERROR_HANDLER);

    // File System Initialisation
    CheckErrors(InitFs(), FDIR_ERROR_HANDLER);

    // Create all tasks
    CheckErrors(CreateTasks(), FDIR_ERROR_HANDLER);

    // Create all buffers
    CheckErrors(CreateBuffers(), FDIR_ERROR_HANDLER);

    // Create all user mutexes
    CheckErrors(CreateMutexes(), FDIR_ERROR_HANDLER);

    // Initialise Console
    InitConsole();
    ConsolePrint("Init Done\n");

    // Print Welcome Message
    ConsolePrint("Welcome on ");
    ConsolePrint(g_program_name);
    ConsolePrint("\n");

    // Print System Information
    PrintSystemInfo();
}

/**
 *  @fn     InitCache(void)
 *  @brief  Function that initialises cache memories if it exists
 */
static void InitCache(void)
{
#if defined(CONFIG_CACHE)
    // Enable Instruction Cache
    SCB_EnableICache();

    // Enable Data Cache
    SCB_EnableDCache();
#endif
}

/**
 *  @fn     EnableFaultHandlers(void)
 *  @brief  Function that initialises fault handlers
 */
static void EnableFaultHandlers(void) 
{
    // Enables memory management, bus fault and usage fault exceptions
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk;
}