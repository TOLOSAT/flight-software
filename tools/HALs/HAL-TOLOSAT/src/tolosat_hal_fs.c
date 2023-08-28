/**
 * @file    tolosat_hal_fs.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT HAL File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static DSTATUS DiskInitialize(BYTE disk);
static DSTATUS DiskStatus(BYTE disk);
static DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count);
static DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count);
static DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff);

static void SD_Select(void);
static void SD_Unselect(void);
static halStatus_t SD_WaitUntilReady(void);
static halStatus_t SD_SwitchOn(void);
static halStatus_t SD_SwitchOff(void);
static BYTE SD_RxDataBlock(BYTE *buff, UINT len);
static BYTE SD_TxDataBlock(const uint8_t *buff, BYTE token);
static halStatus_t SD_SendCmd(BYTE cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size);

/*************************** Variables Definitions ***************************/

extern spiInst_t spi_sdcard_inst;
static DSTATUS g_disk0_status = STA_NOINIT;           /**< Disk0 Status */
static SDCardStatus_t g_sd_card_status = SD_CARD_OFF; /**< Indicates if SD card is ON/OFF */
static uint8_t g_sd_card_type = CT_NO_TYPE;           /**< SD card type */

/*************************** Functions Definitions ***************************/

/**
 * @fn              FsOpen(FsInst_t *fs_inst)
 * @brief           Function that initialise a FS
 * @param[in,out]   fs_inst Instance that contains FS parameters and driver
 * @retval          #FCT_INVALID_PARAM if fs_inst is null pointer
 * @retval          #FCT_ERROR if cannot create FS
 * @retval          #FCT_SUCCESSFUL else
 */
