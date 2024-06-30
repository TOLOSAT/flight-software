/**
 * @file    spisd_driver.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for SD card SPI driver
 * @date    02/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "spi/spisd_driver.h"
#include "generic_hal.h"
#include "platform.h"

/***************************** Macros Definitions ****************************/

/* Definitions for MMC/SDC command */
#define CMD_MSG_SIZE                6u          /**< Command Size */
#define CMD_MSG_ANSWER_SIZE         4u          /**< Command Answer Size */
#define CMD0                        0x40u       /**< Command GO_IDLE_STATE */
#define CMD1                        0x41u       /**< Command SEND_OP_COND */
#define CMD8                        0x48u       /**< Command SEND_IF_COND */
#define CMD9                        0x49u       /**< Command SEND_CSD */
#define CMD10                       0x4au       /**< Command SEND_CID */
#define CMD12                       0x4cu       /**< Command STOP_TRANSMISSION */
#define CMD16                       0x50u       /**< Command SET_BLOCKLEN */
#define CMD17                       0x51u       /**< Command READ_SINGLE_BLOCK */
#define CMD18                       0x52u       /**< Command READ_MULTIPLE_BLOCK */
#define CMD23                       0x57u       /**< Command SET_BLOCK_COUNT */
#define CMD24                       0x58u       /**< Command WRITE_BLOCK */
#define CMD25                       0x59u       /**< Command WRITE_MULTIPLE_BLOCK */
#define CMD41                       0x69u       /**< Command SEND_OP_COND (ACMD) */
#define CMD55                       0x77u       /**< Command APP_CMD */
#define CMD58                       0x7au       /**< Command READ_OCR */
#define NULL_COMMAND_ARG            0x00000000u /**< Command argument filled with 0 */

/* Definition for MMC/SDC tokens */
#define SD_START_BLOCK_TOKEN        0xfeu        /**< Token notifying start of a 512 bits block */
#define SD_START_MULT_BLOCK_TOKEN   0xfcu        /**< Token notifying start of multiple 512 bits blocks */
#define SD_STOP_TOKEN               0xfdu        /**< Token stoping 512 bits block transaction */

/* Definition for MMC/SDC transmit data response */
#define SD_DATA_RESPONSE_MASK       0x1fu        /**< SPI data transmit data response mask */
#define SD_DATA_ACCEPTED            0x05u        /**< SPI data transmit data accepted */
#define SD_DATA_CRC_ERROR           0x0bu        /**< SPI data transmit data crc error */
#define SD_DATA_WRITE_ERROR         0x0du        /**< SPI data transmit data write */

/* SD Card constants */
#define SD_TIMEOUT                  30000u      /**< SD Card Timeout for ST HAL */   
#define SD_WAKEUP_MSG_SIZE          10u         /**< Wakeup message size*/
#define SD_BLOCK_SIZE               512u        /**< Card Block Size */
#define SD_INITIALIZATION_CONF      0x40000000u /**< SD card initialization configuration */
#define SD_CARD_INTERFACE_COND      0x000001aau /**< SD Card interface condition register (voltage setting and others) */
#define SD_CCS_BITMASK              0x40u       /**< Bitmask to access to CCS bit (Card Capacity status) if 1 then SD card is HC or XC */
#define SD_CS_PORT                  GPIOA       /**< GPIO Port of SD card CS Pin */
#define SD_CS_PIN                   GPIO_PIN_4  /**< GPIO Pin of SD card CS Pin */

/* SD card Status Flag */
#define SD_IDLE_FLAG                0x01u       /**< SD card IDLE flag position */
#define SD_ERASE_RST_FLAG           0x02u       /**< SD card ERASE RESET flag position */
#define SD_ILLEGAL_CMD_FLAG         0x04u       /**< SD card ILLEGAL COMMAND flag position */
#define SD_CRC_ERROR_FLAG           0x08u       /**< SD card CRC ERROR flag position */
#define SD_ERASE_ERROR_FLAG         0x10u       /**< SD card ERASE ERROR flag position */
#define SD_ADDR_ERROR_FLAG          0x20u       /**< SD card ADDR ERROR flag position */
#define SD_PARAM_ERROR_FLAG         0x40u       /**< SD card PARAM ERROR flag position */

/*************************** Functions Declarations **************************/

static fsStatus_t SpiSD_InitHw(void);
static fsStatus_t SpiSD_Select(void);
static fsStatus_t SpiSD_Unselect(void);
static fsStatus_t SpiSD_WaitUntilReady(void);
static fsStatus_t SpiSD_SwitchOn(void);
static fsStatus_t SpiSD_SwitchOff(void);
static fsStatus_t SpiSD_RxDataBlock(uint8_t *buff, uint32_t len);
static fsStatus_t SpiSD_TxDataBlock(const uint8_t *buff, uint32_t len, uint8_t token);
static fsStatus_t SpiSD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size);
static halStatus_t SpiSD_SendBytes(uint8_t *data, uint32_t size);
static halStatus_t SpiSD_ReceiveBytes(uint8_t *data, uint32_t size);
static uint8_t ComputeCommandCRC7(const uint8_t *cmd_msg);

