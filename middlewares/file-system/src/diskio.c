/**
 * @file    diskio.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT Disk IO functions
 * @date    17/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "diskio.h"
#include "sd_spi_driver.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static DSTATUS g_disk0_status = STA_NOINIT; /**< Disk0 Status */

/*************************** Functions Definitions ***************************/

/**
 * @fn          DiskInitialize(BYTE disk)
 * @brief       Function that initialise disk drive
 * @param[in]   disk Disk reference number
 * @retval      STA_NOINIT if disk number is not valid
 * @retval      STA_NODISK if disk is not available
 * @retval      0 if disk initialization is a success
 */
DSTATUS DiskInitialize(BYTE disk)
{
    // Variables Initialization
    DSTATUS return_value = STA_NOINIT;
    fsStatus_t test_hal;

    // Single drive only, drv should be 0
    if (disk == DISK0_REF)
    {
        // Switch on and select SD card
        test_hal = SD_SwitchOn();
        if (test_hal == FS_SUCCESSFUL)
        {
            // Select SD card (transaction begins)
            (void)SD_Select();

            // Send Go Idle Command to start initialisation procedure
            test_hal = SD_SendCmd(CMD0, NULL_COMMAND_ARG, NULL, 0u);
            if (test_hal == FS_SUCCESSFUL)
            {
                // If CMD8 command is accept it is SDC V2 type, if not type is SDC V1
                uint8_t interface_condition[CMD_MSG_ANSWER_SIZE] = {0};
                test_hal = SD_SendCmd(CMD8, SD_CARD_INTERFACE_COND, (uint8_t *)&interface_condition, 4u);
                if (test_hal == FS_SUCCESSFUL)
                {
                    // Type is SDC V2+
                    // Now check voltage set is effective
                    if ((interface_condition[2] == (uint8_t)((0x0000ff00u & SD_CARD_INTERFACE_COND) >> 8u)) && (interface_condition[3] == (uint8_t)(0x000000ffu & SD_CARD_INTERFACE_COND)))
                    {
                        // Activates SD card activation process until initialisation ended
                        uint32_t counter = 0u;
                        test_hal = FS_BUSY;
                        while ((test_hal != FS_SUCCESSFUL) && (counter < SD_INITIALIZATION_TRIALS))
                        {
                            test_hal = SD_SendCmd(CMD55, NULL_COMMAND_ARG, NULL, 0u);
                            if (test_hal == FS_SUCCESSFUL)
                            {
                                // Sends host capacity support information and activates the card's initialization process. (HCS bit = 1 because we supports SDHC and SDXC)
                                test_hal = SD_SendCmd(CMD41, SD_INITIALIZATION_CONF, NULL, 0u);
                            }
                            counter++;
                        }

                        // Check if initialisation wents well
                        if (test_hal == FS_SUCCESSFUL)
                        {
                            // Read Operation Control Register (OCR) and check CCS (card capacity status)
                            uint8_t ocr[CMD_MSG_ANSWER_SIZE] = {0};
                            test_hal = SD_SendCmd(CMD58, NULL_COMMAND_ARG, (uint8_t *)&ocr, 4u);
                            if (test_hal == FS_SUCCESSFUL)
                            {
                                // Check if High Capacity or not (SDCARD_V2HC vs SDCARD_V2)
                                if ((ocr[0] & SD_CCS_BITMASK) == SD_CCS_BITMASK)
                                {
                                    g_sd_card_type = SDCARD_V2HC;
                                }
                                else
                                {
                                    g_sd_card_type = SDCARD_V2;
                                }
                            }
                        }
                    }
                }
                else
                {
                    // Type is SDC V1 or MMC
                    test_hal = SD_SendCmd(CMD55, NULL_COMMAND_ARG, NULL, 0);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        test_hal = SD_SendCmd(CMD41, NULL_COMMAND_ARG, NULL, 0);
                        if (test_hal == FS_SUCCESSFUL)
                        {
                            // Set Block Lenght to 512 bits
                            test_hal = SD_SendCmd(CMD16, SD_BLOCK_SIZE, NULL, 0u);
                            if (test_hal != FS_SUCCESSFUL)
                            {
                                g_sd_card_type = SDCARD_V1;
                            }
                        }
                    }
                }

                // Unselect SD card (transaction ended)
                (void)SD_Unselect();

                // Status No INIT flag
                if (g_sd_card_type != NOT_SDCARD)
                {
                    g_disk0_status &= ~STA_NOINIT;
                    return_value = g_disk0_status;
                }
                else
                {
                    // Initialization failed
                    (void)SD_SwitchOff();
                }
            }
            else
            {
                // Switch on failed
                (void)SD_Unselect();
                (void)SD_SwitchOff();
                return_value = STA_NOINIT;
            }
        }
        else
        {
            return_value = STA_NOINIT;
        }
    }
    else
    {
        return_value = STA_NODISK;
    }

    return return_value;
}

