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

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

extern void Error_Handler(void);
extern void InitMonitorHandler(uartInst_t *uart_inst);

/************************** Variable Definitions *****************************/

iicInst_t iic_avionic_inst = {
    .iic_ref = I2C1,
    .drive_type = IIC_IT_MASTER_DRIVE,
};
uartInst_t uart_print_inst = {
    .uart_ref = USART2,
    .drive_type = UART_POLLING_DRIVE,
    .baud_rate = 115200,
};
uartInst_t uart_cu_inst = {
    .uart_ref = USART6,
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
    // HAL Initialisation
    InitHal();

    // GPIOs Initialisation
    GpioOpen(&led_inst, LED2_GPIO_PORT, LED2_PIN);
    GpioOpen(&user_button_inst, USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN);

    // UARTs Initialisation
    UartOpen(&uart_print_inst);
    UartOpen(&uart_cu_inst);

    // I2Cs Initialisation
    IicOpen(&iic_avionic_inst);

    // Monitor Initialisation
    InitMonitorHandler(&uart_print_inst);

    // OS Kernel Initialisation
    osKernelInitialize();

    return (0);
}