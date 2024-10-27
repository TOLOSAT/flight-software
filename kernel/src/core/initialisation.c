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
#include "system/console.h"
#include "system/ecc.h"
#include "system/sysinfo.h"
#include "system/sysusage.h"
#include "system/sysleds.h"
#include "utils/log.h"

/***************************** Macros Definitions ****************************/

#if defined(CONFIG_MPU)
#define DEFAULT_REGION_NO               0u                                  /**< Default region numero */
#define DEFAULT_REGION_BASE_ADDR        0x00000000u                         /**< Default region base address */
#define DEFAULT_REGION_SIZE             ARM_MPU_REGION_SIZE_4GB             /**< Default region size */

#define PERIPHERALS_REGION_NO           1u                                  /**< Peripheral region numero */
#define PERIPHERALS_REGION_BASE_ADDR    0x40000000u                         /**< Peripheral region base address */
#define PERIPHERALS_REGION_SIZE         ARM_MPU_REGION_SIZE_512MB           /**< Peripheral region size */

#define KERNEL_TEXT_REGION_NO           2u                                  /**< Kernel text region numero */
#define KERNEL_TEXT_REGION_BASE_ADDR    (uint32_t)&_kernel_text_start_      /**< Kernel text region base address */
#define KERNEL_TEXT_REGION_SIZE         ARM_MPU_REGION_SIZE_64KB            /**< Kernel text region size */

#define KERNEL_DATA_REGION_NO           3u                                  /**< Kernel data region numero */
#define KERNEL_DATA_REGION_BASE_ADDR    (uint32_t)&_kernel_data_start_      /**< Kernel data region base address */
#define KERNEL_DATA_REGION_SIZE         ARM_MPU_REGION_SIZE_128KB           /**< Kernel data region size */

#define KERNEL_RODATA_REGION_NO         4u                                  /**< Kernel rodata region numero */
#define KERNEL_RODATA_REGION_BASE_ADDR  (uint32_t)&_kernel_rodata_start_    /**< Kernel rodata region base address */
#define KERNEL_RODATA_REGION_SIZE       ARM_MPU_REGION_SIZE_8KB             /**< Kernel rodata region size */

#define DMABUFF_REGION_NO               5u                                  /**< DMA buffer region numero */
#define DMABUFF_REGION_BASE_ADDR        (uint32_t)&__dmabuff_start__        /**< DMA buffer region base address */
#define DMABUFF_REGION_SIZE             ARM_MPU_REGION_SIZE_32KB            /**< DMA buffer region size */
#endif

/*************************** Functions Declarations **************************/

static void InitCache(void);
static void InitMPU(void);
static void EnableFaultHandlers(void);

/*************************** Variables Definitions ***************************/

#if defined(CONFIG_MPU)
extern uint32_t _kernel_text_start_;
extern uint32_t _kernel_rodata_start_;
extern uint32_t _kernel_data_start_;
extern uint32_t __dmabuff_start__;
#endif

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

/**
 * @fn     InitCache(void)
 * @brief  Function that initialises cache memories if available
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
 * @fn     InitMPU(void)
 * @brief  Function that initialises Memory Protection Unit if available
 * 
 * The default cache and shareability strategies are :
 * - For normal memories : shareable and cacheable (write through, no write allocate). (i.e. TEX=000, C=1, B=0, S=1)
 * - For devices : shareable and not cacheable (i.e. TEX=000, C=0, B=1, S=1)
 */
