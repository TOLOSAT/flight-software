/**
 * @file    tolosat_fs.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "tolosat_fs.h"
#include "sd_spi_driver.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static DSTATUS g_disk0_status = STA_NOINIT;           /**< Disk0 Status */

/*************************** Functions Definitions ***************************/

/**
 * @fn              FsOpen(FsInst_t *fs_inst)
 * @brief           Function that initialise a FS
 * @param[in,out]   fs_inst Instance that contains FS parameters and driver
 * @retval          #FS_INVALID_PARAM if fs_inst is null pointer
 * @retval          #FS_ERROR if cannot create FS
 * @retval          #FS_SUCCESSFUL else
 */
fsStatus_t FsOpen(FsInst_t *fs_inst)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;

    // Function Core
    if (fs_inst != NULL)
    {
        fs_inst->driver.disk_initialize = DiskInitialize;
        fs_inst->driver.disk_status = DiskStatus;
        fs_inst->driver.disk_read = DiskRead;
        fs_inst->driver.disk_write = DiskWrite;
        fs_inst->driver.disk_ioctl = DiskIoctl;

        // We link driver functions to FATFS
        uint8_t test_hal = FATFS_LinkDriver(&fs_inst->driver, fs_inst->disk_path);
        if (test_hal != 0u)
        {
            return_value = FS_ERROR;
        }
        else
        {
            // Then we mount the disk
            test_hal = f_mount(&fs_inst->file_system, "/", 1);
            if (test_hal != 0u)
            {
                return_value = FS_ERROR;
            }
        }
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
}

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
            (void)SD_Select();

            // Send Go Idle Command to start initialisation procedure
            test_hal = SD_SendCmd(CMD0, 0x00000000u, NULL, 0u);
            if (test_hal == FS_SUCCESSFUL)
            {
                // If CMD8 command is accept it is SDC V2 type, if not type is SDC V1
                uint8_t ocr[4];
                test_hal = SD_SendCmd(CMD8, 0x000001aau, (uint8_t *)&ocr, 4u);
                if (test_hal == FS_SUCCESSFUL)
                {
                    // Type is SDC V2+
                    // Now check voltage
                    if ((ocr[2] == 0x01u) && (ocr[3] == 0xaau))
                    {
                        // Activates SD card activation process -> CMD41
                        test_hal = SD_SendCmd(CMD55, 0x00000000u, NULL, 0u);
                        if (test_hal == FS_SUCCESSFUL)
                        {
                            test_hal = SD_SendCmd(CMD41, 0x40000000u, NULL, 0u);
                            if (test_hal == FS_SUCCESSFUL)
                            {
                                // Read Operation Control Register (OCR) -> CMD58
                                test_hal = SD_SendCmd(CMD58, 0x00000000u, (uint8_t *)&ocr, 4u);
                                if (test_hal == FS_SUCCESSFUL)
                                {
                                    // Check if High Capacity or not (SDCARD_V2HC vs SDCARD_V2)
                                    if ((ocr[0] & SD_HCS_BITMASK) == SD_HCS_BITMASK)
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
                }
                else
                {
                    // Type is SDC V1 or MMC
                    test_hal = SD_SendCmd(CMD55, 0x00000000u, NULL, 0);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        test_hal = SD_SendCmd(CMD41, 0x00000000u, NULL, 0);
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

                /* Idle */
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
                    test_hal = SD_SendCmd(CMD12, 0x00000000u, NULL, 0u);
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
                        test_hal = SD_SendCmd(CMD55, 0x00000000u, NULL, 0u);
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
                    test_hal = SD_SendCmd(CMD9, 0x00000000u, NULL, 0u);
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
                    test_hal = SD_SendCmd(CMD9, 0x00000000u, NULL, 0u);
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
                    test_hal = SD_SendCmd(CMD10, 0x00000000u, NULL, 0u);
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