/**
 * @fn          DiskStatus(BYTE disk)
 * @brief       Function that returns disk status
 * @param[in]   disk Driver reference number
 * @return      Disk Status
 */
DSTATUS DiskStatus(BYTE disk)
{
    // Variables Initialization
    DSTATUS return_value = STA_NOINIT;

    // Function Core
    if (disk != DISK0_REF)
    {
        return_value = STA_NODISK;
    }
    else
    {
        return_value = g_disk0_status;
    }

    return return_value;
}

/**
 * @fn          DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count)
 * @brief       Function that reads inside disk
 * @param[in]   disk Disk reference number
 * @param[out]  buff Buffer where data goes after reading
 * @param[in]   sector First sector address
 * @param[in]   count Number of sector to read
 * @retval      RES_PARERR if disk is not DISK0_REF or count is null
 * @retval      RES_NOTRDY if disk is not ready
 * @retval      RES_ERROR if reading has encountered an error
 * @retval      RES_OK else
 */
DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count)
{
    // Variables Initialization
    DRESULT return_value = RES_OK;
    DWORD sector_address = sector;
    UINT sector_read = 0u;
    fsStatus_t test_hal;

    // Function Core
    if ((disk == DISK0_REF) && (count != 0u) && (buff != NULL))
    {
        // Check if disk is ready
        if ((g_disk0_status & STA_NOINIT) == STA_NOINIT)
        {
            return_value = RES_NOTRDY;
        }
        else
        {
            // If not high capacity card convert sector number to byte address
            if (g_sd_card_type != SDCARD_V2HC)
            {
                sector_address *= SD_BLOCK_SIZE;
            }

            // Transaction begins, select SD card
            (void)SD_Select();

            if (count == 1u)
            {
                /* READ_SINGLE_BLOCK */
                test_hal = SD_SendCmd(CMD17, sector_address, NULL, 0u);
                if (test_hal == FS_SUCCESSFUL)
                {
                    test_hal = SD_RxDataBlock(buff, SD_BLOCK_SIZE);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        sector_read = count;
                    }
                }
            }
            else
            {
                /* READ_MULTIPLE_BLOCK */
                test_hal = SD_SendCmd(CMD18, sector_address, NULL, 0u);
                if (test_hal == FS_SUCCESSFUL)
                {
                    while ((sector_read < count) && (test_hal == FS_SUCCESSFUL))
                    {
                        test_hal = SD_RxDataBlock((buff + (sector_read * SD_BLOCK_SIZE)), SD_BLOCK_SIZE);
                        sector_read++;
                    }

                    /* STOP_TRANSMISSION */
                    test_hal = SD_SendCmd(CMD12, NULL_COMMAND_ARG, NULL, 0u);
                    if (test_hal != FS_SUCCESSFUL)
                    {
                        sector_read = 0;
                    }
                }
            }

            // Transaction ended, unselect SD card
            (void)SD_Unselect();

            // Check if we have read the right amount of sectors
            if (sector_read != count)
            {
                return_value = RES_ERROR;
            }
        }
    }
    else
    {
        return_value = RES_PARERR;
    }

    return return_value;
}