halStatus_t FsOpen(FsInst_t *fs_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if (fs_inst != NULL)
    {
        fs_inst->driver.disk_initialize = DiskInitialize;
        fs_inst->driver.disk_status = DiskStatus;
        fs_inst->driver.disk_read = DiskRead;
        fs_inst->driver.disk_write = DiskWrite;
        fs_inst->driver.disk_ioctl = DiskIoctl;

        // We link driver functions to FATFS
        uint8_t test_val = FATFS_LinkDriver(&fs_inst->driver, fs_inst->disk_path);
        if (test_val != 0u)
        {
            return_value = FCT_ERROR;
        }
        else
        {
            // Then we mount the disk
            test_val = f_mount(&fs_inst->file_system, "/", 1);
            if (test_val != 0u)
            {
                return_value = FCT_ERROR;
            }
        }
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
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
    /* single drive, drv should be 0 */
    if (disk != DISK0_REF)
    {
        return STA_NOINIT;
    }

    /* no disk */
    if (g_disk0_status & STA_NODISK)
    {
        return g_disk0_status;
    }

    /* power on */
    SD_SwitchOn();

    /* slave select */
    SD_Select();

    /* send GO_IDLE_STATE command */
    if (SD_SendCmd(CMD0, 0) == 1)
    {
        uint32_t counter = 0u;

        /* SDC V2+ accept CMD8 command */
        if (SD_SendCmd(CMD8, 0x1AA) == 1)
        {
            uint8_t ocr[4];

            /* operation condition register */
            (void)SpiRead(&spi_sdcard_inst, (spiMsg_t *)&ocr, 4u);

            /* voltage range 2.7-3.6V */
            if ((ocr[2] == 0x01u) && (ocr[3] == 0xaau))
            {
                // Activates SD card activation process -> CMD41
                uint32_t activation_status = 0u;
                while ((counter < SD_CNT_TIMEOUT) && (activation_status == 0u))
                {
                    BYTE command_answer;
                    command_answer = SD_SendCmd(CMD55, 0);
                    if (command_answer <= 1u)
                    {
                        command_answer = SD_SendCmd(CMD41, 1UL << 30);
                        if (command_answer == 0u)
                        {
                            activation_status = 1u;
                        }
                    }
                    counter++;
                }

                // Read Operation Control Register (OCR) -> CMD58
                if ((counter < SD_CNT_TIMEOUT) && (SD_SendCmd(CMD58, 0) == 0))
                {
                    /* Check CCS bit */
                    (void)SpiRead(&spi_sdcard_inst, (spiMsg_t *)&ocr, 4u);

                    /* SDv2 (HC or SC) */
                    g_sd_card_type = (ocr[0] & 0x40u) ? CT_SD2 | CT_BLOCK : CT_SD2;
                }
            }
        }
        else
        {
            /* SDC V1 or MMC */
            g_sd_card_type = ((SD_SendCmd(CMD55, 0) <= 1) && (SD_SendCmd(CMD41, 0) <= 1)) ? CT_SD1 : CT_MMC;

            // Activates memory card activation process -> CMD41 (SDC V1) or CMD1(MMC)
            uint32_t activation_status = 0u;
            while ((counter < SD_CNT_TIMEOUT) && (activation_status == 0u))
            {
                BYTE command_answer;
                if (g_sd_card_type == CT_SD1)
                {
                    command_answer = SD_SendCmd(CMD55, 0);
                    if (command_answer <= 1u)
                    {
                        command_answer = SD_SendCmd(CMD41, 0);
                        if (command_answer == 0u)
                        {
                            activation_status = 1u;
                        }
                    }
                }
                else
                {
                    command_answer = SD_SendCmd(CMD1, 0);
                    if (command_answer == 0)
                    {
                        activation_status = 1u;
                    }
                }
                counter++;
            }

            /* SET_BLOCKLEN */
            if ((counter >= SD_CNT_TIMEOUT) || (SD_SendCmd(CMD16, 512u) != 0))
            {
                g_sd_card_type = 0;
            }
        }
    }

    /* Idle */
    SD_Unselect();

    /* Clear STA_NOINIT */
    if (g_sd_card_type != CT_NO_TYPE)
    {
        g_disk0_status &= ~STA_NOINIT;
    }
    else
    {
        /* Initialization failed */
        SD_SwitchOff();
    }

    return g_disk0_status;
}

/**
 * @fn          DiskStatus(BYTE disk)
 * @brief       Function that returns disk status
 * @param[in]   disk Driver reference number
 * @return      Disk Status
 */
static DSTATUS DiskStatus(BYTE disk)
{
    if (disk != DISK0_REF)
    {
        return STA_NOINIT;
    }
    return g_disk0_status;
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
    DWORD sector_address = sector;
    UINT sector_read = 0u;

    /* disk should be 0 */
    if ((disk != DISK0_REF) && (count == 0u))
    {
        return RES_PARERR;
    }

    /* no disk */
    if ((g_disk0_status & STA_NOINIT) == STA_NOINIT)
    {
        return RES_NOTRDY;
    }

    /* convert to byte address */
    if (!(g_sd_card_type & CT_SD2))
    {
        sector_address *= 512u;
    }

    SD_Select();

    if (count == 1)
    {
        /* READ_SINGLE_BLOCK */
        if ((SD_SendCmd(CMD17, sector_address) == 0) && SD_RxDataBlock(buff, 512u))
        {
            sector_read = count;
        }
    }
    else
    {
        /* READ_MULTIPLE_BLOCK */
        if (SD_SendCmd(CMD18, sector_address) == 0)
        {
            BYTE read_status = 1u;
            while ((sector_read < count) && (read_status == 1u))
            {
                read_status = SD_RxDataBlock(buff, 512u);
                buff += 512u;
                sector_read++;
            }

            /* STOP_TRANSMISSION */
            SD_SendCmd(CMD12, 0);
        }
    }

    /* Idle */
    SD_Unselect();

    if (sector_read == count)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
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
    DWORD sector_address = sector;
    UINT sector_written = 0u;

    /* disk should be 0 */
    if ((disk != DISK0_REF) || !count)
    {
        return RES_PARERR;
    }

    /* no disk */
    if ((g_disk0_status & STA_NOINIT) == STA_NOINIT)
    {
        return RES_NOTRDY;
    }

    /* write protection */
    if ((g_disk0_status & STA_PROTECT) == STA_PROTECT)
    {
        return RES_WRPRT;
    }

    /* convert to byte address */
    if (!(g_sd_card_type & CT_SD2))
    {
        sector_address *= 512u;
    }

    SD_Select();

    if (count == 1)
    {
        /* WRITE_BLOCK */
        if ((SD_SendCmd(CMD24, sector_address) == 0) && SD_TxDataBlock(buff, 0xFE))
        {
            sector_written = count;
        }
    }
    else
    {
        /* WRITE_MULTIPLE_BLOCK */
        if (g_sd_card_type & CT_SD1)
        {
            SD_SendCmd(CMD55, 0);
            SD_SendCmd(CMD23, count); /* ACMD23 */
        }

        if (SD_SendCmd(CMD25, sector_address) == 0)
        {
            BYTE write_status = 1u;
            while ((sector_written < count) && (write_status == 1u))
            {
                write_status = SD_TxDataBlock(buff, 0xFC);
                buff += 512u;
                sector_written++;
            }

            /* STOP_TRAN token */
            if (!SD_TxDataBlock(0, 0xFD))
            {
                sector_written = 0;
            }
        }
    }

    /* Idle */
    SD_Unselect();

    if (sector_written == count)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
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
    DRESULT res;
    uint8_t csd[16];
    uint8_t *ptr = (uint8_t *)buff;
    WORD csize;

    /* disk should be 0 */
    if (disk != DISK0_REF)
    {
        return RES_PARERR;
    }
    res = RES_ERROR;

    if (cmd == CTRL_POWER)
    {
        switch (*ptr)
        {
        case 0:
            SD_SwitchOff(); /* Power Off */
            res = RES_OK;
            break;
        case 1:
            SD_SwitchOn(); /* Power On */
            res = RES_OK;
            break;
        case 2:
            *(ptr + 1) = g_sd_card_type;
            res = RES_OK; /* Power Check */
            break;
        default:
            res = RES_PARERR;
            break;
        }
    }
    else
    {
        /* no disk */
        if (g_disk0_status & STA_NOINIT)
        {
            return RES_NOTRDY;
        }

        SD_Select();

        switch (cmd)
        {
        case GET_SECTOR_COUNT:
            /* SEND_CSD */
            if ((SD_SendCmd(CMD9, 0) == 0) && (SD_RxDataBlock(csd, 16)))
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
                res = RES_OK;
            }
            break;
        case GET_SECTOR_SIZE:
            *(WORD *)buff = 512u;
            res = RES_OK;
            break;
        case CTRL_SYNC:
            if (SD_WaitUntilReady() == FCT_SUCCESSFUL)
            {
                res = RES_OK;
            }
            break;
        case MMC_GET_CSD:
            /* SEND_CSD */
            if ((SD_SendCmd(CMD9, 0) == 0) && (SD_RxDataBlock(ptr, 16)))
            {
                res = RES_OK;
            }
            break;
        case MMC_GET_CID:
            /* SEND_CID */
            if ((SD_SendCmd(CMD10, 0) == 0) && (SD_RxDataBlock(ptr, 16)))
            {
                res = RES_OK;
            }
            break;
        case MMC_GET_OCR:
            /* READ_OCR */
            if (SD_SendCmd(CMD58, 0) == 0)
            {
                (void)SpiRead(&spi_sdcard_inst, ptr, 4u);
                res = RES_OK;
            }
            break;
        default:
            res = RES_PARERR;
            break;
        }

        SD_Unselect();
    }

    return res;
}

