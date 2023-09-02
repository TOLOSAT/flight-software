/**
 * @file    tolosat_fs.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "tolosat_fs.h"
#include "tolosat_hal.h"
#include "io_instances.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static DSTATUS DiskInitialize(BYTE disk);
static DSTATUS DiskStatus(BYTE disk);
static DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count);
static DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count);
static DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff);

static fsStatus_t SD_Select(void);
static fsStatus_t SD_Unselect(void);
static fsStatus_t SD_WaitUntilReady(void);
static fsStatus_t SD_SwitchOn(void);
static fsStatus_t SD_SwitchOff(void);
static fsStatus_t SD_RxDataBlock(uint8_t *buff, uint32_t len);
static fsStatus_t SD_TxDataBlock(const uint8_t *buff, uint32_t len, uint8_t token);
static fsStatus_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size);

/*************************** Variables Definitions ***************************/

static DSTATUS g_disk0_status = STA_NOINIT;           /**< Disk0 Status */
static SDCardStatus_t g_sd_card_status = SD_CARD_OFF; /**< Indicates if SD card is ON/OFF */
static SDCardType_t g_sd_card_type = NOT_SDCARD;      /**< SD card type */

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
static DSTATUS DiskInitialize(BYTE disk)
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
static DSTATUS DiskStatus(BYTE disk)
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
static DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count)
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
 * @fn          DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count)
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
static DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count)
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
 * @fn              DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count)
 * @brief           Function that operates a control over disk
 * @param[in]       disk Disk reference number
 * @param[in]       cmd Buffer of data to write on disk
 * @param[in,out]   buff Buffer to send/receive control data
 * @retval          RES_PARERR if disk is not DISK0_REF or count is null
 * @retval          RES_NOTRDY if disk is not ready
 * @retval          RES_ERROR if IO control has encountered an error
 * @retval          RES_OK else
 */
static DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff)
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

/***************************************/
/********** SD Card functions **********/
/***************************************/

/**
 * @fn      SD_Select(void)
 * @brief   Select SD card on SPI bus
 * @retval  #FS_ERROR if SPI or GPIO error occured
 * @retval  #FS_SUCCESSFUL else
 */
static fsStatus_t SD_Select(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = THAL_SUCCESSFUL;

    // Select slave
    test_hal = GpioWrite(&sd_card_cs, GPIO_PIN_RESET);
    if (test_hal == THAL_SUCCESSFUL)
    {
        // Then send a fill char onto MOSI
        uint8_t fill_char = SPI_FILL_CHAR;
        test_hal = SpiWrite(&spi_sdcard_inst, &fill_char, 1u);
        if (test_hal != THAL_SUCCESSFUL)
        {
            return_value = FS_ERROR;
        }
    }
    else
    {
        return_value = FS_ERROR;
    }

    return return_value;
}

/**
 * @fn      SD_Unselect(void)
 * @brief   Unselect SD card on SPI bus
 * @retval  #FS_ERROR if SPI or GPIO error occured
 * @retval  #FS_SUCCESSFUL else
 */
static fsStatus_t SD_Unselect(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = THAL_SUCCESSFUL;

    // Send a fill char onto MOSI
    uint8_t fill_char = SPI_FILL_CHAR;
    test_hal = SpiWrite(&spi_sdcard_inst, &fill_char, 1u);
    if (test_hal == THAL_SUCCESSFUL)
    {
        // Then unselect slave
        test_hal = GpioWrite(&sd_card_cs, GPIO_PIN_SET);
        if (test_hal != THAL_SUCCESSFUL)
        {
            return_value = FS_ERROR;
        }
    }
    else
    {
        return_value = FS_ERROR;
    }

    return return_value;
}

/**
 * @fn      SD_WaitUntilReady(void)
 * @brief   Wait until SD card is ready
 * @retval  SPI_FILL_CHAR if SD card is ready
 * @retval  #FS_SUCCESSFUL if SD card is ready (spi slave register is now empty)
 * @retval  #FS_TIMEOUT if function timeouted before clearing SD card being ready
 * @retval  #FS_ERROR if SPI has encountered an error
 */
