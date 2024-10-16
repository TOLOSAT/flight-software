/**
 * @file    initialisation.c
 * @author  Merlin Kooshmanian
 * @brief   Source file initialising tools and HAL
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
#include "utils/log.h"
#include "utils/console.h"
#include "utils/ecc.h"
#include "utils/monitoring.h"
#include "utils/sysinfo.h"
#include "utils/sysled.h"
#include "utils/watchdog.h"

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
    CheckError(InitHal());

    // BSP Late Initialisation
    CheckError(BSPLateInit());

    // System LEDs Initialisation
    CheckError(InitSysLED());

    // Peripherals Initialisation
    CheckError(InitPeripherals());

    // RTC Initialisation
    CheckError(InitRtc());

    // Start ECC
    CheckError(InitEcc());

    // Monitor Initialisation
    CheckError(InitMonitoring());

    // File System Initialisation
    CheckError(InitFs());

    // Create all tasks
    CheckError(CreateTasks());

    // Create all buffers
    CheckError(CreateBuffers());

    // Create all user mutexes
    CheckError(CreateMutexes());

    // Initialise Watchdog
    CheckError(InitWatchdog());

    // Initialise Console
    InitConsole();
    LOG("Init Done\n");

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