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
#include "kernel.h"
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
void IN_KERNEL_TEXT_SECTION init(void)
{
    // Variable Initialisation
    uint32_t status = 0u;

    // First Enable Fault Handlers
    EnableFaultHandlers();

    // Cache Initialisation
    InitCache();

    // HAL Initialisation
    status = InitHal();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // BSP Late Initialisation
    status = BSPLateInit();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Peripherals Initialisation
    status = InitPeripherals();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // RTC Initialisation
    status = InitRtc();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Start ECC
    status = InitEcc();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Monitor Initialisation
    status = InitMonitoring();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // File System Initialisation
    status = InitFs();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Create all tasks
    status = CreateTasks();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Create all buffers
    status = CreateBuffers();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Create all user mutexes
    status = CreateMutexes();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Notify Console
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
static void IN_KERNEL_TEXT_SECTION InitCache(void)
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
static void IN_KERNEL_TEXT_SECTION EnableFaultHandlers(void) 
{
    // Enables memory management, bus fault and usage fault exceptions
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk;
}