/**
 * @file init.c
 * @author Merlin Kooshmanian
 * @brief Source file initialising tools and HAL
 * @date 21/01/2023
 *
 * Last Update : 21/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <cmsis_os2.h>

#if defined(STM32F411xE)
#include "stm32f4xx_nucleo_bsp.h"
#endif
#if defined(STM32F103xB)
#include "stm32f1xx_nucleo_bsp.h"
#endif
#include "tolosat_hal.h"
#include "errors_mgmt.h"
#include "buffers.h"
#include "tasks.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

extern void InitMonitorHandler(uartInst_t *uart_inst);

/************************** Variable Definitions *****************************/

DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

iicInst_t iic_avionic_inst = {
    .iic_ref = TOLOSAT_I2C_AVIONIC,
    .drive_type = IIC_IT_MASTER_DRIVE,
};
uartInst_t uart_tmtc_inst = {
    .uart_ref = TOLOSAT_UART_TMTC,
    .drive_type = UART_DMA_DRIVE,
    .baud_rate = 115200,
};
uartInst_t uart_print_inst = {
    .uart_ref = TOLOSAT_UART_PRINT,
    .drive_type = UART_POLLING_DRIVE,
    .baud_rate = 115200,
};
uartInst_t uart_cu_inst = {
    .uart_ref = TOLOSAT_UART_CU,
    .drive_type = UART_INTERRUPT_DRIVE,
    .baud_rate = 115200,
};
gpioInst_t led_inst = {
    .mode = GPIO_MODE_OUTPUT_PP,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
};
gpioInst_t user_button_inst = {
    .mode = GPIO_MODE_IT_FALLING,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
};

/************************* Functions Definitions *****************************/

/**
 * @fn      init(void)
 * @brief   Init tools and HAL
 * @param   void
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
    status = UartOpen(&uart_cu_inst);
    CheckErrors(status, ERROR_HANDLER);

    // I2Cs Initialisation
    status = IicOpen(&iic_avionic_inst);
    CheckErrors(status, ERROR_HANDLER);

    // Monitor Initialisation
    InitMonitorHandler(&uart_print_inst);

    // OS Kernel Initialisation
    osKernelInitialize();

    // Create all tasks
    status = createTasks();
    CheckErrors(status, ERROR_HANDLER);

    // Create all buffers
    status = createBuffers();
    CheckErrors(status, ERROR_HANDLER);

    return (0);
}