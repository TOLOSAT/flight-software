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

/***************************** Macros Definitions ****************************/

#define BUFFER_MSG_SIZE 2U /**< Size of exemple buffer message */
#define UART_MSG_SIZE 1U   /**< Size of exemple uart message */
#define I2C_MSG_SIZE 1U    /**< Size of exemple i2c message */
#define SLAVE_ADDR 0x05    /**< Exemple slave address */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

extern gpioInst_t led_inst;
extern FsInst_t fs_inst;

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

    // Mount SDCARD
    FRESULT fresult = f_mount(&fs_inst.file_system, "/", 1);
    if (fresult != FR_OK)
    {
        printf("ERROR!!! in mounting SD CARD...\n");
    }
    else
    {
        printf("SD CARD mounted successfully...\n");
    }

	// Open file to write/ create a file if it doesn't exist
	fresult = f_open(&fs_inst.buffer_file, "test.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

	// Writing text
	f_puts("Hello from TOLOSAT FS", &fs_inst.buffer_file);

	// Close file
	f_close(&fs_inst.buffer_file);

    // Unmount SDCARD
    fresult = f_mount(NULL, "/", 1);
	if (fresult == FR_OK) 
    {
        printf("SD CARD UNMOUNTED successfully...\n");
    }

    // Function Core
    while (1)
    {
        printf("Hello\n");
        GpioToggle(&led_inst);

        task_status = waitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}