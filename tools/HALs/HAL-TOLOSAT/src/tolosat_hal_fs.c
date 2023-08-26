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

/* Definitions for MMC/SDC command */
#define CMD0 (0x40 + 0)   /* GO_IDLE_STATE */
#define CMD1 (0x40 + 1)   /* SEND_OP_COND */
#define CMD8 (0x40 + 8)   /* SEND_IF_COND */
#define CMD9 (0x40 + 9)   /* SEND_CSD */
#define CMD10 (0x40 + 10) /* SEND_CID */
#define CMD12 (0x40 + 12) /* STOP_TRANSMISSION */
#define CMD16 (0x40 + 16) /* SET_BLOCKLEN */
#define CMD17 (0x40 + 17) /* READ_SINGLE_BLOCK */
#define CMD18 (0x40 + 18) /* READ_MULTIPLE_BLOCK */
#define CMD23 (0x40 + 23) /* SET_BLOCK_COUNT */
#define CMD24 (0x40 + 24) /* WRITE_BLOCK */
#define CMD25 (0x40 + 25) /* WRITE_MULTIPLE_BLOCK */
#define CMD41 (0x40 + 41) /* SEND_OP_COND (ACMD) */
#define CMD55 (0x40 + 55) /* APP_CMD */
#define CMD58 (0x40 + 58) /* READ_OCR */

/* MMC card type flags (MMC_GET_TYPE) */
#define CT_MMC 0x01   /* MMC ver 3 */
#define CT_SD1 0x02   /* SD ver 1 */
#define CT_SD2 0x04   /* SD ver 2 */
#define CT_SDC 0x06   /* SD */
#define CT_BLOCK 0x08 /* Block addressing */

#define SD_CS_PORT GPIOA
#define SD_CS_PIN GPIO_PIN_4

/*************************** Functions Declarations **************************/

static DSTATUS DiskInitialize(BYTE pdrv);
static DSTATUS DiskStatus(BYTE pdrv);
static DRESULT DiskRead(BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
static DRESULT DiskWrite(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
static DRESULT DiskIoctl(BYTE pdrv, BYTE cmd, void *buff);

static void SelectSDCard(void);
static void DeselectSDCard(void);
static uint8_t SD_ReadyWait(void);
static void SD_PowerOn(void);
static void SD_PowerOff(void);
static uint8_t SD_CheckPower(void);
static BYTE SD_RxDataBlock(BYTE *buff, UINT len);
static BYTE SD_TxDataBlock(const uint8_t *buff, BYTE token);
static BYTE SD_SendCmd(BYTE cmd, uint32_t arg);

/*************************** Variables Definitions ***************************/

extern spiInst_t spi_sdcard_inst;
extern volatile uint16_t Timer1, Timer2; /* 1ms Timer Counter */

static volatile DSTATUS Stat = STA_NOINIT; /* Disk Status */
static uint8_t CardType;                   /* Type 0:MMC, 1:SDC, 2:Block addressing */
static uint8_t PowerFlag;                  /* Power flag */

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

static DSTATUS DiskInitialize(BYTE pdrv)
{
    uint8_t type, ocr[4];

    /* single drive, drv should be 0 */
    if (pdrv)
    {
        return STA_NOINIT;
    }

    /* no disk */
    if (Stat & STA_NODISK)
    {
        return Stat;
    }

    /* power on */
    SD_PowerOn();

    /* slave select */
    SelectSDCard();

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

    CardType = type;

    /* Idle */
    DeselectSDCard();

    /* Clear STA_NOINIT */
    if (type)
    {
        Stat &= ~STA_NOINIT;
    }
    else
    {
        /* Initialization failed */
        SD_PowerOff();
    }

    return Stat;
}

static DSTATUS DiskStatus(BYTE pdrv)
{
    if (pdrv)
    {
        return STA_NOINIT;
    }
    return Stat;
}

static DRESULT DiskRead(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    /* pdrv should be 0 */
    if (pdrv || !count)
    {
        return RES_PARERR;
    }

    /* no disk */
    if (Stat & STA_NOINIT)
    {
        return RES_NOTRDY;
    }

    /* convert to byte address */
    if (!(CardType & CT_SD2))
    {
        sector *= 512;
    }

    SelectSDCard();

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
    DeselectSDCard();

    return count ? RES_ERROR : RES_OK;
}

static DRESULT DiskWrite(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
    /* pdrv should be 0 */
    if (pdrv || !count)
    {
        return RES_PARERR;
    }

    /* no disk */
    if (Stat & STA_NOINIT)
    {
        return RES_NOTRDY;
    }

    /* write protection */
    if (Stat & STA_PROTECT)
    {
        return RES_WRPRT;
    }

    /* convert to byte address */
    if (!(CardType & CT_SD2))
    {
        sector *= 512;
    }

    SelectSDCard();

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
        if (CardType & CT_SD1)
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
    DeselectSDCard();

    return count ? RES_ERROR : RES_OK;
}

static DRESULT DiskIoctl(BYTE pdrv, BYTE cmd, void *buff)
{
    DRESULT res;
    uint8_t n, csd[16], *ptr = buff;
    WORD csize;

    /* pdrv should be 0 */
    if (pdrv)
    {
        return RES_PARERR;
    }
    res = RES_ERROR;

    if (cmd == CTRL_POWER)
    {
        switch (*ptr)
        {
        case 0:
            SD_PowerOff(); /* Power Off */
            res = RES_OK;
            break;
        case 1:
            SD_PowerOn(); /* Power On */
            res = RES_OK;
            break;
        case 2:
            *(ptr + 1) = SD_CheckPower();
            res = RES_OK; /* Power Check */
            break;
        default:
            res = RES_PARERR;
        }
    }
    else
    {
        /* no disk */
        if (Stat & STA_NOINIT)
        {
            return RES_NOTRDY;
        }

        SelectSDCard();

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
            if (SD_ReadyWait() == 0xFF)
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

        DeselectSDCard();
    }

    return res;
}

/***************************************
 * SPI functions
 **************************************/

/* slave select */
static void SelectSDCard(void)
{
    // Select slave
    HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_RESET);
    // Then send a fill char onto MOSI
    uint8_t fill_char = SPI_FILL_CHAR;
    SpiWrite(&spi_sdcard_inst, &fill_char, 1u);
}

/* slave deselect */
static void DeselectSDCard(void)
{
    // Send a fill char onto MOSI
    uint8_t fill_char = SPI_FILL_CHAR;
    SpiWrite(&spi_sdcard_inst, &fill_char, 1u);
    // Then deselect slave
    HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_SET);
}

