/**
 * @file    initialisation.c
 * @author  Merlin Kooshmanian
 * @brief   Source file initialising tools and HAL
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "core/initialisation.h"
#include "core/buffers.h"
#include "core/tasks.h"
#include "core/timers.h"
#include "drv/common.h"
#include "drv/peripherals.h"
#include "drv/memories.h"
#include "drv/others/drv_rtc.h"
#include "drv/others/drv_wdg.h"
#include "fdir/fdir.h"
#include "core/fs.h"
#include "system/cache.h"
#include "system/console.h"
#include "system/context.h"
#include "system/ecc.h"
#include "system/mpu.h"
#include "system/sysinfo.h"
#include "system/sysleds.h"
#include "system/sysmon.h"
#include "system/syswdg.h"
#include "utils/log.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      init(void)
 * @brief   Function that initialise tools and HAL
 */
void init(void)
{
    // FDIR Initialisation
    InitFDIR();

    // Cache Initialisation
    InitCache();

    // MPU Initialisation
    InitMPU();

    // HAL Initialisation
    InitHal();

    // BSP Late Initialisation
    BSPLateInit();

    // System LEDs Initialisation
    InitSysLEDs();

    // Peripherals Initialisation
    InitPeripherals();

    // Memory Initialisation
    InitMemories();

    // RTC Initialisation
    InitRtc();

    // File System Initialisation
    InitFs();

    // Start ECC
    InitEcc();

    // Initialise the context of the kernel
    InitContext();

    // Initialise Console
    InitConsole();

    // Print System Information
    PrintSystemInfo();

    // WARNING : interrupts will be disabled by the following
    // functions. They will be re-reactivated when the OS will
    // be started.

    // System Monitor Initialisation
    InitSYSMON();

    // System Watchdog Initialisation
    InitSYSWDG();

    // Create all tasks
    CreateTasks();

    // Create all buffers
    CreateBuffers();

    // Create all timers
    CreateTimers();

    // Create all user mutexes
    CreateMutexes();
    CreateFsMutexes();
    CreateConsoleMutexes();
    CreatePeripheralsMutexes();
}
