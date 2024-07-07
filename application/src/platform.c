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

/*************************** Variables Definitions ***************************/

/**
 * @var     uart_tmtc_inst
 * @brief   UART tmtc instance declaration
 */
uartInst_t IN_UART_DATA_SECTION uart_tmtc_inst = {
    .uart_ref = UART_TMTC,
    .dma_rx_ref = UART_TMTC_DMA_RX,
    .dma_tx_ref = UART_TMTC_DMA_TX,
    .drive_type = UART_DMA_DRIVE,
    .baudrate = 115200,
    .irq_no = UART_TMTC_IRQ_NO,
    .dma_rx_irq_no = UART_TMTC_DMA_RX_IRQ_NO,
    .dma_tx_irq_no = UART_TMTC_DMA_TX_IRQ_NO,
};

/**
 * @var     uart_print_inst
 * @brief   UART print instance declaration
 */
uartInst_t IN_UART_DATA_SECTION uart_print_inst = {
    .uart_ref = UART_PRINT,
    .drive_type = UART_POLLING_DRIVE,
    .baudrate = 115200,
    .irq_no = UART_PRINT_IRQ_NO,
};

/**
 * @var     uart_pl_inst
 * @brief   UART payload instance declaration
 */
uartInst_t IN_UART_DATA_SECTION uart_pl_inst = {
    .uart_ref = UART_PL,
    .drive_type = UART_INTERRUPT_DRIVE,
    .baudrate = 115200,
    .irq_no = UART_PL_IRQ_NO,
};

/**
 * @var     i2c_avionic_inst
 * @brief   I2C avionic instance declaration
 */
i2cInst_t IN_I2C_DATA_SECTION i2c_avionic_inst = {
    .i2c_ref = I2C_AVIONIC,
    .drive_type = I2C_IT_MASTER_DRIVE,
    .irq_no = I2C_AVIONIC_IRQ_NO,
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
    .irq_no = IRQ_NONE,
    .callback = NULL,
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
    .irq_no = USER_BUTTON_EXTI_IRQ_NO,
    .callback = NULL,
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
    status = UartOpen(&uart_tmtc_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);
    status = UartOpen(&uart_pl_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // I2Cs Initialisation
    status = I2cOpen(&i2c_avionic_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // OneWire Initialisation
    status = OwOpen(&one_wire_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // File System Initialisation
    status = FsOpen(&sd_fs_inst);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    return status;
}
