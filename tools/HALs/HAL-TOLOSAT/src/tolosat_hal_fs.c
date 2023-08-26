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

#define DISK0_REF   0x00u   /**< Disk0 reference */

/* Definitions for MMC/SDC command */
#define CMD0        0x40u   /**< Command GO_IDLE_STATE */
#define CMD1        0x41u   /**< Command SEND_OP_COND */
#define CMD8        0x48u   /**< Command SEND_IF_COND */
#define CMD9        0x49u   /**< Command SEND_CSD */
#define CMD10       0x4au   /**< Command SEND_CID */
#define CMD12       0x4cu   /**< Command STOP_TRANSMISSION */
#define CMD16       0x50u   /**< Command SET_BLOCKLEN */
#define CMD17       0x51u   /**< Command READ_SINGLE_BLOCK */
#define CMD18       0x52u   /**< Command READ_MULTIPLE_BLOCK */
#define CMD23       0x57u   /**< Command SET_BLOCK_COUNT */
#define CMD24       0x58u   /**< Command WRITE_BLOCK */
#define CMD25       0x59u   /**< Command WRITE_MULTIPLE_BLOCK */
#define CMD41       0x69u   /**< Command SEND_OP_COND (ACMD) */
#define CMD55       0x77u   /**< Command APP_CMD */
#define CMD58       0x7au   /**< Command READ_OCR */

/* MMC/SDC Card types */
#define CT_MMC      0x01u   /**< Card type MMC ver 3 */
#define CT_SD1      0x02u   /**< Card type SD ver 1 */
#define CT_SD2      0x04u   /**< Card type SD ver 2 */
#define CT_SDC      0x06u   /**< Card type SD */
#define CT_BLOCK    0x08u   /**< Card type Block addressing */

#define SD_CS_PORT  GPIOA       /**< GPIO Port of SD card CS Pin */
#define SD_CS_PIN   GPIO_PIN_4  /**< GPIO Pin of SD card CS Pin */


/*************************** Functions Declarations **************************/

static DSTATUS DiskInitialize(BYTE disk);
static DSTATUS DiskStatus(BYTE disk);
static DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count);
static DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count);
static DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff);

static void SD_Select(void);
static void SD_Unselect(void);
static uint8_t SD_ReadyWait(void);
static void SD_SwitchOn(void);
static void SD_SwitchOff(void);
static uint8_t SD_CheckStatus(void);
static BYTE SD_RxDataBlock(BYTE *buff, UINT len);
static BYTE SD_TxDataBlock(const uint8_t *buff, BYTE token);
static BYTE SD_SendCmd(BYTE cmd, uint32_t arg);

/*************************** Variables Definitions ***************************/

extern spiInst_t spi_sdcard_inst;
extern volatile uint16_t Timer1;
extern volatile uint16_t Timer2;

