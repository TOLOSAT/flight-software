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
#include "drv/drv_wdg.h"
#include "system/cache.h"
#include "system/console.h"
#include "system/ecc.h"
#include "system/mpu.h"
#include "system/sysinfo.h"
#include "system/sysusage.h"
#include "system/sysleds.h"
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
    CheckError(InitHal());

    // BSP Late Initialisation
    CheckError(BSPLateInit());

    // System LEDs Initialisation
    CheckError(InitSysLEDs());

    // Peripherals Initialisation
    CheckError(InitPeripherals());

    // RTC Initialisation
    CheckError(InitRtc());

    // File System Initialisation
    CheckError(InitFs());

    // Start ECC
    CheckError(InitEcc());

    // Monitor Initialisation
    CheckError(InitMonitoring());

    // Create all tasks
    CheckError(CreateTasks());

    // Create all buffers
    CheckError(CreateBuffers());

    // Create all user mutexes
    CheckError(CreateMutexes());

    // Initialise Console
    InitConsole();
    LOG("Init Done\n");

    // Print System Information
    PrintSystemInfo();
}
