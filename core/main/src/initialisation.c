/**
 * @file initialisation.c
 * @author Merlin Kooshmanian
 * @brief Source file initialising tools and HAL
 * @date 21/01/2023
 *
 * Last Update : 31/05/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <cmsis_os2.h>

#include "initialisation.h"
#include "conf/io_conf.h"
#include "tolosat_hal.h"
#include "fdir.h"
#include "monitoring.h"
#include "buffers.h"
#include "tasks.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void InitConsole(uartInst_t *uart_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      init(void)
 * @brief   Function that initialise tools and HAL
 * @return  0
 *
 * Error management needs to be improved
 */
uint32_t init(void)
{
    // Variable Initialisation
    uint32_t status = 0;

    // HAL Initialisation
    status = InitHal();
    CheckErrors(status, ERROR_HANDLER);

    // GPIOs Initialisation
    status = GpioOpen(&led_inst, LED2_GPIO_PORT, LED2_PIN);
    CheckErrors(status, ERROR_HANDLER);
    status = GpioOpen(&user_button_inst, USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN);
    CheckErrors(status, ERROR_HANDLER);

    // UARTs Initialisation
    status = UartOpen(&uart_print_inst);
    CheckErrors(status, ERROR_HANDLER);
    status = UartOpen(&uart_tmtc_inst);
    CheckErrors(status, ERROR_HANDLER);
    status = UartOpen(&uart_pl_inst);
    CheckErrors(status, ERROR_HANDLER);

    // I2Cs Initialisation
    status = IicOpen(&iic_avionic_inst);
    CheckErrors(status, ERROR_HANDLER);

    // RTC Initialisation
    status = RtcInit();
    CheckErrors(status, ERROR_HANDLER);
    
    // Monitor Initialisation
    status = InitMonitoring();
    CheckErrors(status, ERROR_HANDLER);

    // Console Initialisation
    InitConsole(&uart_print_inst);

    // OS Kernel Initialisation
    status = osKernelInitialize();
    CheckErrors(status, ERROR_HANDLER);

    // Create all tasks
    status = createTasks();
    CheckErrors(status, ERROR_HANDLER);

    // Create all buffers
    status = createBuffers();
    CheckErrors(status, ERROR_HANDLER);

    return 0;
}