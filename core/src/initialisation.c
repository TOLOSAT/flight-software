/**
 * @file    initialisation.c
 * @author  Merlin Kooshmanian
 * @brief   Source file initialising tools and HAL
 * @date    21/01/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "initialisation.h"
#include "io_instances.h"
#include "tolosat_hal.h"
#include "tolosat_fs.h"
#include "fdir.h"
#include "monitoring.h"
#include "buffers.h"
#include "tasks.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void InitConsole(uartInst_t *uart_inst);
void InitCache(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      init(void)
 * @brief   Function that initialise tools and HAL
 */
void init(void)
{
    // Variable Initialisation
    uint32_t status = 0;

    // Cache Initialisation
    InitCache();

    // HAL Initialisation
    status = InitHal();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // GPIOs Initialisation
    status = GpioOpen(&led_inst, USER_LED_GPIO_PORT, USER_LED_PIN);
    CheckErrors(status, FDIR_ERROR_HANDLER);
    status = GpioOpen(&user_button_inst, USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN);
    CheckErrors(status, FDIR_ERROR_HANDLER);
    status = GpioOpen(&sd_card_cs, TAPAS_SPI_SDCARD_CS_GPIO_PORT, TAPAS_SPI_SDCARD_CS_PIN);
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
    status = SpiOpen(&spi_sdcard_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // RTC Initialisation
    status = RtcInit();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // File System Initialisation
    status = FsOpen(&fs_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Monitor Initialisation
    status = InitMonitoring();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Console Initialisation
    InitConsole(&uart_print_inst);

    // Create all tasks
    status = createTasks();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Create all buffers
    status = createBuffers();
    CheckErrors(status, FDIR_ERROR_HANDLER);
}

/**
 *  @fn     InitCache(void)
 *  @brief  Function that initialises cache memories if it exists
 */
void InitCache(void)
{
#if defined(CACHE_AVAILABLE)
    // Enable Instruction Cache
    SCB_EnableICache();

    // Enable Data Cache
    SCB_EnableDCache();
#endif
}