static DSTATUS g_disk0_status = STA_NOINIT; /**< Disk0 Status */
static uint8_t g_sd_card_status;            /**< Indicates if SD card is ON/OFF */
static uint8_t g_sd_card_type;              /**< SD card type */

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
    uint8_t type, ocr[4];

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

    /* check disk type */
    type = 0;

    /* send GO_IDLE_STATE command */
    if (SD_SendCmd(CMD0, 0) == 1)
    {
        /* timeout 1 sec */
        Timer1 = 1000;

        /* SDC V2+ accept CMD8 command, http://elm-chan.org/docs/mmc/mmc_e.html */
        if (SD_SendCmd(CMD8, 0x1AA) == 1)
        {
            /* operation condition register */
            SpiRead(&spi_sdcard_inst, (uint8_t *)&ocr, 4u);

            /* voltage range 2.7-3.6V */
            if (ocr[2] == 0x01 && ocr[3] == 0xAA)
            {
                /* ACMD41 with HCS bit */
                do
                {
                    if (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 1UL << 30) == 0)
                    {
                        break;
                    }
                } while (Timer1);

                /* READ_OCR */
                if (Timer1 && SD_SendCmd(CMD58, 0) == 0)
                {
                    /* Check CCS bit */
                    SpiRead(&spi_sdcard_inst, (uint8_t *)&ocr, 4u);

                    /* SDv2 (HC or SC) */
                    type = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;
                }
            }
        }
        else
        {
            /* SDC V1 or MMC */
            type = (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) <= 1) ? CT_SD1 : CT_MMC;

            do
            {
                if (type == CT_SD1)
                {
                    if (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) == 0)
                    {
                        break; /* ACMD41 */
                    }
                }
                else
                {
                    if (SD_SendCmd(CMD1, 0) == 0)
                    {
                        break; /* CMD1 */
                    }
                }

            } while (Timer1);

            /* SET_BLOCKLEN */
            if (!Timer1 || SD_SendCmd(CMD16, 512) != 0)
            {
                type = 0;
            }
        }
    }

    g_sd_card_type = type;

    /* Idle */
    SD_Unselect();

    /* Clear STA_NOINIT */
    if (type)
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
    /* disk should be 0 */
    if ((disk != DISK0_REF) && (count == 0u))
    {
        return RES_PARERR;
    }

    /* no disk */
    if (g_disk0_status & STA_NOINIT)
    {
        return RES_NOTRDY;
    }

    /* convert to byte address */
    if (!(g_sd_card_type & CT_SD2))
    {
        sector *= 512;
    }

    SD_Select();

    if (count == 1)
    {
        /* READ_SINGLE_BLOCK */
        if ((SD_SendCmd(CMD17, sector) == 0) && SD_RxDataBlock(buff, 512))
        {
            count = 0;
        }
    }
    else
    {
        /* READ_MULTIPLE_BLOCK */
        if (SD_SendCmd(CMD18, sector) == 0)
        {
            do
            {
                if (!SD_RxDataBlock(buff, 512))
                {
                    break;
                }
                buff += 512;
            } while (--count);

            /* STOP_TRANSMISSION */
            SD_SendCmd(CMD12, 0);
        }
    }

    /* Idle */
    SD_Unselect();

    return count ? RES_ERROR : RES_OK;
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
    /* disk should be 0 */
    if ((disk != DISK0_REF) || !count)
    {
        return RES_PARERR;
    }

    /* no disk */
    if (g_disk0_status & STA_NOINIT)
    {
        return RES_NOTRDY;
    }

    /* write protection */
    if (g_disk0_status & STA_PROTECT)
    {
        return RES_WRPRT;
    }

    /* convert to byte address */
    if (!(g_sd_card_type & CT_SD2))
    {
        sector *= 512;
    }

    SD_Select();

    if (count == 1)
    {
        /* WRITE_BLOCK */
        if ((SD_SendCmd(CMD24, sector) == 0) && SD_TxDataBlock(buff, 0xFE))
        {
            count = 0;
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

        if (SD_SendCmd(CMD25, sector) == 0)
        {
            do
            {
                if (!SD_TxDataBlock(buff, 0xFC))
                {
                    break;
                }
                buff += 512;
            } while (--count);

            /* STOP_TRAN token */
            if (!SD_TxDataBlock(0, 0xFD))
            {
                count = 1;
            }
        }
    }

    /* Idle */
    SD_Unselect();

    return count ? RES_ERROR : RES_OK;
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
    uint8_t n, csd[16], *ptr = buff;
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
            *(ptr + 1) = SD_CheckStatus();
            res = RES_OK; /* Power Check */
            break;
        default:
            res = RES_PARERR;
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
            if ((SD_SendCmd(CMD9, 0) == 0) && SD_RxDataBlock(csd, 16))
            {
                if ((csd[0] >> 6) == 1)
                {
                    /* SDC V2 */
                    csize = csd[9] + ((WORD)csd[8] << 8) + 1;
                    *(DWORD *)buff = (DWORD)csize << 10;
                }
                else
                {
                    /* MMC or SDC V1 */
                    n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
                    csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
                    *(DWORD *)buff = (DWORD)csize << (n - 9);
                }
                res = RES_OK;
            }
            break;
        case GET_SECTOR_SIZE:
            *(WORD *)buff = 512;
            res = RES_OK;
            break;
        case CTRL_SYNC:
            if (SD_ReadyWait() == SPI_FILL_CHAR)
            {
                res = RES_OK;
            }
            break;
        case MMC_GET_CSD:
            /* SEND_CSD */
            if (SD_SendCmd(CMD9, 0) == 0 && SD_RxDataBlock(ptr, 16))
            {
                res = RES_OK;
            }
            break;
        case MMC_GET_CID:
            /* SEND_CID */
            if (SD_SendCmd(CMD10, 0) == 0 && SD_RxDataBlock(ptr, 16))
            {
                res = RES_OK;
            }
            break;
        case MMC_GET_OCR:
            /* READ_OCR */
            if (SD_SendCmd(CMD58, 0) == 0)
            {
                SpiRead(&spi_sdcard_inst, (uint8_t *)&ptr, 4u);
                res = RES_OK;
            }
            break;
        default:
            res = RES_PARERR;
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
    SpiWrite(&spi_sdcard_inst, &fill_char, 1u);
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
    SpiWrite(&spi_sdcard_inst, &fill_char, 1u);
    // Then unselect slave
    HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_SET);
}