static void InitMPU(void)
{
#if defined(CONFIG_MPU)
    // Variable initialisation
    uint32_t rbar = 0u;
    uint32_t rasr = 0u;

    // First disable MPU (in case it has been enabled before)
    ARM_MPU_Disable();

    // Set background region
    rbar = ARM_MPU_RBAR(DEFAULT_REGION_NO, DEFAULT_REGION_BASE_ADDR);
    rasr = ARM_MPU_RASR(
        0,                          // DisableExec: 0 (executable)
        ARM_MPU_AP_FULL,            // AccessPermission: full access (read/write for privileged and non-privileged)
        0,                          // TypeExtField: 0b000
        1,                          // IsShareable: 1 (shareable)
        1,                          // IsCacheable: 1 (cacheable)
        0,                          // IsBufferable: 0 (not bufferable)
        0,                          // SubRegionDisable: 0 (no sub-region disabled)
        DEFAULT_REGION_SIZE);       // Region size
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect peripherals
    rbar = ARM_MPU_RBAR(PERIPHERALS_REGION_NO, PERIPHERALS_REGION_BASE_ADDR);
    rasr = ARM_MPU_RASR(
        1,                          // DisableExec: 1 (not executable)
        ARM_MPU_AP_PRIV,            // AccessPermission: read/write access for privileged only
        0,                          // TypeExtField: 0b000
        1,                          // IsShareable: 1 (shareable)
        0,                          // IsCacheable: 0 (not cacheable)
        1,                          // IsBufferable: 1 (bufferable)
        0,                          // SubRegionDisable: 0 (no sub-region disabled)
        PERIPHERALS_REGION_SIZE);   // Region size
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect kernel text
    rbar = ARM_MPU_RBAR(KERNEL_TEXT_REGION_NO, KERNEL_TEXT_REGION_BASE_ADDR); // cppcheck-suppress misra-c2012-11.4; Is one of the exception of the rule because we need to address memory
    rasr = ARM_MPU_RASR(
        0,                          // DisableExec: 0 (executable)
        ARM_MPU_AP_PRO,             // AccessPermission: read-only access for privileged only
        0,                          // TypeExtField: 0b000
        1,                          // IsShareable: 1 (shareable)
        1,                          // IsCacheable: 1 (cacheable)
        0,                          // IsBufferable: 0 (not bufferable)
        0,                          // SubRegionDisable: 0 (no sub-region disabled)
        KERNEL_TEXT_REGION_SIZE);   // Region size
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect kernel data
    rbar = ARM_MPU_RBAR(KERNEL_DATA_REGION_NO, KERNEL_DATA_REGION_BASE_ADDR); // cppcheck-suppress misra-c2012-11.4; Is one of the exception of the rule because we need to address memory
    rasr = ARM_MPU_RASR(
        1,                          // DisableExec: 1 (not executable)
        ARM_MPU_AP_PRIV,            // AccessPermission: read-write access for privileged only
        0,                          // TypeExtField: 0b000
        1,                          // IsShareable: 1 (shareable)
        1,                          // IsCacheable: 1 (cacheable)
        0,                          // IsBufferable: 0 (not bufferable)
        0,                          // SubRegionDisable: 0 (no sub-region disabled)
        KERNEL_DATA_REGION_SIZE);   // Region size
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect kernel rodata
    rbar = ARM_MPU_RBAR(KERNEL_RODATA_REGION_NO, KERNEL_RODATA_REGION_BASE_ADDR); // cppcheck-suppress misra-c2012-11.4; Is one of the exception of the rule because we need to address memory
    rasr = ARM_MPU_RASR(
        1,                          // DisableExec: 1 (not executable)
        ARM_MPU_AP_PRO,             // AccessPermission: read-only access for privileged only
        0,                          // TypeExtField: 0b000
        1,                          // IsShareable: 1 (shareable)
        1,                          // IsCacheable: 1 (cacheable)
        0,                          // IsBufferable: 0 (not bufferable)
        0,                          // SubRegionDisable: 0 (no sub-region disabled)
        KERNEL_RODATA_REGION_SIZE); // Region size
    ARM_MPU_SetRegion(rbar, rasr);

    // Remove cacheability of DMABUFF section
    rbar = ARM_MPU_RBAR(DMABUFF_REGION_NO, DMABUFF_REGION_BASE_ADDR); // cppcheck-suppress misra-c2012-11.4; Is one of the exception of the rule because we need to address memory
    rasr = ARM_MPU_RASR(
        1,                          // DisableExec: 1 (not executable)
        ARM_MPU_AP_FULL,            // AccessPermission: full access (read/write for privileged and non-privileged)
        0,                          // TypeExtField: 0b000
        1,                          // IsShareable: 1 (shareable)
        0,                          // IsCacheable: 0 (not cacheable)
        1,                          // IsBufferable: 1 (bufferable)
        0,                          // SubRegionDisable: 0 (no sub-region disabled)
        DMABUFF_REGION_SIZE);       // Region size
    ARM_MPU_SetRegion(rbar, rasr);

    // Finally enable the MPU
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);
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