/***************************************
 * SD Card functions
 **************************************/

/**
 * @fn      SD_Select(void)
 * @brief   Select SD card on SPI bus
 * @return  Nothing
 */
static void SD_Select(void)
{
    // Select slave
    HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_RESET);
    // Then send a fill char onto MOSI
    uint8_t fill_char = SPI_FILL_CHAR;
    (void)SpiWrite(&spi_sdcard_inst, &fill_char, 1u);
}

/**
 * @fn      SD_Unselect(void)
 * @brief   Unselect SD card on SPI bus
 * @return  Nothing
 */
static void SD_Unselect(void)
{
    // Send a fill char onto MOSI
    uint8_t fill_char = SPI_FILL_CHAR;
    (void)SpiWrite(&spi_sdcard_inst, &fill_char, 1u);
    // Then unselect slave
    HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_SET);
}

/**
 * @fn      SD_WaitUntilReady(void)
 * @brief   Wait until SD card is ready
 * @retval  SPI_FILL_CHAR if SD card is ready
 * @retval  #FCT_SUCCESSFUL if SD card is ready (spi slave register is now empty)
 * @retval  #FCT_TIMEOUT if function timeouted before clearing SD card being ready
 */
static halStatus_t SD_WaitUntilReady(void)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    uint8_t answer = 0u;
    uint32_t counter = 0u;

    // Read SD card until it returns SPI_FILL_CHAR or timeouted
    while ((answer != SPI_FILL_CHAR) && (counter < SD_CNT_TIMEOUT))
    {
        (void)SpiRead(&spi_sdcard_inst, &answer, 1u);
        counter++;
    }

    if (counter >= SD_CNT_TIMEOUT)
    {
        return_value = FCT_TIMEOUT;
    }

    return return_value;
}