/**
 * @fn          DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count)
 * @brief       Function that writes inside disk
 * @param[in]   disk Disk reference number
 * @param[in]   buff Buffer of data to write on disk
 * @param[in]   sector First sector address
 * @param[in]   count Number of sector to write
 * @retval      RES_PARERR if disk is not DISK0_REF or count is null
 * @retval      RES_NOTRDY if disk is not ready
 * @retval      RES_WRPRT if disk is protected against reading
 * @retval      RES_ERROR if writing has encountered an error
 * @retval      RES_OK else
 */
DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count)
{
    // Variables Initialization
    DRESULT return_value = RES_OK;
    DWORD sector_address = sector;
    UINT sector_written = 0u;
    fsStatus_t test_hal;

    // Function Core
    if ((disk == DISK0_REF) && (count != 0u) && (buff != NULL))
    {
        // Check if disk is ready
        if ((g_disk0_status & STA_NOINIT) == STA_NOINIT)
        {
            return_value = RES_NOTRDY;
        }
        else
        {

            // Check if allowed to write
            if ((g_disk0_status & STA_PROTECT) == STA_PROTECT)
            {
                return_value = RES_WRPRT;
            }
            else
            {

                // If not high capacity card convert sector number to byte address
                if (g_sd_card_type != SDCARD_V2HC)
                {
                    sector_address *= SD_BLOCK_SIZE;
                }

                // Transaction begins, select SD card
                (void)SD_Select();

                if (count == 1u)
                {
                    /* WRITE_BLOCK */
                    test_hal = SD_SendCmd(CMD24, sector_address, NULL, 0u);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        test_hal = SD_TxDataBlock(buff, SD_BLOCK_SIZE, SD_START_BLOCK_TOKEN);
                        if (test_hal == FS_SUCCESSFUL)
                        {
                            sector_written = count;
                        }
                    }
                }
                else
                {
                    /* WRITE_MULTIPLE_BLOCK */
                    if (g_sd_card_type == SDCARD_V1)
                    {
                        test_hal = SD_SendCmd(CMD55, NULL_COMMAND_ARG, NULL, 0u);
                        if (test_hal == FS_SUCCESSFUL)
                        {
                            test_hal = SD_SendCmd(CMD23, count, NULL, 0u);
                            if (test_hal == FS_SUCCESSFUL)
                            {
                                test_hal = SD_SendCmd(CMD25, sector_address, NULL, 0u);
                            }
                        }
                    }
                    else
                    {
                        test_hal = SD_SendCmd(CMD25, sector_address, NULL, 0u);
                    }

                    // Check if multiple block write init went well
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        while ((sector_written < count) && (test_hal == FS_SUCCESSFUL))
                        {
                            test_hal = SD_TxDataBlock((buff + (sector_written * SD_BLOCK_SIZE)), SD_BLOCK_SIZE, SD_START_MULT_BLOCK_TOKEN);
                            sector_written++;
                        }

                        /* STOP_TRAN token */
                        test_hal = SD_TxDataBlock(NULL, 0u, SD_STOP_TOKEN);
                        if (test_hal != FS_SUCCESSFUL)
                        {
                            sector_written = 0;
                        }
                    }
                }

                // Transaction ended, unselect SD card
                (void)SD_Unselect();

                if (sector_written != count)
                {
                    return_value = RES_ERROR;
                }
            }
        }
    }
    else
    {
        return_value = RES_PARERR;
    }

    return return_value;
}

