/**
 ******************************************************************************
 * @file           : usbd_storage_if.c
 * @version        : v1.0_Cube
 * @brief          : Memory management layer.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/******************************* Include Files *******************************/

#include "usb-otg/usbd_storage_if.h"
#include "stm32h7xx_hal.h"

/***************************** Macros Definitions ****************************/

#define STORAGE_LUN_NBR 1
#define STORAGE_BLK_NBR 0x10000
#define STORAGE_BLK_SIZ 0x200

/*************************** Functions Declarations **************************/

static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);

/*************************** Variables Definitions ***************************/

extern SD_HandleTypeDef sd_card_inst;

/** USB Mass storage Standard Inquiry Data. */
const int8_t STORAGE_Inquirydata_FS[] = {
    // clang-format off
    0x00,                                   //
    0x80,                                   //
    0x02,                                   //
    0x02,                                   //
    (STANDARD_INQUIRY_DATA_LEN - 5),        //
    0x00,                                   //
    0x00,                                   //
    0x00,                                   //
    'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', // Manufacturer : 8 bytes
    'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', // Product : 16 Bytes
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', //
    '0', '.', '0', '1'                      // Version : 4 Bytes
    // clang-format on
};

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS = {
    STORAGE_Init_FS, STORAGE_GetCapacity_FS, STORAGE_IsReady_FS,   STORAGE_IsWriteProtected_FS,
    STORAGE_Read_FS, STORAGE_Write_FS,       STORAGE_GetMaxLun_FS, (int8_t *)STORAGE_Inquirydata_FS
};

/*************************** Functions Definitions ***************************/

/**
 * @brief  Initializes the storage unit (medium) over USB FS IP
 * @param  lun: Logical unit number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_Init_FS(uint8_t lun)
{
    UNUSED(lun);

    return (USBD_OK);
}

/**
 * @brief  Returns the medium capacity.
 * @param  lun: Logical unit number.
 * @param  block_num: Number of total block number.
 * @param  block_size: Block size.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
    UNUSED(lun);

    HAL_SD_CardInfoTypeDef sdinfo;

    if (HAL_SD_GetCardInfo(&sd_card_inst, &sdinfo) != HAL_OK)
    {
        return (USBD_FAIL);
    }

    *block_num  = sdinfo.LogBlockNbr - 1;
    *block_size = sdinfo.LogBlockSize;
    return (USBD_OK);
}

/**
 * @brief   Checks whether the medium is ready.
 * @param  lun:  Logical unit number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_IsReady_FS(uint8_t lun)
{
    UNUSED(lun);

    return (USBD_OK);
}

/**
 * @brief  Checks whether the medium is write protected.
 * @param  lun: Logical unit number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_IsWriteProtected_FS(uint8_t lun)
{
    UNUSED(lun);

    return (USBD_OK);
}

/**
 * @brief  Reads data from the medium.
 * @param  lun: Logical unit number.
 * @param  buf: data buffer.
 * @param  blk_addr: Logical block address.
 * @param  blk_len: Blocks number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    UNUSED(lun);

    if (HAL_SD_ReadBlocks(&sd_card_inst, buf, blk_addr, blk_len, HAL_MAX_DELAY) != HAL_OK)
    {
        return (USBD_FAIL);
    }

    while (HAL_SD_GetCardState(&sd_card_inst) != HAL_SD_CARD_TRANSFER)
    {
    }

    return (USBD_OK);
}

/**
 * @brief  Writes data into the medium.
 * @param  lun: Logical unit number.
 * @param  buf: data buffer.
 * @param  blk_addr: Logical block address.
 * @param  blk_len: Blocks number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    UNUSED(lun);

    if (HAL_SD_WriteBlocks(&sd_card_inst, buf, blk_addr, blk_len, HAL_MAX_DELAY) != HAL_OK)
    {
        return (USBD_FAIL);
    }

    while (HAL_SD_GetCardState(&sd_card_inst) != HAL_SD_CARD_TRANSFER)
    {
    }

    return (USBD_OK);
}

/**
 * @brief  Returns the Max Supported LUNs.
 * @param  None
 * @retval Lun(s) number.
 */
int8_t STORAGE_GetMaxLun_FS(void)
{
    return (STORAGE_LUN_NBR - 1);
}
