/**
 * @file    sdmmc_driver.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SD card using SDMMC driver
 * @date    29/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 * Adapted from STMicroelectronic example
 */

#ifndef SDMMC_DRIVER_H
#define SDMMC_DRIVER_H

/******************************* Include Files *******************************/

#include "stm32h7xx_hal.h"

/***************************** Macros Definitions ****************************/

#define SD_CardInfo             HAL_SD_CardInfoTypeDef
#define MSD_OK                      0x00u
#define MSD_ERROR                   0x01u
#define MSD_ERROR_SD_NOT_PRESENT    0x02u
#define SD_TRANSFER_OK              0x00u
#define SD_TRANSFER_BUSY            0x01u
#define SD_PRESENT                  0x01u
#define SD_NOT_PRESENT              0x00u
#define SD_DATATIMEOUT              100000000u
#define SD_DetectIRQHandler()       HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8)
#define SD_DETECT_PIN               GPIO_PIN_5
#define SD_DETECT_GPIO_PORT         GPIOD

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

uint8_t SD_Init(void);
uint8_t SD_ITConfig(void);
uint8_t SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
uint8_t SD_GetCardState(void);
void SD_GetCardInfo(SD_CardInfo *CardInfo);
uint8_t SD_IsDetected(void);

#endif /* SDMMC_DRIVER_H */