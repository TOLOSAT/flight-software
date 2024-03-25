/**
 * @file    boot_init.c
 * @author  Merlin Kooshmanian
 * @brief   Source file initialising tools and HAL used by boot
 * @date    25/03/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "boot_init.h"
#include "diskio.h"
#include "boot_fdir.h"
#include "tolosat_fs.h"
#include "generic_hal.h"
#include "io_instances.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      init_boot(void)
 * @brief   Function that initialise tools and HAL for boot
 */
void init_boot(void)
{
    // Variable Initialisation
    uint32_t status = 0u;
    static FATFS file_system = {0};
    static Diskio_drvTypeDef driver = {0};
    char disk_path[4] = {0};
    driver.disk_initialize = DiskInitialize;
    driver.disk_status = DiskStatus;
    driver.disk_read = DiskRead;
    driver.disk_write = DiskWrite;
    driver.disk_ioctl = DiskIoctl;

    // HAL Initialisation
    status = InitHal();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // GPIOs Initialisation
    status = GpioOpen(&led_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // SPIs Initialisation
    status = SpiOpen(&spi_avionic_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    status = FATFS_LinkDriver(&driver, disk_path);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    status = f_mount(&file_system, "/", 1);
    CheckErrors(status, FDIR_ERROR_HANDLER);
}