/**
 * @fn              DiskIoctl(BYTE disk, BYTE cmd, void *buff)
 * @brief           Function that operates a control over disk
 * @param[in]       disk Disk reference number
 * @param[in]       cmd Buffer of data to write on disk
 * @param[in,out]   buff Buffer to send/receive control data
 * @retval          RES_PARERR if disk is not DISK0_REF or count is null
 * @retval          RES_NOTRDY if disk is not ready
 * @retval          RES_ERROR if IO control has encountered an error
 * @retval          RES_OK else
 */
DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff)
{
    // Variables Initialization
    DRESULT return_value = RES_OK;
    fsStatus_t test_hal = FS_SUCCESSFUL;
    uint8_t *ptr = (uint8_t *)buff; // cppcheck-suppress misra-c2012-11.5
    uint8_t csd[16];
    WORD csize;

    /* disk should be 0 */
    if (disk == DISK0_REF)
    {
        return_value = RES_ERROR;

        if (cmd == CTRL_POWER)
        {
            switch (*ptr)
            {
            case 0:
                (void)SD_SwitchOff();
                return_value = RES_OK;
                break;
            case 1:
                test_hal = SD_SwitchOn();
                if (test_hal == FS_SUCCESSFUL)
                {
                    return_value = RES_OK;
                }
                else
                {
                    return_value = RES_ERROR;
                }

                break;
            case 2:
                ptr[1] = g_sd_card_status;
                return_value = RES_OK;
                break;
            default:
                return_value = RES_PARERR;
                break;
            }
        }
        else
        {
            // Check Disk Status
            if ((g_disk0_status & STA_NOINIT) != STA_NOINIT)
            {
                (void)SD_Select();

                switch (cmd)
                {
                case GET_SECTOR_COUNT:
                    test_hal = SD_SendCmd(CMD9, NULL_COMMAND_ARG, NULL, 0u);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        test_hal = SD_RxDataBlock(csd, 16u);
                        if (test_hal == FS_SUCCESSFUL)
                        {
                            if ((csd[0] >> 6) == 0x01u)
                            {
                                /* SDC V2 */
                                csize = csd[9] + ((WORD)csd[8] << 8) + 1;
                                *(DWORD *)buff = (DWORD)csize << 10;
                            }
                            else
                            {
                                /* MMC or SDC V1 */
                                uint8_t n = (csd[5] & 0x0fu) + ((csd[10] & 0x80u) >> 7) + ((csd[9] & 0x03u) << 1) + 2u;
                                csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 0x03u) << 10) + 1u;
                                *(DWORD *)buff = (DWORD)csize << (n - 9u);
                            }
                            return_value = RES_OK;
                        }
                    }
                    break;
                case GET_SECTOR_SIZE:
                    *(WORD *)buff = SD_BLOCK_SIZE;
                    return_value = RES_OK;
                    break;
                case CTRL_SYNC:
                    test_hal = SD_WaitUntilReady();
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        return_value = RES_OK;
                    }
                    break;
                case MMC_GET_CSD:
                    test_hal = SD_SendCmd(CMD9, NULL_COMMAND_ARG, NULL, 0u);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        test_hal = SD_RxDataBlock(ptr, 16u);
                        if (test_hal == FS_SUCCESSFUL)
                        {
                            return_value = RES_OK;
                        }
                    }
                    break;
                case MMC_GET_CID:
                    test_hal = SD_SendCmd(CMD10, NULL_COMMAND_ARG, NULL, 0u);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        test_hal = SD_RxDataBlock(ptr, 16u);
                        if (test_hal == FS_SUCCESSFUL)
                        {
                            return_value = RES_OK;
                        }
                    }
                    break;
                case MMC_GET_OCR:
                    test_hal = SD_SendCmd(CMD58, 0, ptr, 4u);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        return_value = RES_OK;
                    }
                    break;
                default:
                    return_value = RES_PARERR;
                    break;
                }

                (void)SD_Unselect();
            }
            else
            {
                return_value = RES_NOTRDY;
            }
        }
    }
    else
    {
        return_value = RES_PARERR;
    }

    return return_value;
}