/**
 * @fn      SD_SwitchOn(void)
 * @brief   Wake up the SD card an start initialize SPI mode
 * @retval  #FCT_ERROR if SPI has encountered an error
 * @retval  #FCT_TIMEOUT if SD card never answered IDLE state
 * @retval  #FCT_SUCCESSFUL else
 */
static halStatus_t SD_SwitchOn(void)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    halStatus_t test_val = FCT_SUCCESSFUL;
    uint8_t wakeup_message[SD_WAKEUP_MSG_SIZE];
    uint8_t answer = SPI_FILL_CHAR;

    // Function Core
    // Wakeup SD card by sending pad caracter without selecting it
    SD_Unselect();
    (void)memset(&wakeup_message, SPI_FILL_CHAR, SD_WAKEUP_MSG_SIZE);
    test_val = SpiWrite(&spi_sdcard_inst, (spiMsg_t *)&wakeup_message, SD_WAKEUP_MSG_SIZE);

    // Continue only if SPI has not encountered an error
    if (test_val == FCT_SUCCESSFUL)
    {
        uint8_t reset_spi_mode_cmd[CMD_MSG_SIZE] = {CMD0, 0x00u, 0x00u, 0x00u, 0x00u, 0x95u};

        // Select SD card
        SD_Select();

        // Send reset onto spi mode command
        test_val = SpiWrite(&spi_sdcard_inst, (spiMsg_t *)reset_spi_mode_cmd, CMD_MSG_SIZE);

        // Continue only if SPI has not encountered an error
        if (test_val == FCT_SUCCESSFUL)
        {
            // Wait until SD card
            uint32_t counter = 0u;
            while ((test_val == FCT_SUCCESSFUL) && (answer != SD_IDLE_FLAG) && (counter < SD_CNT_TIMEOUT))
            {
                test_val = SpiRead(&spi_sdcard_inst, &answer, 1u);
                counter++;
            }

            // Unselect SD card
            SD_Unselect();

            // Test if procedure wents well
            if ((test_val == FCT_SUCCESSFUL) && (counter < SD_CNT_TIMEOUT))
            {
                g_sd_card_status = SD_CARD_ON;
            }
            else
            {
                g_sd_card_status = SD_CARD_OFF;
                if (counter >= SD_CNT_TIMEOUT)
                {
                    return_value = FCT_TIMEOUT;
                }
                else
                {
                    return_value = FCT_ERROR;
                }
            }
        }
        else
        {
            return_value = FCT_ERROR;
        }
    }
    else
    {
        return_value = FCT_ERROR;
    }

    return return_value;
}

/**
 * @fn      SD_SwitchOff(void)
 * @brief   Switch off the SD card
 * @retval  #FCT_SUCCESSFUL always
 */
static halStatus_t SD_SwitchOff(void)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    g_sd_card_status = SD_CARD_OFF;

    return return_value;
}

/**
 * @fn          SD_RxDataBlock(BYTE *buff, UINT len)
 * @brief       Receives a block from SD card
 * @param[out]  buff Buffer containing the block received
 * @param[in]   len Length of the block
 * @retval      0 if failed to receive the block
 * @retval      1 if succeed to receive the block
 */
static BYTE SD_RxDataBlock(BYTE *buff, UINT len)
{
    uint8_t token;
    uint32_t counter = 0u;

    /* loop until receive a response or timeout */
    (void)SpiRead(&spi_sdcard_inst, &token, 1u);
    while ((token == SPI_FILL_CHAR) && (counter < SD_CNT_TIMEOUT))
    {
        (void)SpiRead(&spi_sdcard_inst, &token, 1u);
        counter++;
    }

    /* invalid response */
    if (token != 0xfeu)
    {
        return 0; // FALSE
    }

    /* receive data */
    (void)SpiRead(&spi_sdcard_inst, buff, len);

    /* discard CRC */
    uint8_t crc[2] = {0};
    (void)SpiRead(&spi_sdcard_inst, (spiMsg_t *)&crc, 2u);

    return 1; // True
}

