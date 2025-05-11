/**
 * @file    memdrv_sd.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for SD card using SDMMC driver
 *
 * @copyright Copyright (c) TOLOSAT 2025
 * Adapted from STMicroelectronic example
 */

/******************************* Include Files *******************************/

#include "memory/memdrv_sd.h"

/***************************** Macros Definitions ****************************/

#define SD_TIMEOUT            30000u     /**< SD Card Timeout for ST HAL */
#define SD_DEFAULT_BLOCK_SIZE 512u       /**< Size of a block in the SD Card */
#define SD_NOT_PRESENT        0x00u      /**< Indicates that no SD card is present */
#define SD_PRESENT            0x01u      /**< Indicates that an SD card is present*/
#define SD_DETECT_PIN         GPIO_PIN_5 /**< GPIO detect pin for SD card */
#define SD_DETECT_PORT        GPIOD      /**< GPIO detect port for SD card */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     sd_card_inst
 * @brief   SD card instance for SDMMC
 */
static SD_HandleTypeDef sd_card_inst = { 0 };

/*************************** Functions Definitions ***************************/

/**
 * @fn          SD_Init(uint8_t disk)
 * @brief       Function that initialises an SD card with SDMMC
 * @param[in]   disk Disk that will be initialised
 * @retval      #RET_INVALID_PARAM if disk does not exist
 * @retval      #RET_ERROR if initialisation failed
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SD_Init(uint8_t disk)
{
    returnCode_t return_value             = RET_SUCCESSFUL;
    sd_card_inst.Instance                 = SDMMC1;
    sd_card_inst.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
    sd_card_inst.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    sd_card_inst.Init.BusWide             = SDMMC_BUS_WIDE_4B;
    sd_card_inst.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
    sd_card_inst.Init.ClockDiv            = 8u;

    // Check parameter(s)
    if (disk == DISK0_REF)
    {
        /* HAL SD initialization */
        HAL_StatusTypeDef test_hal = HAL_SD_Init(&sd_card_inst);
        /* Configure SD Bus width (4 bits mode selected) */
        if (test_hal != HAL_OK)
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SD_ReadBlocks(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that reads SD card blocks using SDMMC
 * @param[in]   disk Disk that is read
 * @param[out]  data Pointer to the data that will be read
 * @param[in]   addr Address of the data that will be read
 * @param[in]   len  Number of block that will be read
 * @retval      #RET_INVALID_PARAM if disk does not exist, len equal zero, pointer is null
 * @retval      #RET_ERROR if an error occured
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SD_ReadBlocks(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (disk == DISK0_REF)
    {
        uint32_t tickstart         = HAL_GetTick();
        HAL_StatusTypeDef test_hal = HAL_SD_ReadBlocks(&sd_card_inst, data, addr, len, SD_TIMEOUT);
        if (test_hal == HAL_OK)
        {
            HAL_SD_CardStateTypeDef sd_state = HAL_SD_GetCardState(&sd_card_inst);
            while ((sd_state == HAL_SD_CARD_PROGRAMMING) && ((HAL_GetTick() - tickstart) < SD_TIMEOUT))
            {
                sd_state = HAL_SD_GetCardState(&sd_card_inst);
            }

            // Write procedure is finished when state is HAL_SD_CARD_TRANSFER
            if (sd_state != HAL_SD_CARD_TRANSFER)
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SD_WriteBlocks(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that writes SD card blocks using SDMMC
 * @param[in]   disk Disk that is written
 * @param[in]   data Pointer to the data that will be written
 * @param[in]   addr Address of the data that will be written
 * @param[in]   len  Number of block that will be written
 * @retval      #RET_INVALID_PARAM if disk does not exist, len equal zero, pointer is null
 * @retval      #RET_ERROR if an error occured or write is not permitted
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SD_WriteBlocks(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (disk == DISK0_REF)
    {
        uint32_t tickstart         = HAL_GetTick();
        HAL_StatusTypeDef test_hal = HAL_SD_WriteBlocks(&sd_card_inst, (uint8_t *)data, addr, len, SD_TIMEOUT); // cppcheck-suppress misra-c2012-11.8;
                                                                                                                // Low-level drivers don't use the
                                                                                                                // const argument so it has to
                                                                                                                // disappear somewhere
        if (test_hal == HAL_OK)
        {
            HAL_SD_CardStateTypeDef sd_state = HAL_SD_GetCardState(&sd_card_inst);
            while ((sd_state == HAL_SD_CARD_PROGRAMMING) && ((HAL_GetTick() - tickstart) < SD_TIMEOUT))
            {
                sd_state = HAL_SD_GetCardState(&sd_card_inst);
            }

            // Write procedure is finished when state is HAL_SD_CARD_TRANSFER
            if (sd_state != HAL_SD_CARD_TRANSFER)
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SD_Ioctl(uint8_t disk, uint8_t cmd, void *data)
 * @brief           Function that perfoms io control on the SD card (get info, change parameters ...)
 * @param[in]       disk Disk on which we perform the io control
 * @param[in]       cmd Which can of action is done on the SD card
 * @param[in,out]   data Data shared depending of command
 * @retval          #RET_INVALID_PARAM if the io control is not available for this device
 * @retval          #RET_ERROR if an error occured
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t SD_Ioctl(uint8_t disk, uint8_t cmd, void *data)
{
    returnCode_t return_value = RET_ERROR;

    // Check parameter(s)
    if (disk == DISK0_REF)
    {
        HAL_SD_CardInfoTypeDef card_info;
        HAL_SD_CardStateTypeDef card_state = HAL_SD_GetCardState(&sd_card_inst);
        if (card_state == HAL_SD_CARD_TRANSFER)
        {
            switch (cmd)
            {
                /* Make sure that no pending write process */
                case CTRL_SYNC :
                    return_value = RET_SUCCESSFUL;
                    break;

                /* Get number of sectors on the disk (DWORD) */
                case GET_SECTOR_COUNT :
                    HAL_SD_GetCardInfo(&sd_card_inst, &card_info);
                    *(DWORD *)data = card_info.LogBlockNbr;
                    return_value   = RET_SUCCESSFUL;
                    break;

                /* Get R/W sector size (WORD) */
                case GET_SECTOR_SIZE :
                    HAL_SD_GetCardInfo(&sd_card_inst, &card_info);
                    *(WORD *)data = card_info.LogBlockSize;
                    return_value  = RET_SUCCESSFUL;
                    break;

                /* Get erase block size in unit of sector (DWORD) */
                case GET_BLOCK_SIZE :
                    HAL_SD_GetCardInfo(&sd_card_inst, &card_info);
                    *(DWORD *)data = card_info.LogBlockSize / SD_DEFAULT_BLOCK_SIZE;
                    return_value   = RET_SUCCESSFUL;
                    break;

                default :
                    return_value = RET_INVALID_PARAM;
                    break;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SD_GetStatus(uint8_t disk)
 * @brief       Function that gets status of the SD card
 * @param[in]   disk on from which we get the status
 * @return      #RET_SUCCESSFUL if the disk is available and initialised
 * @return      #RET_ERROR else
 */
returnCode_t SD_GetStatus(uint8_t disk)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (disk == DISK0_REF)
    {
        HAL_SD_CardStateTypeDef card_state = HAL_SD_GetCardState(&sd_card_inst);
        if (card_state != HAL_SD_CARD_TRANSFER)
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_ERROR;
    }

    return return_value;
}
