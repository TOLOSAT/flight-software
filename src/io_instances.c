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
 * @var     spi_avionic_inst
 * @brief   SPI avionic instance declaration
 */
spiInst_t spi_avionic_inst = {
    .spi_ref = SPI_AVIONIC,
    .drive_type = SPI_POLLING_MASTER_DRIVE,
    .prescaler = SPI_BAUDRATEPRESCALER_8,
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