/**
 * @fn          SD_TxDataBlock(const uint8_t *buff, BYTE token)
 * @brief       Sends a block from SD card
 * @param[in]   buff Buffer containing the block to send
 * @param[in]   len Length of the block
 * @retval      0 if failed to send the block
 * @retval      1 if succeed to send the block
 */
static BYTE SD_TxDataBlock(const uint8_t *buff, BYTE token)
{
    uint8_t answer = 0u;

    /* wait SD ready */
    if (SD_WaitUntilReady() != FCT_SUCCESSFUL)
    {
        return 0u;
    }

    /* transmit token */
    (void)SpiWrite(&spi_sdcard_inst, &token, 1u);

    /* if it's not STOP token, transmit data */
    if (token != 0xFD)
    {
        (void)SpiWrite(&spi_sdcard_inst, (spiMsg_t *)buff, 512u);

        /* discard CRC */
        uint8_t crc[2] = {0};
        (void)SpiRead(&spi_sdcard_inst, (spiMsg_t *)&crc, 2u);

        /* receive response */
        uint8_t i = 0;
        while ((i <= 64u) && ((answer & 0x1fu) != 0x05u))
        {
            (void)SpiRead(&spi_sdcard_inst, &answer, 1u);
            i++;
        }

        // Clear receive buffer until fill char is received
        uint8_t discarded_answer = 0u;
        while (discarded_answer == 0u)
        {
            // Clear receive buffer
            (void)SpiRead(&spi_sdcard_inst, &discarded_answer, 1u);
        }
    }

    /* transmit 0x05 accepted */
    if ((answer & 0x1fu) == 0x05u)
    {
        return 1u;
    }

    return 0u;
}

/**
 * @fn          SD_SendCmd(BYTE cmd, uint32_t arg, uint8_t *answer)
 * @brief       Sends a command to the SD card
 * @param[in]   cmd Command to send
 * @param[in]   arg Command argument
 * @param[out]  answer Command answer
 * @param[in]   answer_size Command answer size
 * @retval      #FCT_INVALID_PARAM if command is invalid, or answer is null pointer but answer_size non null
 * @retval      #FCT_TIMEOUT if SD card was not ready or CMD12 still busy
 * @retval      #FCT_ERROR if an error occured
 * @retval      #FCT_SUCCESSFUL else
 */
static halStatus_t SD_SendCmd(BYTE cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    halStatus_t test_val = FCT_SUCCESSFUL;

    // Function Core
    if ((answer_size != 0u) && (answer == NULL))
    {
        return_value = FCT_INVALID_PARAM;
    }
    else
    {
        if ((cmd >= 0x40u) && (cmd <= 0x7fu))
        {
            // Wait until transfer complete
            test_val = SD_WaitUntilReady();
            if (test_val == FCT_SUCCESSFUL)
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
                test_val = SpiWrite(&spi_sdcard_inst, (spiMsg_t *)&cmd_msg, CMD_MSG_SIZE);
                if (test_val == FCT_SUCCESSFUL)
                {
                    // If command is CMD12 (STOP_TRANSMISSION) wait until ready
                    if (cmd == CMD12)
                    {
                        test_val = SD_WaitUntilReady();
                        if (test_val != FCT_SUCCESSFUL)
                        {
                            return_value = FCT_TIMEOUT;
                        }
                    }
                    else if (answer_size != 0u)
                    {
                        // Receive answer
                        test_val = SpiRead(&spi_sdcard_inst, answer, answer_size);

                        // Check if everything wents well
                        if (test_val != FCT_SUCCESSFUL)
                        {
                            return_value = FCT_ERROR;
                        }
                    }
                }
                else
                {
                    return_value = FCT_ERROR;
                }
            }
            else
            {
                return_value = FCT_TIMEOUT;
            }
        }
        else
        {
            return FCT_INVALID_PARAM;
        }
    }

    return return_value;
}