static fsStatus_t SD_WaitUntilReady(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = THAL_SUCCESSFUL;
    uint8_t answer = 0u;
    uint32_t counter = 0u;

    // Read SD card until it returns SPI_FILL_CHAR or timeouted
    while ((test_hal == THAL_SUCCESSFUL) && (answer != SPI_FILL_CHAR) && (counter < SD_CNT_TIMEOUT))
    {
        test_hal = SpiRead(&spi_sdcard_inst, &answer, 1u);
        counter++;
    }

    if (counter >= SD_CNT_TIMEOUT)
    {
        return_value = FS_TIMEOUT;
    }

    if (test_hal == THAL_ERROR)
    {
        return_value = FS_ERROR;
    }

    return return_value;
}

/**
 * @fn      SD_SwitchOn(void)
 * @brief   Wake up the SD card an start initialize SPI mode
 * @retval  #FS_ERROR if SPI has encountered an error
 * @retval  #FS_TIMEOUT if SD card never answered IDLE state
 * @retval  #FS_SUCCESSFUL else
 */
static fsStatus_t SD_SwitchOn(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = THAL_SUCCESSFUL;
    uint8_t wakeup_message[SD_WAKEUP_MSG_SIZE];
    uint8_t answer = SPI_FILL_CHAR;

    // Function Core
    // Wakeup SD card by sending pad caracter without selecting it
    (void)SD_Unselect();
    (void)memset(&wakeup_message, SPI_FILL_CHAR, SD_WAKEUP_MSG_SIZE);
    test_hal = SpiWrite(&spi_sdcard_inst, (spiMsg_t *)&wakeup_message, SD_WAKEUP_MSG_SIZE);

    // Continue only if SPI has not encountered an error
    if (test_hal == THAL_SUCCESSFUL)
    {
        uint8_t reset_spi_mode_cmd[CMD_MSG_SIZE] = {CMD0, 0x00u, 0x00u, 0x00u, 0x00u, 0x95u};

        // Select SD card
        (void)SD_Select();

        // Send reset onto spi mode command
        test_hal = SpiWrite(&spi_sdcard_inst, (spiMsg_t *)reset_spi_mode_cmd, CMD_MSG_SIZE);

        // Continue only if SPI has not encountered an error
        if (test_hal == THAL_SUCCESSFUL)
        {
            // Wait until SD card
            uint32_t counter = 0u;
            while ((test_hal == THAL_SUCCESSFUL) && (answer != SD_IDLE_FLAG) && (counter < SD_CNT_TIMEOUT))
            {
                test_hal = SpiRead(&spi_sdcard_inst, &answer, 1u);
                counter++;
            }

            // Unselect SD card
            (void)SD_Unselect();

            // Test if procedure wents well
            if ((test_hal == THAL_SUCCESSFUL) && (counter < SD_CNT_TIMEOUT))
            {
                g_sd_card_status = SD_CARD_ON;
            }
            else
            {
                g_sd_card_status = SD_CARD_OFF;
                if (counter >= SD_CNT_TIMEOUT)
                {
                    return_value = FS_TIMEOUT;
                }
                else
                {
                    return_value = FS_ERROR;
                }
            }
        }
        else
        {
            return_value = FS_ERROR;
        }
    }
    else
    {
        return_value = FS_ERROR;
    }

    return return_value;
}

/**
 * @fn      SD_SwitchOff(void)
 * @brief   Switch off the SD card
 * @retval  #FS_SUCCESSFUL always
 */
static fsStatus_t SD_SwitchOff(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;

    // Function Core
    g_sd_card_status = SD_CARD_OFF;

    return return_value;
}

/**
 * @fn          SD_RxDataBlock(uint8_t *buff, uint32_t len)
 * @brief       Receives a block from SD card
 * @param[out]  buff Buffer containing the block received
 * @param[in]   len Length of the block
 * @retval      #FS_INVALID_PARAM if buff is null pointer or len is null
 * @retval      #FS_ERROR if SPI has encountered an error
 * @retval      #FS_SUCCESSFUL else
 */