/*************************** Variables Definitions ***************************/

static DSTATUS IN_FS_DATA_SECTION g_disk0_status = STA_NOINIT; /**< Disk0 Status */
SDCardStatus_t IN_FS_DATA_SECTION g_sd_card_status = SD_CARD_OFF; /**< Indicates if SD card is ON/OFF */
SDCardType_t IN_FS_DATA_SECTION g_sd_card_type = NOT_SDCARD;      /**< SD card type */

/**
 * @var     sd_card_gpio
 * @brief   GPIO for sd card (cs or card detect depend of the context) instance declaration
 */
static gpioInst_t IN_FS_DATA_SECTION sd_card_gpio = {
    .port = SD_GPIO_PORT,
    .pin = SD_GPIO_PIN,
    .mode = GPIO_MODE_OUTPUT_PP,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
    .irq_no = IRQ_NONE,
    .callback = NULL,
};

/**
 * @var     spi_sd_card_inst
 * @brief   SPI avionic instance declaration
 */
static spiInst_t IN_FS_DATA_SECTION spi_sd_card_inst = {
    .spi_ref = SPI_SD_CARD,
    .drive_type = SPI_POLLING_MASTER_DRIVE,
    .prescaler = SPI_BAUDRATEPRESCALER_8,
};

/*************************** Functions Definitions ***************************/

/**
 * @fn          SpiSD_GetStatus(uint8_t disk)
 * @brief       Function that gets status of the SD card
 * @param[in]   disk on from which we get the status
 * @return      DSTATUS 
 */
