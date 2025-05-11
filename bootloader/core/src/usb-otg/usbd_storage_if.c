/**
 * @file    usbd_storage_if.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for USB Storage interface layer
 * @note    Based on the STM32 usbd_msc_storage_template.c
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "usb-otg/usbd_storage_if.h"
#include "memory/memdrv_sd.h"

/***************************** Macros Definitions ****************************/

#define STORAGE_LUN_NBR 1u /**< Number of logical unit (1 because only one disk) */

/*************************** Functions Declarations **************************/

static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);

/*************************** Variables Definitions ***************************/

/**
 * @var   inquiry_standard_data
 * @brief USB mass storage Inquiry Standard Data
 */
static const int8_t inquiry_standard_data[STANDARD_INQUIRY_DATA_LEN] = {
    // clang-format off
    0x00,                                   //
    0x80,                                   //
    0x02,                                   //
    0x02,                                   //
    (STANDARD_INQUIRY_DATA_LEN - 5),        //
    0x00,                                   //
    0x00,                                   //
    0x00,                                   //
    'T', 'O', 'L', 'O', 'S', 'A', 'T', ' ', // Manufacturer : 8 bytes
    'T', 'A', 'P', 'A', 'S', ' ', 'h', 'a', // Product : 16 Bytes
    'r', 'd', ' ', 'd', 'r', 'i', 'v', 'e', //
    '0', '.', '0', '1'                      // Version : 4 Bytes
    // clang-format on
};

/**
 * @var     g_usbd_storage_if
 * @brief   USB Device storage interface
 *
 * This interface contains all function allowing the libUSB to handle the library
 */
USBD_StorageTypeDef g_usbd_storage_if = {
    STORAGE_Init_FS,                //
    STORAGE_GetCapacity_FS,         //
    STORAGE_IsReady_FS,             //
    STORAGE_IsWriteProtected_FS,    //
    STORAGE_Read_FS,                //
    STORAGE_Write_FS,               //
    STORAGE_GetMaxLun_FS,           //
    (int8_t *)inquiry_standard_data // cppcheck-suppress misra-c2012-11.8; USBD_StorageTypeDef don't use the const argument so it has to disappear
};

/*************************** Functions Definitions ***************************/

/**
 * @brief  Initializes the storage unit (medium) over USB FS IP
 * @param  lun: Logical unit number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_Init_FS(uint8_t lun)
{
    int8_t return_value = USBD_OK;

    if (SD_GetStatus(lun) != RET_SUCCESSFUL)
    {
        return_value = USBD_FAIL;
    }

    return return_value;
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
    int8_t return_value       = USBD_OK;
    uint32_t local_nb_block   = 0u;
    uint16_t local_block_size = 0u;

    if (SD_Ioctl(lun, GET_SECTOR_COUNT, &local_nb_block) != RET_SUCCESSFUL)
    {
        return_value = USBD_FAIL;
    }

    if (SD_Ioctl(lun, GET_SECTOR_SIZE, &local_block_size) != RET_SUCCESSFUL)
    {
        return_value = USBD_FAIL;
    }

    *block_num  = local_nb_block - 1u;
    *block_size = local_block_size;

    return return_value;
}

/**
 * @brief   Checks whether the medium is ready.
 * @param  lun:  Logical unit number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_IsReady_FS(uint8_t lun)
{
    int8_t return_value = USBD_OK;

    if (SD_GetStatus(lun) != RET_SUCCESSFUL)
    {
        return_value = USBD_FAIL;
    }

    return return_value;
}

/**
 * @brief  Checks whether the medium is write protected.
 * @param  lun: Logical unit number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_IsWriteProtected_FS(uint8_t lun)
{
    UNUSED(lun);

    return USBD_OK;
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
    int8_t return_value = USBD_OK;

    if (SD_ReadBlocks(lun, buf, blk_addr, blk_len) != RET_SUCCESSFUL)
    {
        return_value = USBD_FAIL;
    }

    return return_value;
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
    int8_t return_value = USBD_OK;

    if (SD_WriteBlocks(lun, buf, blk_addr, blk_len) != RET_SUCCESSFUL)
    {
        return_value = USBD_FAIL;
    }

    return return_value;
}

/**
 * @brief  Returns the Max Supported LUNs.
 * @param  None
 * @retval Lun(s) number.
 */
int8_t STORAGE_GetMaxLun_FS(void)
{
    return (STORAGE_LUN_NBR - 1u);
}
