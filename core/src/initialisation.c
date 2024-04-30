/**
 * @file    initialisation.c
 * @author  Merlin Kooshmanian
 * @brief   Source file initialising tools and HAL
 * @date    21/01/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "initialisation.h"
#include "core_basics.h"
#include "monitoring.h"
#include "tolosat_fs.h"
#include "generic_hal.h"
#include "platform.h"

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
    // Variable Initialisation
    uint32_t status = 0u;

    // First Enable Fault Handlers
    EnableFaultHandlers();

    // Cache Initialisation
    InitCache();

    // HAL Initialisation
    status = InitHal();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // GPIOs Initialisation
    status = GpioOpen(&led_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);
    status = GpioOpen(&user_button_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);
    status = GpioOpen(&sd_card_gpio);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // UARTs Initialisation
    status = UartOpen(&uart_print_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);
    status = UartOpen(&uart_tmtc_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);
    status = UartOpen(&uart_pl_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // I2Cs Initialisation
    status = IicOpen(&iic_avionic_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // SPIs Initialisation
    status = SpiOpen(&spi_avionic_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // OneWire Initialisation
    status = OwOpen(&one_wire_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // RTC Initialisation
    status = RtcInit();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // File System Initialisation
    status = FsOpen(&sd_fs_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Monitor Initialisation
    status = InitMonitoring();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Start ECC
    status = EccInit();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Create all tasks
    status = CreateTasks();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Create all buffers
    status = CreateBuffers();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Create all mutexes
    status = CreateMutexes();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Notify Console
    ConsolePrint("Init Succeed\n");
}

/**
 *  @fn     InitCache(void)
 *  @brief  Function that initialises cache memories if it exists
 */
static void InitCache(void)
{
#if defined(CACHE_AVAILABLE)
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