DSTATUS IN_FS_TEXT_SECTION SpiSD_GetStatus(uint8_t disk)
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
 * @fn          SpiSD_Init(uint8_t disk)
 * @brief       Function that initialises an SD card with SPI
 * @param[in]   disk Disk that will be initialised
 * @retval      #FS_INVALID_PARAM if disk does not exist
 * @retval      #FS_ERROR if initialisation failed
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t IN_FS_TEXT_SECTION SpiSD_Init(uint8_t disk)
{
    // Variables Initialization
    fsStatus_t return_value = FS_SUCCESSFUL;

    // First initialise SPI
    return_value = SpiSD_InitHw();
    
    if (return_value == FS_SUCCESSFUL)
    {
        // Single drive only, drv should be 0
        if (disk == DISK0_REF)
        {
            uint32_t tickstart = HAL_GetTick();
            // Switch on and select SD card
            fsStatus_t test_hal = SpiSD_SwitchOn();
            if (test_hal == FS_SUCCESSFUL)
            {
                // Select SD card (transaction begins)
                (void)SpiSD_Select();

                // Send Go Idle Command to start initialisation procedure
                test_hal = SpiSD_SendCmd(CMD0, NULL_COMMAND_ARG, NULL, 0u);
                if (test_hal == FS_SUCCESSFUL)
                {
                    // If CMD8 command is accept it is SDC V2 type, if not type is SDC V1
                    uint8_t interface_condition[CMD_MSG_ANSWER_SIZE] = {0};
                    test_hal = SpiSD_SendCmd(CMD8, SD_CARD_INTERFACE_COND, (uint8_t *)&interface_condition, 4u);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        // Type is SDC V2+
                        // Now check voltage set is effective
                        if ((interface_condition[2] == (uint8_t)((0x0000ff00u & SD_CARD_INTERFACE_COND) >> 8u)) && (interface_condition[3] == (uint8_t)(0x000000ffu & SD_CARD_INTERFACE_COND)))
                        {
                            // Activates SD card activation process until initialisation ended
                            test_hal = FS_BUSY;
                            while ((test_hal != FS_SUCCESSFUL) && ((HAL_GetTick() - tickstart) <  SD_TIMEOUT))
                            {
                                test_hal = SpiSD_SendCmd(CMD55, NULL_COMMAND_ARG, NULL, 0u);
                                if (test_hal == FS_SUCCESSFUL)
                                {
                                    // Sends host capacity support information and activates the card's initialization process. (HCS bit = 1 because we supports SDHC and SDXC)
                                    test_hal = SpiSD_SendCmd(CMD41, SD_INITIALIZATION_CONF, NULL, 0u);
                                }
                            }

                            // Check if initialisation wents well
                            if (test_hal == FS_SUCCESSFUL)
                            {
                                // Read Operation Control Register (OCR) and check CCS (card capacity status)
                                uint8_t ocr[CMD_MSG_ANSWER_SIZE] = {0};
                                test_hal = SpiSD_SendCmd(CMD58, NULL_COMMAND_ARG, (uint8_t *)&ocr, 4u);
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
                        test_hal = SpiSD_SendCmd(CMD55, NULL_COMMAND_ARG, NULL, 0);
                        if (test_hal == FS_SUCCESSFUL)
                        {
                            test_hal = SpiSD_SendCmd(CMD41, NULL_COMMAND_ARG, NULL, 0);
                            if (test_hal == FS_SUCCESSFUL)
                            {
                                // Set Block Lenght to 512 bits
                                test_hal = SpiSD_SendCmd(CMD16, SD_BLOCK_SIZE, NULL, 0u);
                                if (test_hal != FS_SUCCESSFUL)
                                {
                                    g_sd_card_type = SDCARD_V1;
                                }
                            }
                        }
                    }

                    // Unselect SD card (transaction ended)
                    (void)SpiSD_Unselect();

                    // Status No INIT flag
                    if (g_sd_card_type != NOT_SDCARD)
                    {
                        g_disk0_status &= ~STA_NOINIT;
                    }
                    else
                    {
                        // Initialization failed
                        (void)SpiSD_SwitchOff();
                    }
                }
                else
                {
                    // Switch on failed
                    (void)SpiSD_Unselect();
                    (void)SpiSD_SwitchOff();
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
    }

    return return_value;
}

/**
 * @fn          SpiSD_ReadBlocks(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that reads SD card blocks using SPI
 * @param[in]   disk Disk that is read
 * @param[out]  data Pointer to the data that will be read
 * @param[in]   addr Address of the data that will be read
 * @param[in]   len  Number of block that will be read
 * @retval      #FS_INVALID_PARAM if disk does not exist, len equal zero, pointer is null
 * @retval      #FS_BUSY if disk is not available
 * @retval      #FS_ERROR if an error occured
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t IN_FS_TEXT_SECTION SpiSD_ReadBlocks(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len)
{
    // Variables Initialization
    fsStatus_t return_value = FS_SUCCESSFUL;
    DWORD sector_address = addr;
    UINT sector_read = 0u;

    // Function Core
    if ((disk == DISK0_REF) && (len != 0u) && (data != NULL))
    {
        // Check if disk is ready
        if ((g_disk0_status & STA_NOINIT) == STA_NOINIT)
        {
            return_value = FS_BUSY;
        }
        else
        {
            // If not high capacity card convert sector number to byte address
            if (g_sd_card_type != SDCARD_V2HC)
            {
                sector_address *= SD_BLOCK_SIZE;
            }

            // Transaction begins, select SD card
            (void)SpiSD_Select();

            fsStatus_t test_val = FS_SUCCESSFUL;
            if (len == 1u)
            {
                /* READ_SINGLE_BLOCK */
                test_val = SpiSD_SendCmd(CMD17, sector_address, NULL, 0u);
                if (test_val == FS_SUCCESSFUL)
                {
                    test_val = SpiSD_RxDataBlock(data, SD_BLOCK_SIZE);
                    if (test_val == FS_SUCCESSFUL)
                    {
                        sector_read = len;
                    }
                }
            }
            else
            {
                /* READ_MULTIPLE_BLOCK */
                test_val = SpiSD_SendCmd(CMD18, sector_address, NULL, 0u);
                if (test_val == FS_SUCCESSFUL)
                {
                    while ((sector_read < len) && (test_val == FS_SUCCESSFUL))
                    {
                        test_val = SpiSD_RxDataBlock(&data[sector_read * SD_BLOCK_SIZE], SD_BLOCK_SIZE);
                        sector_read++;
                    }

                    /* STOP_TRANSMISSION */
                    test_val = SpiSD_SendCmd(CMD12, NULL_COMMAND_ARG, NULL, 0u);
                    if (test_val != FS_SUCCESSFUL)
                    {
                        sector_read = 0;
                    }
                }
            }

            // Transaction ended, unselect SD card
            (void)SpiSD_Unselect();

            // Check if we have read the right amount of sectors
            if (sector_read != len)
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
 * @fn          SpiSD_WriteBlocks(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that writes SD card blocks using SPI
 * @param[in]   disk Disk that is written
 * @param[in]   data Pointer to the data that will be written
 * @param[in]   addr Address of the data that will be written
 * @param[in]   len  Number of block that will be written
 * @retval      #FS_INVALID_PARAM if disk does not exist, len equal zero, pointer is null
 * @retval      #FS_BUSY if disk is not available
 * @retval      #FS_ERROR if an error occured or write is not permitted
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t IN_FS_TEXT_SECTION SpiSD_WriteBlocks(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len)
{
        // Variables Initialization
    fsStatus_t return_value = FS_SUCCESSFUL;
    DWORD sector_address = addr;
    UINT sector_written = 0u;

    // Function Core
    if ((disk == DISK0_REF) && (len != 0u) && (data != NULL))
    {
        // Check if disk is ready
        if ((g_disk0_status & STA_NOINIT) == STA_NOINIT)
        {
            return_value = FS_BUSY;
        }
        else
        {

            // Check if allowed to write
            if ((g_disk0_status & STA_PROTECT) == STA_PROTECT)
            {
                return_value = FS_ERROR;
            }
            else
            {
                // If not high capacity card convert sector number to byte address
                if (g_sd_card_type != SDCARD_V2HC)
                {
                    sector_address *= SD_BLOCK_SIZE;
                }

                // Transaction begins, select SD card
                (void)SpiSD_Select();

                fsStatus_t test_val = FS_SUCCESSFUL;
                if (len == 1u)
                {
                    /* WRITE_BLOCK */
                    test_val = SpiSD_SendCmd(CMD24, sector_address, NULL, 0u);
                    if (test_val == FS_SUCCESSFUL)
                    {
                        test_val = SpiSD_TxDataBlock(data, SD_BLOCK_SIZE, SD_START_BLOCK_TOKEN);
                        if (test_val == FS_SUCCESSFUL)
                        {
                            sector_written = len;
                        }
                    }
                }
                else
                {
                    /* WRITE_MULTIPLE_BLOCK */
                    if (g_sd_card_type == SDCARD_V1)
                    {
                        test_val = SpiSD_SendCmd(CMD55, NULL_COMMAND_ARG, NULL, 0u);
                        if (test_val == FS_SUCCESSFUL)
                        {
                            test_val = SpiSD_SendCmd(CMD23, len, NULL, 0u);
                            if (test_val == FS_SUCCESSFUL)
                            {
                                test_val = SpiSD_SendCmd(CMD25, sector_address, NULL, 0u);
                            }
                        }
                    }
                    else
                    {
                        test_val = SpiSD_SendCmd(CMD25, sector_address, NULL, 0u);
                    }

                    // Check if multiple block write init went well
                    if (test_val == FS_SUCCESSFUL)
                    {
                        while ((sector_written < len) && (test_val == FS_SUCCESSFUL))
                        {
                            test_val = SpiSD_TxDataBlock(&data[sector_written * SD_BLOCK_SIZE], SD_BLOCK_SIZE, SD_START_MULT_BLOCK_TOKEN);
                            sector_written++;
                        }

                        /* STOP_TRAN token */
                        test_val = SpiSD_TxDataBlock(NULL, 0u, SD_STOP_TOKEN);
                        if (test_val != FS_SUCCESSFUL)
                        {
                            sector_written = 0;
                        }
                    }
                }

                // Transaction ended, unselect SD card
                (void)SpiSD_Unselect();

                if (sector_written != len)
                {
                    return_value = FS_ERROR;
                }
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
 * @fn              SpiSD_Ioctl(uint8_t disk, uint8_t cmd, void *data)
 * @brief           Function that perfoms io control on the SD card (get info, change parameters ...)
 * @param[in]       disk Disk on which we perform the io control
 * @param[in]       cmd Which can of action is done on the SD card
 * @param[in,out]   data Data shared depending of command
 * @retval          #FS_INVALID_PARAM if the io control is not available for this device 
 * @retval          #FS_ERROR if an error occured 
 * @retval          #FS_SUCCESSFUL else 
 */
fsStatus_t IN_FS_TEXT_SECTION SpiSD_Ioctl(uint8_t disk, uint8_t cmd, void *data)
{
    // Variables Initialization
    fsStatus_t return_value = FS_SUCCESSFUL;
    fsStatus_t test_hal = FS_SUCCESSFUL;
    uint8_t *ptr = (uint8_t *)data; // cppcheck-suppress misra-c2012-11.5; I didnt find a solution yet
    uint8_t csd[16];
    WORD csize;

    /* disk should be 0 */
    if (disk == DISK0_REF)
    {
        return_value = FS_ERROR;

        if (cmd == CTRL_POWER)
        {
            switch (*ptr)
            {
            case 0:
                (void)SpiSD_SwitchOff();
                return_value = FS_SUCCESSFUL;
                break;
            case 1:
                test_hal = SpiSD_SwitchOn();
                if (test_hal == FS_SUCCESSFUL)
                {
                    return_value = FS_SUCCESSFUL;
                }
                else
                {
                    return_value = FS_ERROR;
                }

                break;
            case 2:
                ptr[1] = g_sd_card_status;
                return_value = FS_SUCCESSFUL;
                break;
            default:
                return_value = FS_INVALID_PARAM;
                break;
            }
        }
        else
        {
            // Check Disk Status
            DSTATUS status = SpiSD_GetStatus(disk);
            if ((status & STA_NOINIT) != STA_NOINIT)
            {
                (void)SpiSD_Select();

                switch (cmd)
                {
                case GET_SECTOR_COUNT:
                    test_hal = SpiSD_SendCmd(CMD9, NULL_COMMAND_ARG, NULL, 0u);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        test_hal = SpiSD_RxDataBlock(csd, 16u);
                        if (test_hal == FS_SUCCESSFUL)
                        {
                            if ((csd[0] >> 6) == 0x01u)
                            {
                                /* SDC V2 */
                                csize = csd[9] + ((WORD)csd[8] << 8) + 1;
                                *(DWORD *)data = (DWORD)csize << 10;
                            }
                            else
                            {
                                /* MMC or SDC V1 */
                                uint8_t n = (csd[5] & 0x0fu) + ((csd[10] & 0x80u) >> 7) + ((csd[9] & 0x03u) << 1) + 2u;
                                csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 0x03u) << 10) + 1u;
                                *(DWORD *)data = (DWORD)csize << (n - 9u);
                            }
                            return_value = FS_SUCCESSFUL;
                        }
                    }
                    break;
                case GET_SECTOR_SIZE:
                    *(WORD *)data = SD_BLOCK_SIZE;
                    return_value = FS_SUCCESSFUL;
                    break;
                case CTRL_SYNC:
                    test_hal = SpiSD_WaitUntilReady();
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        return_value = FS_SUCCESSFUL;
                    }
                    break;
                case MMC_GET_CSD:
                    test_hal = SpiSD_SendCmd(CMD9, NULL_COMMAND_ARG, NULL, 0u);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        test_hal = SpiSD_RxDataBlock(ptr, 16u);
                        if (test_hal == FS_SUCCESSFUL)
                        {
                            return_value = FS_SUCCESSFUL;
                        }
                    }
                    break;
                case MMC_GET_CID:
                    test_hal = SpiSD_SendCmd(CMD10, NULL_COMMAND_ARG, NULL, 0u);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        test_hal = SpiSD_RxDataBlock(ptr, 16u);
                        if (test_hal == FS_SUCCESSFUL)
                        {
                            return_value = FS_SUCCESSFUL;
                        }
                    }
                    break;
                case MMC_GET_OCR:
                    test_hal = SpiSD_SendCmd(CMD58, 0, ptr, 4u);
                    if (test_hal == FS_SUCCESSFUL)
                    {
                        return_value = FS_SUCCESSFUL;
                    }
                    break;
                default:
                    return_value = FS_INVALID_PARAM;
                    break;
                }

                (void)SpiSD_Unselect();
            }
            else
            {
                return_value = FS_BUSY;
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
 * @fn      SpiSD_InitHw(void)
 * @brief   Initialise SD Card HW
 * @retval  #FS_ERROR if SPI or GPIO are not initialised
 * @retval  #FS_SUCCESSFUL else
 */
static fsStatus_t SpiSD_InitHw(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t hal_status = GEN_HAL_SUCCESSFUL;

    // Function Core
    hal_status = SpiOpen(&spi_sd_card_inst);
    if (hal_status == GEN_HAL_SUCCESSFUL)
    {
        hal_status = GpioOpen(&sd_card_gpio);
        if (hal_status != GEN_HAL_SUCCESSFUL)
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
 * @fn      SpiSD_Select(void)
 * @brief   Select SD card on SPI bus
 * @retval  #FS_ERROR if SPI or GPIO error occured
 * @retval  #FS_SUCCESSFUL else
 */
static fsStatus_t IN_FS_TEXT_SECTION SpiSD_Select(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = GEN_HAL_SUCCESSFUL;

    // Select slave
    test_hal = GpioWrite(&sd_card_gpio, GPIO_PIN_RESET);
    if (test_hal == GEN_HAL_SUCCESSFUL)
    {
        // Then send a fill char onto MOSI
        uint8_t fill_char = SPI_FILL_CHAR;
        test_hal = SpiSD_SendBytes(&fill_char, 1u);
        if (test_hal != GEN_HAL_SUCCESSFUL)
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
 * @fn      SpiSD_Unselect(void)
 * @brief   Unselect SD card on SPI bus
 * @retval  #FS_ERROR if SPI or GPIO error occured
 * @retval  #FS_SUCCESSFUL else
 */
static fsStatus_t IN_FS_TEXT_SECTION SpiSD_Unselect(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = GEN_HAL_SUCCESSFUL;

    // Send a fill char onto MOSI
    uint8_t fill_char = SPI_FILL_CHAR;
    test_hal = SpiSD_SendBytes(&fill_char, 1u);
    if (test_hal == GEN_HAL_SUCCESSFUL)
    {
        // Then unselect slave
        test_hal = GpioWrite(&sd_card_gpio, GPIO_PIN_SET);
        if (test_hal != GEN_HAL_SUCCESSFUL)
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
 * @fn      SpiSD_WaitUntilReady(void)
 * @brief   Wait until SD card is ready
 * @retval  SPI_FILL_CHAR if SD card is ready
 * @retval  #FS_SUCCESSFUL if SD card is ready (spi slave register is now empty)
 * @retval  #FS_TIMEOUT if function timeouted before clearing SD card being ready
 * @retval  #FS_ERROR if SPI has encountered an error
 */
static fsStatus_t IN_FS_TEXT_SECTION SpiSD_WaitUntilReady(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = GEN_HAL_SUCCESSFUL;
    uint8_t answer = 0u;
    uint32_t tickstart = HAL_GetTick();

    // Read SD card until it returns SPI_FILL_CHAR or timeouted
    while ((test_hal == GEN_HAL_SUCCESSFUL) && (answer != SPI_FILL_CHAR) && ((HAL_GetTick() - tickstart) <  SD_TIMEOUT))
    {
        test_hal = SpiSD_ReceiveBytes(&answer, 1u);
    }

    if ((HAL_GetTick() - tickstart) >=  SD_TIMEOUT)
    {
        return_value = FS_TIMEOUT;
    }

    if (test_hal == GEN_HAL_ERROR)
    {
        return_value = FS_ERROR;
    }

    return return_value;
}

/**
 * @fn      SpiSD_SwitchOn(void)
 * @brief   Wake up the SD card an start initialize SPI mode
 * @retval  #FS_ERROR if SPI has encountered an error
 * @retval  #FS_TIMEOUT if SD card never answered IDLE state
 * @retval  #FS_SUCCESSFUL else
 */
static fsStatus_t IN_FS_TEXT_SECTION SpiSD_SwitchOn(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = GEN_HAL_SUCCESSFUL;
    uint8_t wakeup_message[SD_WAKEUP_MSG_SIZE];
    uint8_t answer = SPI_FILL_CHAR;
    uint32_t tickstart = HAL_GetTick();

    // Function Core
    // Wakeup SD card by sending pad caracter without selecting it
    (void)SpiSD_Unselect();
    (void)memset(&wakeup_message, SPI_FILL_CHAR, SD_WAKEUP_MSG_SIZE);
    test_hal = SpiSD_SendBytes((uint8_t *)&wakeup_message, SD_WAKEUP_MSG_SIZE);

    // Continue only if SPI has not encountered an error
    if (test_hal == GEN_HAL_SUCCESSFUL)
    {
        uint8_t reset_spi_mode_cmd[CMD_MSG_SIZE] = {CMD0, 0x00u, 0x00u, 0x00u, 0x00u, 0x95u};

        // Select SD card
        (void)SpiSD_Select();

        // Send reset onto spi mode command
        test_hal = SpiSD_SendBytes((uint8_t *)reset_spi_mode_cmd, CMD_MSG_SIZE);

        // Continue only if SPI has not encountered an error
        if (test_hal == GEN_HAL_SUCCESSFUL)
        {
            // Wait until SD card
            while ((test_hal == GEN_HAL_SUCCESSFUL) && (answer != SD_IDLE_FLAG) && ((HAL_GetTick() - tickstart) <  SD_TIMEOUT))
            {
                test_hal = SpiSD_ReceiveBytes(&answer, 1u);
            }

            // Unselect SD card
            (void)SpiSD_Unselect();

            // Test if procedure wents well
            if ((test_hal == GEN_HAL_SUCCESSFUL) && ((HAL_GetTick() - tickstart) <  SD_TIMEOUT))
            {
                g_sd_card_status = SD_CARD_ON;
            }
            else
            {
                g_sd_card_status = SD_CARD_OFF;
                if ((HAL_GetTick() - tickstart) >=  SD_TIMEOUT)
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
 * @fn      SpiSD_SwitchOff(void)
 * @brief   Switch off the SD card
 * @retval  #FS_SUCCESSFUL always
 */
static fsStatus_t IN_FS_TEXT_SECTION SpiSD_SwitchOff(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;

    // Function Core
    g_sd_card_status = SD_CARD_OFF;

    return return_value;
}

/**
 * @fn          SpiSD_RxDataBlock(uint8_t *buff, uint32_t len)
 * @brief       Receives a block from SD card
 * @param[out]  buff Buffer containing the block received
 * @param[in]   len Length of the block
 * @retval      #FS_INVALID_PARAM if buff is null pointer or len is null
 * @retval      #FS_ERROR if SPI has encountered an error
 * @retval      #FS_SUCCESSFUL else
 */
static fsStatus_t IN_FS_TEXT_SECTION SpiSD_RxDataBlock(uint8_t *buff, uint32_t len)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = GEN_HAL_SUCCESSFUL;
    uint8_t token = SPI_FILL_CHAR;

    // Function Core
    if ((buff != NULL) && (len != 0u))
    {
        // Loop until receive a response or timeout
        uint32_t tickstart = HAL_GetTick();
        while ((test_hal == GEN_HAL_SUCCESSFUL) && (token == SPI_FILL_CHAR) && ((HAL_GetTick() - tickstart) <  SD_TIMEOUT))
        {
            test_hal = SpiSD_ReceiveBytes(&token, 1u);
        }

        // Check if read was successful and gets a start block token
        if ((token == SD_START_BLOCK_TOKEN) && (test_hal == GEN_HAL_SUCCESSFUL) && ((HAL_GetTick() - tickstart) <  SD_TIMEOUT))
        {
            // Receive block
            test_hal = SpiSD_ReceiveBytes(buff, len);

            // Check if block has corretly been read
            if (test_hal == GEN_HAL_SUCCESSFUL)
            {
                // Receive (and discard CRC)
                uint8_t crc[2] = {0};
                test_hal = SpiSD_ReceiveBytes((uint8_t *)&crc, 2u);
                // Check if crc has corretly been read
                if (test_hal != GEN_HAL_SUCCESSFUL)
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
 * @fn          SpiSD_TxDataBlock(const uint8_t *buff, uint32_t len, uint8_t token)
 * @brief       Sends a block from SD card
 * @param[in]   buff Buffer containing the block to send
 * @param[in]   len Length of the block
 * @param[in]   token Token indicating type of transmission
 * @retval      #FS_INVALID_PARAM if buff is null pointer or len is null except if token is SD_STOP_TOKEN
 * @retval      #FS_ERROR if SPI has encountered an error
 * @retval      #FS_SUCCESSFUL else
 */
static fsStatus_t IN_FS_TEXT_SECTION SpiSD_TxDataBlock(const uint8_t *buff, uint32_t len, uint8_t token)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;

    // Function Core
    if ((len != 0u) && (buff == NULL) && ((token == SD_STOP_TOKEN)))
    {
        return_value = FS_INVALID_PARAM;
    }
    else
    {
        uint32_t tickstart = HAL_GetTick();
        // Wait until SD card is ready
        fsStatus_t test_wait = FS_SUCCESSFUL;
        test_wait = SpiSD_WaitUntilReady();
        if (test_wait == FS_SUCCESSFUL)
        {
            // Send token
            halStatus_t test_hal = GEN_HAL_SUCCESSFUL;
            test_hal = SpiSD_SendBytes(&token, 1u);
            if (test_hal == GEN_HAL_SUCCESSFUL)
            {
                // if it's not STOP token, transmit data
                if (token != SD_STOP_TOKEN)
                {
                    test_hal = SpiSD_SendBytes((uint8_t *)buff, len); // cppcheck-suppress misra-c2012-11.8; Low-level drivers don't use the const argument so it has to disappear somewhere 
                    if (test_hal == GEN_HAL_SUCCESSFUL)
                    {
                        // Read and discard CRC
                        uint8_t crc[2] = {0};
                        test_hal = SpiSD_ReceiveBytes((uint8_t *)&crc, 2u);
                        if (test_hal == GEN_HAL_SUCCESSFUL)
                        {
                            uint8_t answer = SPI_FILL_CHAR;
                            while ((test_hal == GEN_HAL_SUCCESSFUL) && (answer == SPI_FILL_CHAR) && ((HAL_GetTick() - tickstart) <  SD_TIMEOUT))
                            {
                                test_hal = SpiSD_ReceiveBytes(&answer, 1u);
                            }
                            // Check if we get the answer
                            if ((test_hal == GEN_HAL_SUCCESSFUL) && (answer != SPI_FILL_CHAR) && ((HAL_GetTick() - tickstart) <  SD_TIMEOUT))
                            {
                                // Clear receive buffer until fill char is received
                                test_wait = SpiSD_WaitUntilReady();
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
 * @fn          SpiSD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size)
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
static fsStatus_t IN_FS_TEXT_SECTION SpiSD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size)
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
        uint32_t tickstart = HAL_GetTick();
        if ((cmd >= 0x40u) && (cmd <= 0x7fu))
        {
            // Wait until transfer complete
            test_wait = SpiSD_WaitUntilReady();
            if (test_wait == FS_SUCCESSFUL)
            {
                uint8_t cmd_msg[CMD_MSG_SIZE] = {0};
                // Build command message with function arguments
                cmd_msg[0] = (uint8_t)(cmd);
                cmd_msg[1] = (uint8_t)((0xff000000u & arg) >> 24u);
                cmd_msg[2] = (uint8_t)((0x00ff0000u & arg) >> 16u);
                cmd_msg[3] = (uint8_t)((0x0000ff00u & arg) >> 8u);
                cmd_msg[4] = (uint8_t)(0x000000ffu & arg);
                cmd_msg[5] = ComputeCommandCRC7((uint8_t *)&cmd_msg);

                // Send Command
                test_hal = SpiSD_SendBytes((uint8_t *)&cmd_msg, CMD_MSG_SIZE);
                if (test_hal == GEN_HAL_SUCCESSFUL)
                {
                    uint8_t command_status = SPI_FILL_CHAR;
                    while ((command_status == SPI_FILL_CHAR) && (test_hal == GEN_HAL_SUCCESSFUL) && ((HAL_GetTick() - tickstart) <  SD_TIMEOUT))
                    {
                        test_hal = SpiSD_ReceiveBytes(&command_status, 1u);
                    }

                    // Check Result
                    if ((test_hal == GEN_HAL_SUCCESSFUL) && (command_status <= SD_IDLE_FLAG))
                    {
                        if ((cmd == CMD41) && (command_status != 0u))
                        {
                            return_value = FS_BUSY;
                        }
                        else
                        {
                            // If command is CMD12 (STOP_TRANSMISSION) wait until ready
                            if (cmd == CMD12)
                            {
                                test_wait = SpiSD_WaitUntilReady();
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
                                    test_hal = SpiSD_ReceiveBytes(answer, answer_size);

                                    // Check if everything wents well
                                    if (test_hal != GEN_HAL_SUCCESSFUL)
                                    {
                                        return_value = FS_ERROR;
                                    }
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

/**
 * @fn          SpiSD_SendBytes(uint8_t *data, uint32_t size)
 * @brief       Send bytes to SD card
 * @param[in]   data Data sended to SD card
 * @param[in]   size Data size in bytes
 * @return      Status of SpiWrite function
 */
static halStatus_t IN_FS_TEXT_SECTION SpiSD_SendBytes(uint8_t *data, uint32_t size)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;
    uint32_t i = 0u;

    // Function Core
    while ((return_value == GEN_HAL_SUCCESSFUL) && (i < size))
    {
        return_value = SpiWrite(&spi_sd_card_inst, &data[i], 1u);
        i++;
    }

    return return_value;
}

/**
 * @fn          SpiSD_ReceiveBytes(uint8_t *data, uint32_t size)
 * @brief       Receive bytes to SD card
 * @param[out]  data Data received from SD card
 * @param[in]   size Data size in bytes
 * @return      Status of SpiRead function
 */
static halStatus_t IN_FS_TEXT_SECTION SpiSD_ReceiveBytes(uint8_t *data, uint32_t size)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;
    uint8_t fill_char = SPI_FILL_CHAR;
    uint32_t i = 0u;

    // Function Core
    while ((return_value == GEN_HAL_SUCCESSFUL) && (i < size))
    {
        return_value = SpiRead(&spi_sd_card_inst, &data[i], &fill_char, 1u);
        i++;
    }

    return return_value;
}

/**
 * @fn          ComputeCommandCRC7(const uint8_t *cmd_msg)
 * @brief       Computes CRC7 for sd command message and add 1 as byte's LSB
 * @param[in]   cmd_msg Command message for which crc is calculated 
 * @return      CRC7 for bits 7 to 1 and 0b1 for bit 0 
 */
static uint8_t IN_FS_TEXT_SECTION ComputeCommandCRC7(const uint8_t *cmd_msg)
{
    // Variable Initialisation
    const uint8_t g_sd_crc7_lookup_table[256] = 
    {
        0x00, 0x12, 0x24, 0x36, 0x48, 0x5a, 0x6c, 0x7e,
        0x90, 0x82, 0xb4, 0xa6, 0xd8, 0xca, 0xfc, 0xee,
        0x32, 0x20, 0x16, 0x04, 0x7a, 0x68, 0x5e, 0x4c,
        0xa2, 0xb0, 0x86, 0x94, 0xea, 0xf8, 0xce, 0xdc,
        0x64, 0x76, 0x40, 0x52, 0x2c, 0x3e, 0x08, 0x1a,
        0xf4, 0xe6, 0xd0, 0xc2, 0xbc, 0xae, 0x98, 0x8a,
        0x56, 0x44, 0x72, 0x60, 0x1e, 0x0c, 0x3a, 0x28,
        0xc6, 0xd4, 0xe2, 0xf0, 0x8e, 0x9c, 0xaa, 0xb8,
        0xc8, 0xda, 0xec, 0xfe, 0x80, 0x92, 0xa4, 0xb6,
        0x58, 0x4a, 0x7c, 0x6e, 0x10, 0x02, 0x34, 0x26,
        0xfa, 0xe8, 0xde, 0xcc, 0xb2, 0xa0, 0x96, 0x84,
        0x6a, 0x78, 0x4e, 0x5c, 0x22, 0x30, 0x06, 0x14,
        0xac, 0xbe, 0x88, 0x9a, 0xe4, 0xf6, 0xc0, 0xd2,
        0x3c, 0x2e, 0x18, 0x0a, 0x74, 0x66, 0x50, 0x42,
        0x9e, 0x8c, 0xba, 0xa8, 0xd6, 0xc4, 0xf2, 0xe0,
        0x0e, 0x1c, 0x2a, 0x38, 0x46, 0x54, 0x62, 0x70,
        0x82, 0x90, 0xa6, 0xb4, 0xca, 0xd8, 0xee, 0xfc,
        0x12, 0x00, 0x36, 0x24, 0x5a, 0x48, 0x7e, 0x6c,
        0xb0, 0xa2, 0x94, 0x86, 0xf8, 0xea, 0xdc, 0xce,
        0x20, 0x32, 0x04, 0x16, 0x68, 0x7a, 0x4c, 0x5e,
        0xe6, 0xf4, 0xc2, 0xd0, 0xae, 0xbc, 0x8a, 0x98,
        0x76, 0x64, 0x52, 0x40, 0x3e, 0x2c, 0x1a, 0x08,
        0xd4, 0xc6, 0xf0, 0xe2, 0x9c, 0x8e, 0xb8, 0xaa,
        0x44, 0x56, 0x60, 0x72, 0x0c, 0x1e, 0x28, 0x3a,
        0x4a, 0x58, 0x6e, 0x7c, 0x02, 0x10, 0x26, 0x34,
        0xda, 0xc8, 0xfe, 0xec, 0x92, 0x80, 0xb6, 0xa4,
        0x78, 0x6a, 0x5c, 0x4e, 0x30, 0x22, 0x14, 0x06,
        0xe8, 0xfa, 0xcc, 0xde, 0xa0, 0xb2, 0x84, 0x96,
        0x2e, 0x3c, 0x0a, 0x18, 0x66, 0x74, 0x42, 0x50,
        0xbe, 0xac, 0x9a, 0x88, 0xf6, 0xe4, 0xd2, 0xc0,
        0x1c, 0x0e, 0x38, 0x2a, 0x54, 0x46, 0x70, 0x62,
        0x8c, 0x9e, 0xa8, 0xba, 0xc4, 0xd6, 0xe0, 0xf2
    };
    uint8_t crc = 0u;

    // Function Core
    if (cmd_msg != NULL)
    {
        for (uint32_t i = 0u; i < (CMD_MSG_SIZE - 1u); i++)
        {
            crc = g_sd_crc7_lookup_table[crc ^ cmd_msg[i]];
        }

        // Set 1 on the LSB
        crc |= 1u;
    }

    return crc;
}