static fsStatus_t SD_RxDataBlock(uint8_t *buff, uint32_t len)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = THAL_SUCCESSFUL;
    uint8_t token = SPI_FILL_CHAR;

    // Function Core
    if ((buff != NULL) && (len != 0u))
    {
        // Loop until receive a response or timeout
        uint32_t counter = 0u;
        while ((test_hal == THAL_SUCCESSFUL) && (token == SPI_FILL_CHAR) && (counter < SD_CNT_TIMEOUT))
        {
            test_hal = SpiRead(&spi_sdcard_inst, &token, 1u);
            counter++;
        }

        // Check if read was successful and gets a start block token
        if ((token == SD_START_BLOCK_TOKEN) && (test_hal == THAL_SUCCESSFUL) && (counter < SD_CNT_TIMEOUT))
        {
            // Receive block
            test_hal = SpiRead(&spi_sdcard_inst, buff, len);

            // Check if block has corretly been read
            if (test_hal == THAL_SUCCESSFUL)
            {
                // Receive (and discard CRC)
                uint8_t crc[2] = {0};
                test_hal = SpiRead(&spi_sdcard_inst, (spiMsg_t *)&crc, 2u);
                // Check if crc has corretly been read
                if (test_hal != THAL_SUCCESSFUL)
                {
                    return_value = FS_ERROR;
                }
            }
            else
            {
                return_value = FS_ERROR;
            }
        }
        else
        {
            return_value = FS_ERROR;
        }
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SD_TxDataBlock(const uint8_t *buff, uint32_t len, uint8_t token)
 * @brief       Sends a block from SD card
 * @param[in]   buff Buffer containing the block to send
 * @param[in]   len Length of the block
 * @param[in]   token Token indicating type of transmission
 * @retval      #FS_INVALID_PARAM if buff is null pointer or len is null except if token is SD_STOP_TOKEN
 * @retval      #FS_ERROR if SPI has encountered an error
 * @retval      #FS_SUCCESSFUL else
 */
static fsStatus_t SD_TxDataBlock(const uint8_t *buff, uint32_t len, uint8_t token)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    fsStatus_t test_wait;
    halStatus_t test_hal;

    // Function Core
    if ((len != 0u) && (buff == NULL) && ((token == SD_STOP_TOKEN)))
    {
        return_value = FS_INVALID_PARAM;
    }
    else
    {
        // Wait until SD card is ready
        test_wait = SD_WaitUntilReady();
        if (test_wait == FS_SUCCESSFUL)
        {
            // Send token
            test_hal = SpiWrite(&spi_sdcard_inst, &token, 1u);
            if (test_hal == THAL_SUCCESSFUL)
            {
                // if it's not STOP token, transmit data
                if (token != SD_STOP_TOKEN)
                {
                    test_hal = SpiWrite(&spi_sdcard_inst, (spiMsg_t *)buff, len); // cppcheck-suppress misra-c2012-11.8
                    if (test_hal == THAL_SUCCESSFUL)
                    {
                        // Read and discard CRC
                        uint8_t crc[2] = {0};
                        test_hal = SpiRead(&spi_sdcard_inst, (spiMsg_t *)&crc, 2u);
                        if (test_hal == THAL_SUCCESSFUL)
                        {
                            uint8_t answer = SPI_FILL_CHAR;
                            uint32_t counter = 0u;
                            while ((test_hal == THAL_SUCCESSFUL) && (answer == SPI_FILL_CHAR) && (counter < SD_CNT_TIMEOUT))
                            {
                                test_hal = SpiRead(&spi_sdcard_inst, &answer, 1u);
                                counter++;
                            }
                            // Check if we get the answer
                            if ((test_hal == THAL_SUCCESSFUL) && (answer != SPI_FILL_CHAR) && (counter < SD_CNT_TIMEOUT))
                            {
                                // Clear receive buffer until fill char is received
                                test_wait = SD_WaitUntilReady();
                                if (test_wait == FS_SUCCESSFUL)
                                {
                                    // Check if data has been accepted
                                    if ((answer & SD_DATA_RESPONSE_MASK) != SD_DATA_ACCEPTED)
                                    {
                                        return_value = FS_ERROR;
                                    }
                                }
                            }
                        }
                        else
                        {
                            return_value = FS_ERROR;
                        }
                    }
                    else
                    {
                        return_value = FS_ERROR;
                    }
                }
            }
            else
            {
                return_value = FS_ERROR;
            }
        }
        else
        {
            return_value = FS_ERROR;
        }
    }

    return return_value;
}

