/**
 * @file    io_instances.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking instance for IO's
 * @date    15/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "io_instances.h"

/*************************** Variables Definitions ***************************/

/**
 * @var     TAPAS_UART_TMTC_DMA_RX
 * @brief   DMA USART1 RX instance declaration
 */
DMA_HandleTypeDef TAPAS_UART_TMTC_DMA_RX = {0};

/**
 * @var     TAPAS_UART_TMTC_DMA_TX
 * @brief   DMA USART1 TX instance declaration
 */
DMA_HandleTypeDef TAPAS_UART_TMTC_DMA_TX = {0};

/**
 * @var     iic_avionic_inst
 * @brief   I2C avionic instance declaration
 */
iicInst_t iic_avionic_inst = {
    .iic_ref = TAPAS_I2C_AVIONIC,
    .drive_type = IIC_IT_MASTER_DRIVE,
};

/**
 * @var     uart_tmtc_inst
 * @brief   UART tmtc instance declaration
 */
uartInst_t uart_tmtc_inst = {
    .uart_ref = TAPAS_UART_TMTC,
    .drive_type = UART_DMA_DRIVE,
    .baud_rate = 115200,
};

/**
 * @var     uart_print_inst
 * @brief   UART print instance declaration
 */
uartInst_t uart_print_inst = {
    .uart_ref = TAPAS_UART_PRINT,
    .drive_type = UART_POLLING_DRIVE,
    .baud_rate = 115200,
};

/**
 * @var     uart_pl_inst
 * @brief   UART payload instance declaration
 */
uartInst_t uart_pl_inst = {
    .uart_ref = TAPAS_UART_PL,
    .drive_type = UART_INTERRUPT_DRIVE,
    .baud_rate = 115200,
};

/**
 * @var     spi_sdcard_inst
 * @brief   SPI SD card instance declaration
 */
spiInst_t spi_sdcard_inst = {
    .spi_ref = TAPAS_SPI_SDCARD,
    .drive_type = SPI_POLLING_MASTER_DRIVE,
    .read_type = SPI_READ_TX_RX,
    .prescaler = SPI_BAUDRATEPRESCALER_8,
};

/**
 * @var     led_inst
 * @brief   GPIO led instance declaration
 */
gpioInst_t led_inst = {
    .mode = GPIO_MODE_OUTPUT_PP,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
};

/**
 * @var     user_button_inst
 * @brief   GPIO button instance declaration
 */
gpioInst_t user_button_inst = {
    .mode = GPIO_MODE_IT_FALLING,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
};

/**
 * @var     sd_card_cs
 * @brief   GPIO sd card chip select instance declaration
 */
gpioInst_t sd_card_cs = {
    .mode = GPIO_MODE_OUTPUT_PP,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
};

/**
 * @var     fs_inst
 * @brief   File System instance declaration
 */
FsInst_t fs_inst = {0};