/**
 * @fn      SD_ReadyWait(void)
 * @brief   Wait until SD card is ready
 * @retval  SPI_FILL_CHAR if SD card is ready
 * @retval  0 or something else if SD card is not ready and timeouted
 */
static uint8_t SD_ReadyWait(void)
{
    uint8_t result;

    /* timeout 500ms */
    Timer2 = 500;

    // Read SD card until it returns SPI_FILL_CHAR or timeouted
    SpiRead(&spi_sdcard_inst, &result, 1u);
    while ((result != SPI_FILL_CHAR) && Timer2)
    {
        SpiRead(&spi_sdcard_inst, &result, 1u);
    }

    return result;
}

/**
 * @fn      SD_SwitchOn(void)
 * @brief   Switch on the SD card and wake it up
 * @return  Nothing
 */
static void SD_SwitchOn(void)
{
    uint8_t args[6];
    uint8_t line_up_msg[10];
    uint8_t answer = 0;
    uint32_t counter = 0x1FFF;

    /* transmit bytes to wake up */
    SD_Unselect();
    memset(&line_up_msg, SPI_FILL_CHAR, 10u);
    SpiWrite(&spi_sdcard_inst, (uint8_t *)&line_up_msg, 10u);

    /* slave select */
    SD_Select();

    /* make idle state */
    args[0] = CMD0; /* CMD0:GO_IDLE_STATE */
    args[1] = 0;
    args[2] = 0;
    args[3] = 0;
    args[4] = 0;
    args[5] = 0x95; /* CRC */

    SpiWrite(&spi_sdcard_inst, (uint8_t *)args, sizeof(args));

    /* wait response */
    SpiRead(&spi_sdcard_inst, &answer, 1u);
    while ((answer != 0x01) && counter)
    {
        SpiRead(&spi_sdcard_inst, &answer, 1u);
        counter--;
    }

    SD_Unselect();
    SpiWrite(&spi_sdcard_inst, (uint8_t *)&line_up_msg, 1u);

    g_sd_card_status = 1;
}

/**
 * @fn      SD_SwitchOff(void)
 * @brief   Switch off the SD card
 * @return  Nothing
 */
static void SD_SwitchOff(void)
{
    g_sd_card_status = 0;
}

/**
 * @fn      SD_CheckStatus(void)
 * @brief   Return if sd card is ON/OFF status of SD card 
 * @return  SD card status
 */
