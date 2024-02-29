/**
 * @file    sdmmc_driver.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for SD card using SDMMC driver
 * @date    29/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 * Adapted from STMicroelectronic example
 */

/******************************* Include Files *******************************/

#include "sdmmc_driver.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

SD_HandleTypeDef sd_card_inst;

/*************************** Functions Definitions ***************************/

/**
 * @brief  Initializes the SD card device.
 * @retval SD status
 */
uint8_t SD_Init(void)
{
    // Variables Initialisation
    uint8_t sd_state = MSD_OK;
    sd_card_inst.Instance = SDMMC1;
    sd_card_inst.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
    sd_card_inst.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    sd_card_inst.Init.BusWide = SDMMC_BUS_WIDE_4B;
    sd_card_inst.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
    sd_card_inst.Init.ClockDiv = 0;

    /* Check if the SD card is plugged in the slot */
    if (SD_IsDetected() != SD_PRESENT)
    {
        return MSD_ERROR_SD_NOT_PRESENT;
    }
    /* HAL SD initialization */
    sd_state = HAL_SD_Init(&sd_card_inst);
    /* Configure SD Bus width (4 bits mode selected) */
    if (sd_state == MSD_OK)
    {
        /* Enable wide operation */
        if (HAL_SD_ConfigWideBusOperation(&sd_card_inst, SDMMC_BUS_WIDE_4B) != HAL_OK)
        {
            sd_state = MSD_ERROR;
        }
    }

    return sd_state;
}

/**
 * @brief  Configures Interrupt mode for SD detection pin.
 * @retval Returns 0
 */
uint8_t SD_ITConfig(void)
{

    return (uint8_t)0;
}

/**
 * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  ReadAddr: Address from where data is to be read
 * @param  NumOfBlocks: Number of SD blocks to read
 * @param  Timeout: Timeout for read operation
 * @retval SD status
 */
uint8_t SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
    uint8_t sd_state = MSD_OK;

    if (HAL_SD_ReadBlocks(&sd_card_inst, (uint8_t *)pData, ReadAddr, NumOfBlocks, Timeout) != HAL_OK)
    {
        sd_state = MSD_ERROR;
    }

    return sd_state;
}

/**
 * @brief  Writes block(s) to a specified address in an SD card, in polling mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  WriteAddr: Address from where data is to be written
 * @param  NumOfBlocks: Number of SD blocks to write
 * @param  Timeout: Timeout for write operation
 * @retval SD status
 */
uint8_t SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
    uint8_t sd_state = MSD_OK;

    if (HAL_SD_WriteBlocks(&sd_card_inst, (uint8_t *)pData, WriteAddr, NumOfBlocks, Timeout) != HAL_OK)
    {
        sd_state = MSD_ERROR;
    }

    return sd_state;
}

/**
 * @brief  Erases the specified memory area of the given SD card.
 * @param  StartAddr: Start byte address
 * @param  EndAddr: End byte address
 * @retval SD status
 */
uint8_t SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
    uint8_t sd_state = MSD_OK;

    if (HAL_SD_Erase(&sd_card_inst, StartAddr, EndAddr) != HAL_OK)
    {
        sd_state = MSD_ERROR;
    }

    return sd_state;
}

/**
 * @brief  Gets the current SD card data status.
 * @param  None
 * @retval Data transfer state.
 *          This value can be one of the following values:
 *            @arg  SD_TRANSFER_OK: No data transfer is acting
 *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
 */
uint8_t SD_GetCardState(void)
{
    return ((HAL_SD_GetCardState(&sd_card_inst) == HAL_SD_CARD_TRANSFER) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}

/**
 * @brief  Get SD information about specific SD card.
 * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
 * @retval None
 */
void SD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo)
{
    /* Get SD card Information */
    HAL_SD_GetCardInfo(&sd_card_inst, CardInfo);
}

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @param  None
 * @retval Returns if SD is detected or not
 */
uint8_t SD_IsDetected(void)
{
    volatile uint8_t status = SD_PRESENT;
    /* Check SD card detect pin */
    if(HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) != GPIO_PIN_RESET)
    {
        status = SD_NOT_PRESENT;
    }
    return status;
}