/***************************************
 * SD functions
 **************************************/

/* wait SD ready */
static uint8_t SD_ReadyWait(void)
{
    uint8_t result;

    /* timeout 500ms */
    Timer2 = 500;

    /* if SD goes ready, receives 0xFF */
    SpiRead(&spi_sdcard_inst, &result, 1u);
    while ((result != 0xFF) && Timer2)
    {
        SpiRead(&spi_sdcard_inst, &result, 1u);
    }

    return result;
}

/* power on */
static void SD_PowerOn(void)
{
    uint8_t args[6];
    uint8_t line_up_msg[10];
    uint8_t answer = 0;
    uint32_t counter = 0x1FFF;

    /* transmit bytes to wake up */
    DeselectSDCard();
    memset(&line_up_msg, SPI_FILL_CHAR, 10u);
    SpiWrite(&spi_sdcard_inst, (uint8_t *)&line_up_msg, 10u);

    /* slave select */
    SelectSDCard();

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

    DeselectSDCard();
    SpiWrite(&spi_sdcard_inst, (uint8_t *)&line_up_msg, 1u);

    PowerFlag = 1;
}

/* power off */
static void SD_PowerOff(void)
{
    PowerFlag = 0;
}

/* check power flag */
static uint8_t SD_CheckPower(void)
{
    return PowerFlag;
}

/* receive data block */
static BYTE SD_RxDataBlock(BYTE *buff, UINT len)
{
    uint8_t token;

    /* timeout 200ms */
    Timer1 = 200;

    /* loop until receive a response or timeout */
    SpiRead(&spi_sdcard_inst, &token, 1u);
    while ((token == 0xFF) && Timer1)
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

/* transmit data block */
static BYTE SD_TxDataBlock(const uint8_t *buff, BYTE token)
{
    uint8_t answer;
    uint8_t i = 0;

    /* wait SD ready */
    if (SD_ReadyWait() != 0xFF)
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

/* transmit command */
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
    if (SD_ReadyWait() != 0xFF)
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