static uint8_t SD_CheckStatus(void)
{
    return g_sd_card_status;
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

    /* timeout 200ms */
    Timer1 = 200;

    /* loop until receive a response or timeout */
    SpiRead(&spi_sdcard_inst, &token, 1u);
    while ((token == SPI_FILL_CHAR) && Timer1)
    {
        SpiRead(&spi_sdcard_inst, &token, 1u);
    }

    /* invalid response */
    if (token != 0xFE)
    {
        return 0; // FALSE
    }

    /* receive data */
    SpiRead(&spi_sdcard_inst, buff, len);

    /* discard CRC */
    uint8_t crc[2] = {0};
    SpiRead(&spi_sdcard_inst, (uint8_t *)&crc, 2u);

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
    uint8_t answer;
    uint8_t i = 0;

    /* wait SD ready */
    if (SD_ReadyWait() != SPI_FILL_CHAR)
    {
        return 0;
    }

    /* transmit token */
    SpiWrite(&spi_sdcard_inst, &token, 1u);

    /* if it's not STOP token, transmit data */
    if (token != 0xFD)
    {
        SpiWrite(&spi_sdcard_inst, (uint8_t *)buff, 512u);

        /* discard CRC */
        uint8_t crc[2] = {0};
        SpiRead(&spi_sdcard_inst, (uint8_t *)&crc, 2u);

        /* receive response */
        while (i <= 64)
        {
            SpiRead(&spi_sdcard_inst, &answer, 1u);

            /* transmit 0x05 accepted */
            if ((answer & 0x1F) == 0x05)
            {
                break;
            }
            i++;
        }

        // Clear receive buffer until fill char is received
        uint8_t discarded_answer = 0u;
        while (discarded_answer == 0u)
        {
            // Clear receive buffer
            SpiRead(&spi_sdcard_inst, &discarded_answer, 1u);
        }
    }

    /* transmit 0x05 accepted */
    if ((answer & 0x1F) == 0x05)
    {
        return 1;
    }

    return 0;
}

/**
 * @fn          SD_SendCmd(BYTE cmd, uint32_t arg)
 * @brief       Sends a command to the SD card
 * @param[in]   cmd Command to send
 * @param[in]   arg Argument of the command
 * @retval      0 or 1 according to the anwswer of SD card
 * @retval      0xff if a problem occurred
 */
static BYTE SD_SendCmd(BYTE cmd, uint32_t arg)
{
    uint8_t crc, res;
    uint8_t arg_msg[4];

    // Convert Argument into uint8_t array
    arg_msg[0] = (uint8_t)((0xff000000 & arg) >> 24u);
    arg_msg[1] = (uint8_t)((0x00ff0000 & arg) >> 16u);
    arg_msg[2] = (uint8_t)((0x0000ff00 & arg) >> 8u);
    arg_msg[3] = (uint8_t)(0x000000ff & arg);

    /* wait SD ready */
    if (SD_ReadyWait() != SPI_FILL_CHAR)
    {
        return 0xFF;
    }

    /* transmit command */
    SpiWrite(&spi_sdcard_inst, &cmd, 1u);                /* Command */
    SpiWrite(&spi_sdcard_inst, (uint8_t *)&arg_msg, 4u); /* Command */

    /* prepare CRC */
    if (cmd == CMD0)
    {
        crc = 0x95; /* CRC for CMD0(0) */
    }
    else if (cmd == CMD8)
    {
        crc = 0x87; /* CRC for CMD8(0x1AA) */
    }
    else
    {
        crc = 1;
    }

    /* transmit CRC */
    SpiWrite(&spi_sdcard_inst, &crc, 1u);

    /* Skip a stuff byte when STOP_TRANSMISSION */
    if (cmd == CMD12)
    {
        // Send a fill char onto MOSI
        uint8_t fill_char = SPI_FILL_CHAR;
        SpiWrite(&spi_sdcard_inst, &fill_char, 1u);
    }

    /* receive response */
    uint8_t n = 10;
    SpiRead(&spi_sdcard_inst, &res, 1u);
    while ((res & 0x80) && --n)
    {
        SpiRead(&spi_sdcard_inst, &res, 1u);
    }

    return res;
}
