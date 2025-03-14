/**
 * @file    initialisation.c
 * @author  Merlin Kooshmanian
 * @brief   Source file initialising tools and HAL
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core/initialisation.h"
#include "core/buffers.h"
#include "core/tasks.h"
#include "drv/common.h"
#include "drv/peripherals.h"
#include "drv/others/drv_rtc.h"
#include "drv/others/drv_wdg.h"
#include "fdir/fdir.h"
#include "fs/fs.h"
#include "system/cache.h"
#include "system/console.h"
#include "system/context.h"
#include "system/ecc.h"
#include "system/mpu.h"
#include "system/sysinfo.h"
#include "system/sysleds.h"
#include "system/sysmon.h"
#include "utils/log.h"

#include "drv/memory/memdrv_qspi.h"

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

    // RTC Initialisation
    InitRtc();

    // File System Initialisation
    InitFs();

    // Start ECC
    InitEcc();

    // Monitor Initialisation
    InitMonitoring();

    // Create all tasks
    CreateTasks();

    // Create all buffers
    CreateBuffers();

    // Create all user mutexes
    CreateMutexes();

    // Initialise Console
    InitConsole();
    LOG("Init Done\n");

    // Print System Information
    PrintSystemInfo();

    context_t context = { 0 };
    context.state = SOFTWARE_STATE_NOMINAL;
    context.boot = 12;
    context.failedBoot = 4;
    context.version.major = 1;
    context.version.minor = 0;
    context.version.patch = 0;

    context_t context2 = { 0 };

    // WriteContext(context);

    // ReadContext(&context2);

    // Initialise the context of the kernel
    // InitContext();

    returnCode_t return_value = RET_SUCCESSFUL;

    return_value = WriteContext(context);

    return_value = ReadContext(&context2);

    (void) context;
    (void) return_value;
}
