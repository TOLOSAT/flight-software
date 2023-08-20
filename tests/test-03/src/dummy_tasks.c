/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 * @date    26/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <stdio.h> // cppcheck-suppress misra-c2012-21.6
#include <cmsis_os2.h>

#include "dummy_tasks.h"
#include "tasks.h"
#include "fdir.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"
#include "conf/io_conf.h"

/***************************** Macros Definitions ****************************/

#define BUFFER_MSG_SIZE 2U /**< Size of exemple buffer message */
#define UART_MSG_SIZE 1U   /**< Size of exemple uart message */
#define I2C_MSG_SIZE 1U    /**< Size of exemple i2c message */
#define SLAVE_ADDR 0x05    /**< Exemple slave address */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static FIL dummy_file;

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask(void *task_dyn_conf)
 * @brief   Function that runs the dummy main task.
 * @param   task_dyn_conf Status of the current task
 */
void DummyMainTask(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    printf("[#0] Init\n");
    task_status = initPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Test SD card
    printf("[#0] Start SD card test\n");

    // Open file to write/ create a file if it doesn't exist
    FRESULT fresult = f_open(&dummy_file, "test.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

    // Test file opening
    if (fresult == FR_OK)
    {
        // Writing text
        (void)f_puts("Hello from TOLOSAT FS", &dummy_file);

        // Close file
        (void)f_close(&dummy_file);

        // Unmount SDCARD
        (void)f_mount(NULL, "/", 1);

        // End of SD card test
        printf("[#0] SD card test successful (+ disk was unmounted)\n");
    }
    else
    {
        // Error message
        printf("[#0] SD card test failed\n");
    }

    // Function Core
    while (1)
    {
        printf("[#0] Hello\n");
        GpioToggle(&led_inst);

        task_status = waitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}