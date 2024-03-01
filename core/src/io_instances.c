/**
 * @file    io_instances.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking instance for IO's
 * @date    15/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "io_instances.h"

/*************************** Variables Definitions ***************************/

/**
 * @var     UART_TMTC_DMA_RX
 * @brief   DMA USART1 RX instance declaration
 */
DMA_HandleTypeDef UART_TMTC_DMA_RX = {0};

/**
 * @var     UART_TMTC_DMA_TX
 * @brief   DMA USART1 TX instance declaration
 */
DMA_HandleTypeDef UART_TMTC_DMA_TX = {0};

/**
 * @var     uart_tmtc_inst
 * @brief   UART tmtc instance declaration
 */
uartInst_t uart_tmtc_inst = {
    .uart_ref = UART_TMTC,
    .drive_type = UART_DMA_DRIVE,
    .baud_rate = 115200,
};

/**
 * @var     uart_print_inst
 * @brief   UART print instance declaration
 */
uartInst_t uart_print_inst = {
    .uart_ref = UART_PRINT,
    .drive_type = UART_POLLING_DRIVE,
    .baud_rate = 115200,
};

/**
 * @var     uart_pl_inst
 * @brief   UART payload instance declaration
 */
uartInst_t uart_pl_inst = {
    .uart_ref = UART_PL,
    .drive_type = UART_INTERRUPT_DRIVE,
    .baud_rate = 115200,
};

/**
 * @var     iic_avionic_inst
 * @brief   I2C avionic instance declaration
 */
iicInst_t iic_avionic_inst = {
    .iic_ref = I2C_AVIONIC,
    .drive_type = IIC_IT_MASTER_DRIVE,
};

/**
 * @var     spi_avionic_inst
 * @brief   SPI avionic instance declaration
 */
spiInst_t spi_avionic_inst = {
    .spi_ref = SPI_AVIONIC,
    .drive_type = SPI_POLLING_MASTER_DRIVE,
    .prescaler = SPI_BAUDRATEPRESCALER_8,
};

/**
 * @var     one_wire_inst
 * @brief   One Wire instance declaration
 */
owInst_t one_wire_inst = {
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
gpioInst_t led_inst = {
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
gpioInst_t user_button_inst = {
    .port = USER_BUTTON_GPIO_PORT,
    .pin = USER_BUTTON_PIN,
    .mode = GPIO_MODE_IT_FALLING,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
};

/**
 * @var     sd_card_gpio
 * @brief   GPIO for sd card (cs or card detect depend of the context) instance declaration
 */
gpioInst_t sd_card_gpio = {
    .port = SD_GPIO_PORT,
    .pin = SD_GPIO_PIN,
    .mode = GPIO_MODE_OUTPUT_PP,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
};

/**
 * @var     sd_fs_inst
 * @brief   File System instance declaration
 */
fsInst_t sd_fs_inst = {0};
