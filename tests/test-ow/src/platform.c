/**
 * @file    platform.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking platform informations
 * @date    30/04/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "platform.h"
#include "core_basics.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void USER_BUTTON_IRQ_HANDLER(void);

/***************************** External Variables ****************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     uart_print_inst
 * @brief   UART print instance declaration
 */
uartInst_t IN_UART_DATA_SECTION uart_print_inst = {
    .uart_ref = UART_PRINT,
    .drive_type = UART_POLLING_DRIVE,
    .baudrate = 115200,
};

/**
 * @var     one_wire_inst
 * @brief   One Wire instance declaration
 */
owInst_t IN_OW_DATA_SECTION one_wire_inst = {
    .gpio_inst.port = ONEWIRE_GPIO_PORT,
    .gpio_inst.pin = ONEWIRE_PIN,
    .gpio_inst.mode = GPIO_MODE_OUTPUT_OD,
    .gpio_inst.pull = GPIO_NOPULL,
    .gpio_inst.speed = GPIO_SPEED_FREQ_MEDIUM,
};

/**
 * @var     led_inst
 * @brief   GPIO led instance declaration
 */
gpioInst_t IN_GPIO_DATA_SECTION led_inst = {
    .port = USER_LED_GPIO_PORT,
    .pin = USER_LED_PIN,
    .mode = GPIO_MODE_OUTPUT_PP,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
};

/**
 * @var     user_button_inst
 * @brief   GPIO button instance declaration
 */
gpioInst_t IN_GPIO_DATA_SECTION user_button_inst = {
    .port = USER_BUTTON_GPIO_PORT,
    .pin = USER_BUTTON_PIN,
    .mode = GPIO_MODE_IT_FALLING,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
};

/**
 * @var     sd_fs_inst
 * @brief   File System instance declaration
 */
fsInst_t IN_FS_DATA_SECTION sd_fs_inst = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn      PlatformInit(void)
 * @brief   Function that initialise the platform
 */
uint32_t IN_INIT_TEXT_SECTION PlatformInit(void)
{
    // Variable Initialisation
    uint32_t status = 0u;

    // GPIOs Initialisation
    status = GpioOpen(&led_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);
    status = GpioOpen(&user_button_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // UARTs Initialisation
    status = UartOpen(&uart_print_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // OneWire Initialisation
    status = OwOpen(&one_wire_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // File System Initialisation
    status = FsOpen(&sd_fs_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    return status;
}

/*************************** Interruption Handlers ***************************/

/**
 * @brief This function is the BUTTON interruption handler.
 */
void IN_GPIO_TEXT_SECTION USER_BUTTON_IRQ_HANDLER(void)
{
    // First clear interrupt flag
    if (__HAL_GPIO_EXTI_GET_IT(USER_BUTTON_PIN) != 0x00U)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(USER_BUTTON_PIN);
    }

    // Then do the interrupt routine
    /* Do something here */
}