/**
 * @fn          SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size)
 * @brief       Sends a command to the SD card
 * @param[in]   cmd Command to send
 * @param[in]   arg Command argument
 * @param[out]  answer Command answer
 * @param[in]   answer_size Command answer size
 * @retval      #FS_INVALID_PARAM if command is invalid, or answer is null pointer but answer_size non null
 * @retval      #FS_TIMEOUT if SD card was not ready or CMD12 still busy
 * @retval      #FS_ERROR if an error occured
 * @retval      #FS_SUCCESSFUL else
 */
static fsStatus_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    fsStatus_t test_wait;
    halStatus_t test_hal;

    // Function Core
    if ((answer_size != 0u) && (answer == NULL))
    {
        return_value = FS_INVALID_PARAM;
    }
    else
    {
        if ((cmd >= 0x40u) && (cmd <= 0x7fu))
        {
            // Wait until transfer complete
            test_wait = SD_WaitUntilReady();
            if (test_wait == FS_SUCCESSFUL)
            {
                uint8_t cmd_msg[CMD_MSG_SIZE] = {0};
                // Build command message with function arguments
                cmd_msg[0] = (uint8_t)(cmd);
                cmd_msg[1] = (uint8_t)((0xff000000u & arg) >> 24u);
                cmd_msg[2] = (uint8_t)((0x00ff0000u & arg) >> 16u);
                cmd_msg[3] = (uint8_t)((0x0000ff00u & arg) >> 8u);
                cmd_msg[4] = (uint8_t)(0x000000ffu & arg);
                if (cmd == CMD0)
                {
                    cmd_msg[5] = 0x95u; // CRC for CMD0 (because it is required)
                }
                else if (cmd == CMD8)
                {
                    cmd_msg[5] = 0x87u; // CRC for CMD8 (because it is required)
                }
                else
                {
                    cmd_msg[5] = 0x00u; // Else we do not use CRC (because it is optionnal)
                }

                // Send Command
                test_hal = SpiWrite(&spi_sdcard_inst, (spiMsg_t *)&cmd_msg, CMD_MSG_SIZE);
                if (test_hal == THAL_SUCCESSFUL)
                {
                    uint32_t counter = 0u;
                    uint8_t command_status = SPI_FILL_CHAR;
                    while ((command_status == SPI_FILL_CHAR) && (test_hal == THAL_SUCCESSFUL) && (counter < SD_CNT_TIMEOUT))
                    {
                        test_hal = SpiRead(&spi_sdcard_inst, &command_status, 1u);
                    }

                    // Check Result
                    if ((test_hal == THAL_SUCCESSFUL) && (command_status <= SD_IDLE_FLAG))
                    {
                        // If command is CMD12 (STOP_TRANSMISSION) wait until ready
                        if (cmd == CMD12)
                        {
                            test_wait = SD_WaitUntilReady();
                            if (test_wait != FS_SUCCESSFUL)
                            {
                                return_value = FS_TIMEOUT;
                            }
                        }
                        else
                        {
                            if (answer_size != 0u)
                            {
                                // Receive answer
                                test_hal = SpiRead(&spi_sdcard_inst, answer, answer_size);

                                // Check if everything wents well
                                if (test_hal != THAL_SUCCESSFUL)
                                {
                                    return_value = FS_ERROR;
                                }
                            }
                        }
                    }
                    else
                    {
                        return_value = FS_ERROR;
                    }
                }
                else
                {
                    return_value = FS_ERROR;
                }
            }
            else
            {
                return_value = FS_TIMEOUT;
            }
        }
        else
        {
            return_value = FS_INVALID_PARAM;
        }
    }

    return return_value;
}
