/**
 * @file    drv_spisd.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for SD card memory over SPI bus
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "drv/memories.h"
#include "drv/memories/drv_spisd.h"
#include "core/irq.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/* Definitions for MMC/SDC command */
#define SPISD_CMD_MSG_SIZE           6u          /**< Command Size */
#define SPISD_CMD_MSG_ANSWER_SIZE    4u          /**< Command Answer Size */
#define SPISD_CMD0                   0x40u       /**< Command GO_IDLE_STATE */
#define SPISD_CMD1                   0x41u       /**< Command SEND_OP_COND */
#define SPISD_CMD8                   0x48u       /**< Command SEND_IF_COND */
#define SPISD_CMD9                   0x49u       /**< Command SEND_CSD */
#define SPISD_CMD10                  0x4au       /**< Command SEND_CID */
#define SPISD_CMD12                  0x4cu       /**< Command STOP_TRANSMISSION */
#define SPISD_CMD16                  0x50u       /**< Command SET_BLOCKLEN */
#define SPISD_CMD17                  0x51u       /**< Command READ_SINGLE_BLOCK */
#define SPISD_CMD18                  0x52u       /**< Command READ_MULTIPLE_BLOCK */
#define SPISD_CMD23                  0x57u       /**< Command SET_BLOCK_COUNT */
#define SPISD_CMD24                  0x58u       /**< Command WRITE_BLOCK */
#define SPISD_CMD25                  0x59u       /**< Command WRITE_MULTIPLE_BLOCK */
#define SPISD_CMD41                  0x69u       /**< Command SEND_OP_COND (ACMD) */
#define SPISD_CMD55                  0x77u       /**< Command APP_CMD */
#define SPISD_CMD58                  0x7au       /**< Command READ_OCR */
#define SPISD_NULL_COMMAND_ARG       0x00000000u /**< Command argument filled with 0 */

/* Definition for MMC/SDC tokens */
#define SPISD_START_BLOCK_TOKEN      0xfeu /**< Token notifying start of a 512 bits block */
#define SPISD_START_MULT_BLOCK_TOKEN 0xfcu /**< Token notifying start of multiple 512 bits blocks */
#define SPISD_STOP_TOKEN             0xfdu /**< Token stoping 512 bits block transaction */

/* Definition for MMC/SDC transmit data response */
#define SPISD_DATA_RESPONSE_MASK     0x1fu /**< SPI data transmit data response mask */
#define SPISD_DATA_ACCEPTED          0x05u /**< SPI data transmit data accepted */
#define SPISD_DATA_CRC_ERROR         0x0bu /**< SPI data transmit data crc error */
#define SPISD_DATA_WRITE_ERROR       0x0du /**< SPI data transmit data write */

/* SD Card constants */
#define SPISD_TIMEOUT                3000u       /**< SPISD Card Timeout for ST HAL */
#define SPISD_BLOCK_SIZE_IN_SECTORS  1u          /**< Erase block size in sectors (fixed to 1 for SPISD) */
#define SPISD_WAKEUP_MSG_SIZE        10u         /**< Wakeup message size*/
#define SPISD_SECTOR_SIZE            512u        /**< Card Sector Size */
#define SPISD_INITIALIZATION_CONF    0x40000000u /**< SD card initialization configuration */
#define SPISD_CARD_INTERFACE_COND    0x000001aau /**< SD Card interface condition register (voltage setting and others) */
#define SPISD_CCS_BITMASK            0x40u       /**< Bitmask to access to CCS bit (Card Capacity status) if 1 then SD card is HC or XC */

/* SD card Status Flag */
#define SPISD_IDLE_FLAG              0x01u /**< SD card IDLE flag position */
#define SPISD_ERASE_RST_FLAG         0x02u /**< SD card ERASE RESET flag position */
#define SPISD_ILLEGAL_CMD_FLAG       0x04u /**< SD card ILLEGAL COMMAND flag position */
#define SPISD_CRC_ERROR_FLAG         0x08u /**< SD card CRC ERROR flag position */
#define SPISD_ERASE_ERROR_FLAG       0x10u /**< SD card ERASE ERROR flag position */
#define SPISD_ADDR_ERROR_FLAG        0x20u /**< SD card ADDR ERROR flag position */
#define SPISD_PARAM_ERROR_FLAG       0x40u /**< SD card PARAM ERROR flag position */

/*************************** Functions Declarations **************************/

static void SpisdGenericIRQHandler(void *param);
static returnCode_t SpisdInitClock(spisdInst_t *spisd_inst, const spisdConf_t *const spisd_conf);
static returnCode_t SpisdDeInitClock(spisdInst_t *spisd_inst);
static returnCode_t SpisdSetupIOs(spisdInst_t *spisd_inst, const spisdConf_t *const spisd_conf);
static returnCode_t SpiSD_Select(spisdInst_t *spisd_inst);
static returnCode_t SpiSD_Unselect(spisdInst_t *spisd_inst);
static returnCode_t SpiSD_WaitUntilReady(spisdInst_t *spisd_inst);
static returnCode_t SpiSD_Wakeup(spisdInst_t *spisd_inst);
static returnCode_t SpiSD_SelectSpiMode(spisdInst_t *spisd_inst);
static returnCode_t SpiSD_InitSDCard(spisdInst_t *spisd_inst);
static returnCode_t SpiSD_ReadData(spisdInst_t *spisd_inst, data_t data, length_t length);
static returnCode_t SpiSD_WriteData(spisdInst_t *spisd_inst, data_t data, length_t length, uint8_t token);
static returnCode_t SpiSD_SendCmd(spisdInst_t *spisd_inst, uint8_t cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size);
static uint8_t ComputeCommandCRC7(const uint8_t *cmd_msg);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              SpisdOpen(spisdInst_t *spisd_inst, const spisdConf_t *const spisd_conf)
 * @brief           Function that initialise a SPISD memory
 * @param[in,out]   spisd_inst   Instance that contains SPISD handlers
 * @param[in]       spisd_conf   Configuration that contains SPISD parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 */
returnCode_t SpisdOpen(spisdInst_t *spisd_inst, const spisdConf_t *const spisd_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spisd_inst != NULL) && (spisd_conf != NULL))
    {
        // Init peripheral clock
        return_value = SpisdInitClock(spisd_inst, spisd_conf);
        if (return_value == RET_SUCCESSFUL)
        {
            // Setup IOs
            return_value = SpisdSetupIOs(spisd_inst, spisd_conf);
            if (return_value == RET_SUCCESSFUL)
            {
                // Setup SPI
                spisd_inst->spi_handle_struct.Instance               = spisd_conf->spi_periph;
                spisd_inst->spi_handle_struct.Init.BaudRatePrescaler = spisd_conf->prescaler;
                spisd_inst->spi_handle_struct.Init.Mode              = SPI_MODE_MASTER;
                spisd_inst->spi_handle_struct.Init.Direction         = SPI_DIRECTION_2LINES;
                spisd_inst->spi_handle_struct.Init.DataSize          = SPI_DATASIZE_8BIT;
                spisd_inst->spi_handle_struct.Init.CLKPolarity       = SPI_POLARITY_LOW;
                spisd_inst->spi_handle_struct.Init.CLKPhase          = SPI_PHASE_1EDGE;
                spisd_inst->spi_handle_struct.Init.NSS               = SPI_NSS_SOFT;
                spisd_inst->spi_handle_struct.Init.FirstBit          = SPI_FIRSTBIT_MSB;
                spisd_inst->spi_handle_struct.Init.TIMode            = SPI_TIMODE_DISABLE;
                spisd_inst->spi_handle_struct.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
                spisd_inst->spi_handle_struct.Init.CRCPolynomial     = 0x0;
#if defined(STM32H7)
                spisd_inst->spi_handle_struct.Init.NSSPMode                   = SPI_NSS_PULSE_DISABLE;
                spisd_inst->spi_handle_struct.Init.NSSPolarity                = SPI_NSS_POLARITY_LOW;
                spisd_inst->spi_handle_struct.Init.FifoThreshold              = SPI_FIFO_THRESHOLD_01DATA;
                spisd_inst->spi_handle_struct.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
                spisd_inst->spi_handle_struct.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
                spisd_inst->spi_handle_struct.Init.MasterSSIdleness           = SPI_MASTER_SS_IDLENESS_00CYCLE;
                spisd_inst->spi_handle_struct.Init.MasterInterDataIdleness    = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
                spisd_inst->spi_handle_struct.Init.MasterReceiverAutoSusp     = SPI_MASTER_RX_AUTOSUSP_DISABLE;
                spisd_inst->spi_handle_struct.Init.MasterKeepIOState          = SPI_MASTER_KEEP_IO_STATE_DISABLE;
                spisd_inst->spi_handle_struct.Init.IOSwap                     = SPI_IO_SWAP_DISABLE;
#endif

                HAL_StatusTypeDef test_val = HAL_SPI_Init(&spisd_inst->spi_handle_struct);
                if (test_val == HAL_OK)
                {
                    // Link the conf pointer
                    spisd_inst->p_conf = spisd_conf;
                    // Link CS GPIO port and GPIO pin
                    spisd_inst->cs_port = spisd_conf->io_cs.port;
                    spisd_inst->cs_pin  = spisd_conf->io_cs.pin;
                    // Set sd inst as the interrupt parameter to pass it to the interrupt routine
                    IRQHandlerParam_t param = (IRQHandlerParam_t)&spisd_inst->spi_handle_struct;
                    // Request the interrupt
                    return_value = RequestIRQ(spisd_conf->irq_no, spisd_conf->irq_prio, SpisdGenericIRQHandler, param);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        // First wakeup the SD card
                        return_value = SpiSD_Wakeup(spisd_inst);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // Then setup SD card SPI Mode
                            return_value = SpiSD_SelectSpiMode(spisd_inst);
                            if (return_value == RET_SUCCESSFUL)
                            {
                                // Finaly do the SD card initialisation procedure
                                return_value = SpiSD_InitSDCard(spisd_inst);
                            }
                        }
                    }
                }
                else
                {
                    KernelPanic();
                }
            }
            else
            {
                KernelPanic();
            }
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SpisdWrite(spisdInst_t *spisd_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto an SPISD memory
 * @param[in]   spisd_inst     Instance that contains SPISD parameters and SPISD Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer from which data will be copied
 * @param[in]   length      Number of block that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if spisd timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if spisd is still sending previous message
 */
returnCode_t SpisdWrite(spisdInst_t *spisd_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spisd_inst != NULL) && (length != 0u) && (data != NULL) && (spisd_inst->sd_type != NOT_SDCARD))
    {
        uint32_t sector_address = sector;
        // If not high capacity card convert sector number to byte address
        if (spisd_inst->sd_type != SDCARD_V2HC)
        {
            sector_address *= SPISD_SECTOR_SIZE;
        }

        // Transaction begins, select SD card
        return_value = SpiSD_Select(spisd_inst);
        if (return_value == RET_SUCCESSFUL)
        {
            // Depending on the number of sector to read the command will be different
            if (length == 1u)
            {
                // Single sector
                return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD24, sector_address, NULL, 0u);
                if (return_value == RET_SUCCESSFUL)
                {
                    return_value = SpiSD_WriteData(spisd_inst, data, SPISD_SECTOR_SIZE, SPISD_START_BLOCK_TOKEN);
                }
            }
            else
            {
                // Read multiple sectors
                // Send command to start a multiple sector read depending on the card type
                if (spisd_inst->sd_type == SDCARD_V1)
                {
                    return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD55, SPISD_NULL_COMMAND_ARG, NULL, 0u);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD23, length, NULL, 0u);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD25, sector_address, NULL, 0u);
                        }
                    }
                }
                else
                {
                    return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD25, sector_address, NULL, 0u);
                }

                // Check if multiple block write init went well
                if (return_value == RET_SUCCESSFUL)
                {
                    length_t nb_sector_written = 0u;
                    while ((nb_sector_written < length) && (return_value == RET_SUCCESSFUL))
                    {
                        return_value = SpiSD_WriteData(spisd_inst, &data[nb_sector_written * SPISD_SECTOR_SIZE], SPISD_SECTOR_SIZE,
                                                       SPISD_START_MULT_BLOCK_TOKEN);
                        nb_sector_written++;
                    }

                    // Stop the transmission if everything went right
                    if ((return_value == RET_SUCCESSFUL) && (nb_sector_written == length))
                    {
                        return_value = SpiSD_WriteData(spisd_inst, NULL, 0u, SPISD_STOP_TOKEN);
                    }
                }
            }
            // Unselect SD card anyway
            if (SpiSD_Unselect(spisd_inst) != RET_SUCCESSFUL)
            {
                KernelPanic();
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SpisdRead(spisdInst_t *spisd_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an SPISD memory
 * @param[in]   spisd_inst     Instance that contains SPISD parameters and SPISD Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of block that will be read
 * @retval      #RET_SUCCESSFUL if data has been read successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if spisd timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if spisd is still sending previous message
 */
returnCode_t SpisdRead(spisdInst_t *spisd_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spisd_inst != NULL) && (length != 0u) && (data != NULL) && (spisd_inst->sd_type != NOT_SDCARD))
    {
        uint32_t sector_address = sector;
        // If not high capacity card convert sector number to byte address
        if (spisd_inst->sd_type != SDCARD_V2HC)
        {
            sector_address *= SPISD_SECTOR_SIZE;
        }

        // Transaction begins, select SD card
        return_value = SpiSD_Select(spisd_inst);
        if (return_value == RET_SUCCESSFUL)
        {
            // Depending on the number of sector to read the command will be different
            if (length == 1u)
            {
                // Single sector
                return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD17, sector_address, NULL, 0u);
                if (return_value == RET_SUCCESSFUL)
                {
                    return_value = SpiSD_ReadData(spisd_inst, data, SPISD_SECTOR_SIZE);
                }
            }
            else
            {
                // Read multiple sectors
                return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD18, sector_address, NULL, 0u);
                if (return_value == RET_SUCCESSFUL)
                {
                    length_t nb_sector_read = 0u;
                    while ((nb_sector_read < length) && (return_value == RET_SUCCESSFUL))
                    {
                        return_value = SpiSD_ReadData(spisd_inst, &data[nb_sector_read * SPISD_SECTOR_SIZE], SPISD_SECTOR_SIZE);
                        nb_sector_read++;
                    }

                    // Stop the transmission if everything went right
                    if ((return_value == RET_SUCCESSFUL) && (nb_sector_read == length))
                    {
                        return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD12, SPISD_NULL_COMMAND_ARG, NULL, 0u);
                    }
                }
            }

            // Unselect SD card anyway
            if (SpiSD_Unselect(spisd_inst) != RET_SUCCESSFUL)
            {
                KernelPanic();
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpisdIoctl(spisdInst_t *spisd_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   spisd_inst     Instance that contains SPISD handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t SpisdIoctl(spisdInst_t *spisd_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spisd_inst != NULL) && (spisd_inst->sd_type != NOT_SDCARD))
    {
        switch (cmd)
        {
            case IOCTL_MEMORY_GET_STATUS :
                if (data_size == sizeof(memoryStatus_t))
                {
                    if (spisd_inst->sd_type != NOT_SDCARD)
                    {
                        *(memoryStatus_t *)data = MEMORY_READY;
                    }
                    else
                    {
                        *(memoryStatus_t *)data = MEMORY_NO_DISK;
                    }
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_SYNC :
                // Sync is not required for this SPISD card driver, so do nothing
                break;
            case IOCTL_MEMORY_GET_SECTOR_COUNT :
                if (data_size == sizeof(memorySectorCount_t))
                {
                    return_value = SpiSD_Select(spisd_inst);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        uint8_t csd[16];
                        // Get the Card Specific Data registers
                        return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD9, SPISD_NULL_COMMAND_ARG, NULL, 0u);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            return_value = SpiSD_ReadData(spisd_inst, csd, 16u);
                            if (return_value == RET_SUCCESSFUL)
                            {
                                // Depending on the SD card type
                                if ((csd[0] >> 6) == 0x01u)
                                {
                                    // SDC V2
                                    uint16_t csize = csd[9] + ((uint16_t)csd[8] << 8) + 1u;

                                    *(memorySectorCount_t *)data = (memorySectorCount_t)csize << 10;
                                }
                                else
                                {
                                    /* MMC or SDC V1 */
                                    uint8_t n      = ((csd[5] & 0x0fu) + ((csd[10] & 0x80u) >> 7) + ((csd[9] & 0x03u) << 1) + 2u) - 9u;
                                    uint16_t csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 0x03u) << 10) + 1u;

                                    *(memorySectorCount_t *)data = (memorySectorCount_t)csize << n;
                                }
                            }
                        }
                    }

                    // Unselect SD card anyway
                    if (SpiSD_Unselect(spisd_inst) != RET_SUCCESSFUL)
                    {
                        KernelPanic();
                    }
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_GET_SECTOR_SIZE :
                if (data_size == sizeof(memorySectorSize_t))
                {
                    *(memorySectorSize_t *)data = SPISD_SECTOR_SIZE;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_GET_BLOCK_SIZE :
                if (data_size == sizeof(memoryBlockSize_t))
                {
                    *(memoryBlockSize_t *)data = SPISD_BLOCK_SIZE_IN_SECTORS;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_ERASE_BLOCK :
                // TO DO : to be implemented
                return_value = RET_INVALID_PARAM;
                break;

            default :
                return_value = RET_INVALID_PARAM;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpisdClose(spisdInst_t *spisd_inst)
 * @brief           Function that desinit the SPISD connection
 * @param[in,out]   spisd_inst   Instance that contains SPISD handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
returnCode_t SpisdClose(spisdInst_t *spisd_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spisd_inst != NULL)
    {
        HAL_SPI_DeInit(&spisd_inst->spi_handle_struct);
        (void)SpisdDeInitClock(spisd_inst);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpisdInitClock(spisdInst_t *spisd_inst, const spisdConf_t *const spisd_conf)
 * @brief           Function that setups SPISD peripheral clock
 * @param[in,out]   spisd_inst   Instance that contains SPI handlers
 * @param[in]       spi_conf   Configuration that contains SPI parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t SpisdInitClock(spisdInst_t *spisd_inst, const spisdConf_t *const spisd_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(spisd_inst);

    // Check parameter(s)
    if ((spisd_inst != NULL) && (spisd_conf != NULL))
    {
        // Select the peripheral clock
        switch ((uintptr_t)spisd_conf->spi_periph)
        {
            case SPI1_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef spi_peripheral_clock_settings = { 0 };
                spi_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SPI1;
                spi_peripheral_clock_settings.Spi123ClockSelection     = spisd_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&spi_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SPI1_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SPI1_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#if defined(SPI2)
            case SPI2_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef spi_peripheral_clock_settings = { 0 };
                spi_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SPI2;
                spi_peripheral_clock_settings.Spi123ClockSelection     = spisd_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&spi_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SPI2_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SPI2_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* SPI2 */
#if defined(SPI3)
            case SPI3_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef spi_peripheral_clock_settings = { 0 };
                spi_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SPI3;
                spi_peripheral_clock_settings.Spi123ClockSelection     = spisd_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&spi_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SPI3_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SPI3_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* SPI3 */
#if defined(SPI4)
            case SPI4_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef spi_peripheral_clock_settings = { 0 };
                spi_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SPI4;
                spi_peripheral_clock_settings.Spi45ClockSelection      = spisd_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&spi_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SPI4_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SPI4_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* SPI4 */
#if defined(SPI5)
            case SPI5_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef spi_peripheral_clock_settings = { 0 };
                spi_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SPI5;
                spi_peripheral_clock_settings.Spi45ClockSelection      = spisd_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&spi_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SPI5_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SPI5_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* SPI5 */
#if defined(SPI6)
            case SPI6_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef spi_peripheral_clock_settings = { 0 };
                spi_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SPI6;
                spi_peripheral_clock_settings.Spi6ClockSelection       = spisd_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&spi_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SPI6_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SPI6_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* SPI6 */
            default :
                return_value = RET_ERROR;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpisdDeInitClock(spisdInst_t *spisd_inst)
 * @brief           Function that disables SPISD peripheral clock
 * @param[in,out]   spisd_inst   Instance that contains SPI handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t SpisdDeInitClock(spisdInst_t *spisd_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spisd_inst != NULL)
    {
        // Select the peripheral clock
        switch ((uintptr_t)spisd_inst->p_conf->spi_periph)
        {
            case SPI1_BASE :
            {
                __HAL_RCC_SPI1_CLK_DISABLE();
                break;
            }
#if defined(SPI2)
            case SPI2_BASE :
            {
                __HAL_RCC_SPI2_CLK_DISABLE();
                break;
            }
#endif /* SPI2 */
#if defined(SPI3)
            case SPI3_BASE :
            {
                __HAL_RCC_SPI3_CLK_DISABLE();
                break;
            }
#endif /* SPI3 */
#if defined(SPI4)
            case SPI4_BASE :
            {
                __HAL_RCC_SPI4_CLK_DISABLE();
                break;
            }
#endif /* SPI4 */
#if defined(SPI5)
            case SPI5_BASE :
            {
                __HAL_RCC_SPI5_CLK_DISABLE();
                break;
            }
#endif /* SPI5 */
#if defined(SPI6)
            case SPI6_BASE :
            {
                __HAL_RCC_SPI6_CLK_DISABLE();
                break;
            }
#endif /* SPI6 */
            default :
                return_value = RET_ERROR;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpisdSetupIOs(spisdInst_t *spisd_inst, const spisdConf_t *const spisd_conf)
 * @brief           Function that setups IOs
 * @param[in,out]   spisd_inst   Instance that contains SD handlers
 * @param[in]       spisd_conf   Configuration that contains SD parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this SD
 */
static returnCode_t SpisdSetupIOs(spisdInst_t *spisd_inst, const spisdConf_t *const spisd_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(spisd_inst);

    // Check parameter(s)
    if ((spisd_inst != NULL) && (spisd_conf != NULL))
    {
        // First init SCK IO
        return_value = SetupIO(&spisd_conf->io_sck);
        if (return_value == RET_SUCCESSFUL)
        {
            // Then init MISO IO
            return_value = SetupIO(&spisd_conf->io_miso);
            if (return_value == RET_SUCCESSFUL)
            {
                // Then init MOSI IO
                return_value = SetupIO(&spisd_conf->io_mosi);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Then init CS IO
                    return_value = SetupIO(&spisd_conf->io_cs);
                }
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpiSD_Select(void)
 * @brief           Select SD card on SPI bus
 * @param[in,out]   spisd_inst   Instance that contains SD handlers
 * @retval          #RET_TIMEOUT if SPI timeouted
 * @retval          #RET_NOT_AVAILABLE if SPI was not available
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SpiSD_Select(spisdInst_t *spisd_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    uint8_t fill_char         = SPI_FILL_CHAR;

    // Select slave
    HAL_GPIO_WritePin(spisd_inst->cs_port, spisd_inst->cs_pin, GPIO_PIN_RESET);
    // Then send a fill char onto MOSI
    HAL_StatusTypeDef test_hal = HAL_SPI_Transmit(&spisd_inst->spi_handle_struct, &fill_char, 1u, SPISD_TIMEOUT);
    if (test_hal == HAL_OK)
    {
        return_value = RET_SUCCESSFUL;
    }
    else if (test_hal == HAL_BUSY)
    {
        return_value = RET_NOT_AVAILABLE;
    }
    else if (test_hal == HAL_TIMEOUT)
    {
        return_value = RET_TIMEOUT;
    }
    else
    {
        KernelPanic();
    }

    return return_value;
}

/**
 * @fn              SpiSD_Unselect(void)
 * @brief           Unselect SD card on SPI bus
 * @param[in,out]   spisd_inst   Instance that contains SD handlers
 * @retval          #RET_TIMEOUT if SPI timeouted
 * @retval          #RET_NOT_AVAILABLE if SPI was not available
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SpiSD_Unselect(spisdInst_t *spisd_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    uint8_t fill_char         = SPI_FILL_CHAR;

    // Send a fill char onto MOSI
    HAL_StatusTypeDef test_hal = HAL_SPI_Transmit(&spisd_inst->spi_handle_struct, &fill_char, 1u, SPISD_TIMEOUT);
    if (test_hal == HAL_OK)
    {
        // Then unselect slave
        HAL_GPIO_WritePin(spisd_inst->cs_port, spisd_inst->cs_pin, GPIO_PIN_SET);
    }
    else if (test_hal == HAL_BUSY)
    {
        return_value = RET_NOT_AVAILABLE;
    }
    else if (test_hal == HAL_TIMEOUT)
    {
        return_value = RET_TIMEOUT;
    }
    else
    {
        KernelPanic();
    }

    return return_value;
}

/**
 * @fn              SpiSD_WaitUntilReady(spisdInst_t *spisd_inst)
 * @brief           Wait until SD card is ready
 * @param[in,out]   spisd_inst   Instance that contains SD handlers
 * @retval          #RET_SUCCESSFUL if SD card is ready (spi slave register is now empty)
 * @retval          #RET_TIMEOUT if function timeouted before clearing SD card being ready
 */
static returnCode_t SpiSD_WaitUntilReady(spisdInst_t *spisd_inst)
{
    returnCode_t return_value  = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_hal = HAL_OK;
    uint8_t answer             = 0u;
    uint32_t tickstart         = HAL_GetTick();

    // Read SD card until it returns SPI_FILL_CHAR or timeouted
    while ((test_hal == HAL_OK) && (answer != SPI_FILL_CHAR) && ((HAL_GetTick() - tickstart) < SPISD_TIMEOUT))
    {
        test_hal = HAL_SPI_Receive(&spisd_inst->spi_handle_struct, &answer, 1u, SPISD_TIMEOUT);
    }

    if (((HAL_GetTick() - tickstart) >= SPISD_TIMEOUT) || (test_hal == HAL_TIMEOUT))
    {
        return_value = RET_TIMEOUT;
    }

    if ((test_hal == HAL_ERROR) || (test_hal == HAL_BUSY))
    {
        KernelPanic();
    }

    return return_value;
}

/**
 * @fn              SpiSD_Wakeup(spisdInst_t *spisd_inst)
 * @brief           Wakeup SD card
 * @param[in,out]   spisd_inst   Instance that contains SD handlers
 * @retval          #RET_INVALID_PARAM if there is a null pointer
 * @retval          #RET_TIMEOUT if SPI timeouted
 * @retval          #RET_NOT_AVAILABLE if SPI was not available
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SpiSD_Wakeup(spisdInst_t *spisd_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spisd_inst != NULL)
    {
        // Wakeup SD card by sending pad caracter without selecting it
        uint8_t wakeup_message[SPISD_WAKEUP_MSG_SIZE];
        (void)memset(&wakeup_message, SPI_FILL_CHAR, SPISD_WAKEUP_MSG_SIZE);

        // Unselect the SD card (should do nothing unless the sd card was selected)
        return_value = SpiSD_Unselect(spisd_inst);
        if (return_value == RET_SUCCESSFUL)
        {
            HAL_StatusTypeDef test_hal = HAL_SPI_Transmit(&spisd_inst->spi_handle_struct, wakeup_message, SPISD_WAKEUP_MSG_SIZE, SPISD_TIMEOUT);
            if (test_hal == HAL_OK)
            {
                return_value = RET_SUCCESSFUL;
            }
            else if (test_hal == HAL_BUSY)
            {
                return_value = RET_NOT_AVAILABLE;
            }
            else if (test_hal == HAL_TIMEOUT)
            {
                return_value = RET_TIMEOUT;
            }
            else
            {
                KernelPanic();
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

static returnCode_t SpiSD_SelectSpiMode(spisdInst_t *spisd_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spisd_inst != NULL)
    {
        // Select SD card
        return_value = SpiSD_Select(spisd_inst);
        if (return_value == RET_SUCCESSFUL)
        {
            uint8_t reset_spi_mode_cmd[SPISD_CMD_MSG_SIZE] = { SPISD_CMD0, 0x00u, 0x00u, 0x00u, 0x00u, 0x95u };
            // Send reset onto spi mode command
            HAL_StatusTypeDef test_hal = HAL_SPI_Transmit(&spisd_inst->spi_handle_struct, reset_spi_mode_cmd, SPISD_CMD_MSG_SIZE, SPISD_TIMEOUT);
            if (test_hal == HAL_OK)
            {
                uint8_t answer     = SPI_FILL_CHAR;
                uint32_t tickstart = HAL_GetTick();
                // Wait until SD card
                while ((test_hal == HAL_OK) && (answer != SPISD_IDLE_FLAG) && ((HAL_GetTick() - tickstart) < SPISD_TIMEOUT))
                {
                    test_hal = HAL_SPI_Receive(&spisd_inst->spi_handle_struct, &answer, 1u, SPISD_TIMEOUT);
                }

                // Test if procedure wents well
                if ((test_hal == HAL_OK) && ((HAL_GetTick() - tickstart) < SPISD_TIMEOUT))
                {
                    return_value = RET_SUCCESSFUL;
                }
                else if ((HAL_GetTick() - tickstart) >= SPISD_TIMEOUT)
                {
                    return_value = RET_TIMEOUT;
                }
                else if (test_hal == HAL_BUSY)
                {
                    return_value = RET_NOT_AVAILABLE;
                }
                else
                {
                    KernelPanic();
                }
            }
            else if (test_hal == HAL_BUSY)
            {
                return_value = RET_NOT_AVAILABLE;
            }
            else if (test_hal == HAL_TIMEOUT)
            {
                return_value = RET_TIMEOUT;
            }
            else
            {
                KernelPanic();
            }

            // Unselect SD card anyway
            if (SpiSD_Unselect(spisd_inst) != RET_SUCCESSFUL)
            {
                KernelPanic();
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpiSD_InitSDCard(spisdInst_t *spisd_inst)
 * @brief           Does the SD card initialisation routine.
 * @param[in,out]   spisd_inst   Instance that contains SD handlers
 * @retval          #RET_INVALID_PARAM if there is a null pointer
 * @retval          #RET_TIMEOUT if SPI timeouted
 * @retval          #RET_NOT_AVAILABLE if SPI was not available
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SpiSD_InitSDCard(spisdInst_t *spisd_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spisd_inst != NULL)
    {
        // Select SD card
        return_value = SpiSD_Select(spisd_inst);
        if (return_value == RET_SUCCESSFUL)
        {
            // Send Go Idle Command to start initialisation procedure
            return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD0, SPISD_NULL_COMMAND_ARG, NULL, 0u);
            if (return_value == RET_SUCCESSFUL)
            {
                uint8_t interface_condition[SPISD_CMD_MSG_ANSWER_SIZE] = { 0 };

                // If CMD8 command is accept it is SDC V2 type, if not type is SDC V1
                return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD8, SPISD_CARD_INTERFACE_COND, (uint8_t *)&interface_condition, 4u);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Type is SDC V2+
                    // Now check voltage set is effective
                    if ((interface_condition[2] == (uint8_t)((0x0000ff00u & SPISD_CARD_INTERFACE_COND) >> 8u))
                        && (interface_condition[3] == (uint8_t)(0x000000ffu & SPISD_CARD_INTERFACE_COND)))
                    {
                        uint32_t tickstart = HAL_GetTick();
                        // Activates SD card activation process until initialisation ended
                        returnCode_t test = RET_TIMEOUT;
                        while ((test == RET_TIMEOUT) && ((HAL_GetTick() - tickstart) < SPISD_TIMEOUT))
                        {
                            test = SpiSD_SendCmd(spisd_inst, SPISD_CMD55, SPISD_NULL_COMMAND_ARG, NULL, 0u);
                            if (test == RET_SUCCESSFUL)
                            {
                                // Sends host capacity support information and activates the card's initialization process. (HCS bit = 1 because
                                // we supports SDHC and SDXC)
                                test = SpiSD_SendCmd(spisd_inst, SPISD_CMD41, SPISD_INITIALIZATION_CONF, NULL, 0u);
                            }
                        }

                        // Check if initialisation wents well
                        if (test == RET_SUCCESSFUL)
                        {
                            uint8_t ocr[SPISD_CMD_MSG_ANSWER_SIZE] = { 0 };
                            // Read Operation Control Register (OCR) and check CCS (card capacity status)
                            return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD58, SPISD_NULL_COMMAND_ARG, (uint8_t *)&ocr, 4u);
                            if (return_value == RET_SUCCESSFUL)
                            {
                                // Check if High Capacity or not (SDCARD_V2HC vs SDCARD_V2)
                                if ((ocr[0] & SPISD_CCS_BITMASK) == SPISD_CCS_BITMASK)
                                {
                                    spisd_inst->sd_type = SDCARD_V2HC;
                                }
                                else
                                {
                                    spisd_inst->sd_type = SDCARD_V2;
                                }
                            }
                        }
                        else
                        {
                            KernelPanic();
                        }
                    }
                }
                else
                {
                    // Type is SDC V1 or MMC
                    return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD55, SPISD_NULL_COMMAND_ARG, NULL, 0);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD41, SPISD_NULL_COMMAND_ARG, NULL, 0);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // Set Block Lenght to 512 bits
                            return_value = SpiSD_SendCmd(spisd_inst, SPISD_CMD16, SPISD_SECTOR_SIZE, NULL, 0u);
                            if (return_value != RET_SUCCESSFUL)
                            {
                                spisd_inst->sd_type = SDCARD_V1;
                            }
                            else
                            {
                                spisd_inst->sd_type = NOT_SDCARD;
                            }
                        }
                    }
                }
            }

            // Unselect SD card anyway
            if (SpiSD_Unselect(spisd_inst) != RET_SUCCESSFUL)
            {
                KernelPanic();
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpiSD_ReadData(spisdInst_t *spisd_inst, data_t data, length_t length)
 * @brief           Reads data from SD card
 * @param[in,out]   spisd_inst  Instance that contains SD handlers
 * @param[out]      buff        Buffer containing the block received
 * @param[in]       length      Length in byte of the data
 * @retval          #RET_INVALID_PARAM if buff is null pointer or len is null
 * @retval          #RET_TIMEOUT if SD card was not ready
 * @retval          #RET_NOT_AVAILABLE if SPI was not available
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SpiSD_ReadData(spisdInst_t *spisd_inst, data_t data, length_t length)
{
    returnCode_t return_value  = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_hal = HAL_OK;
    uint8_t token              = SPI_FILL_CHAR;

    // Check parameter(s)
    if ((spisd_inst != NULL) && (data != NULL) && (length != 0u))
    {
        // Loop until receive a response or timeout
        uint32_t tickstart = HAL_GetTick();
        while ((test_hal == HAL_OK) && (token == SPI_FILL_CHAR) && ((HAL_GetTick() - tickstart) < SPISD_TIMEOUT))
        {
            test_hal = HAL_SPI_Receive(&spisd_inst->spi_handle_struct, &token, 1u, SPISD_TIMEOUT);
        }

        // Check if read was successful and gets a start block token
        if (((token == SPISD_START_BLOCK_TOKEN) || (token == SPISD_START_MULT_BLOCK_TOKEN)) && (test_hal == HAL_OK)
            && ((HAL_GetTick() - tickstart) < SPISD_TIMEOUT))
        {
            // Receive block
            test_hal = HAL_SPI_Receive(&spisd_inst->spi_handle_struct, data, length, SPISD_TIMEOUT);

            // Check if block has corretly been read
            if (test_hal == HAL_OK)
            {
                uint8_t crc[2] = { 0 };
                // Receive (and discard CRC)
                test_hal = HAL_SPI_Receive(&spisd_inst->spi_handle_struct, crc, 2u, SPISD_TIMEOUT);
                // Check if crc has corretly been read
                if (test_hal != HAL_OK)
                {
                    KernelPanic();
                }
            }
            else if (test_hal == HAL_BUSY)
            {
                return_value = RET_NOT_AVAILABLE;
            }
            else if (test_hal == HAL_TIMEOUT)
            {
                return_value = RET_TIMEOUT;
            }
            else
            {
                KernelPanic();
            }
        }
        else if ((test_hal == HAL_TIMEOUT) || ((HAL_GetTick() - tickstart) >= SPISD_TIMEOUT))
        {
            return_value = RET_TIMEOUT;
        }
        else if (test_hal == HAL_BUSY)
        {
            return_value = RET_NOT_AVAILABLE;
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpiSD_WriteData(const uint8_t *buff, uint32_t len, uint8_t token)
 * @brief           Writes data to SD card
 * @param[in,out]   spisd_inst  Instance that contains SD handlers
 * @param[out]      buff        Buffer containing the block received
 * @param[in]       length      Length in byte of
 * @param[in]       token   Token indicating type of transmission
 * @retval          #RET_INVALID_PARAM if buff is null pointer or len is null except if token is SPISD_STOP_TOKEN
 * @retval          #RET_TIMEOUT if SD card was not ready
 * @retval          #RET_NOT_AVAILABLE if SPI was not available
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SpiSD_WriteData(spisdInst_t *spisd_inst, data_t data, length_t length, uint8_t token)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_hal;

    // Check parameter(s)
    if ((spisd_inst != NULL)
        && (((token == SPISD_STOP_TOKEN) && (data == NULL) && (length == 0u)) || ((token != SPISD_STOP_TOKEN) && (data != NULL) && (length != 0u))))
    {
        // Get current tick
        uint32_t tickstart = HAL_GetTick();

        // Wait until SD card is ready
        return_value = SpiSD_WaitUntilReady(spisd_inst);
        if (return_value == RET_SUCCESSFUL)
        {
            // Send token
            test_hal = HAL_SPI_Transmit(&spisd_inst->spi_handle_struct, &token, 1u, SPISD_TIMEOUT);
            if (test_hal == HAL_OK)
            {
                // If it's not STOP token, data needs to be sent
                if (token != SPISD_STOP_TOKEN)
                {
                    test_hal = HAL_SPI_Transmit(&spisd_inst->spi_handle_struct, data, length, SPISD_TIMEOUT);
                    if (test_hal == HAL_OK)
                    {
                        uint8_t crc[2] = { 0 };
                        // Read and discard CRC
                        test_hal = HAL_SPI_Receive(&spisd_inst->spi_handle_struct, crc, 2u, SPISD_TIMEOUT);
                        if (test_hal == HAL_OK)
                        {
                            uint8_t answer = SPI_FILL_CHAR;
                            while ((test_hal == HAL_OK) && (answer == SPI_FILL_CHAR) && ((HAL_GetTick() - tickstart) < SPISD_TIMEOUT))
                            {
                                test_hal = HAL_SPI_Receive(&spisd_inst->spi_handle_struct, &answer, 1u, SPISD_TIMEOUT);
                            }

                            // Check if we get the answer
                            if ((test_hal == HAL_OK) && (answer != SPI_FILL_CHAR) && ((HAL_GetTick() - tickstart) < SPISD_TIMEOUT))
                            {
                                // Clear receive buffer until fill char is received
                                return_value = SpiSD_WaitUntilReady(spisd_inst);
                                if (return_value == RET_SUCCESSFUL)
                                {
                                    // Check if data has been accepted
                                    if ((answer & SPISD_DATA_RESPONSE_MASK) != SPISD_DATA_ACCEPTED)
                                    {
                                        KernelPanic();
                                    }
                                }
                            }
                            else if (((HAL_GetTick() - tickstart) >= SPISD_TIMEOUT) || (test_hal == HAL_TIMEOUT))
                            {
                                return_value = RET_TIMEOUT;
                            }
                            else if (test_hal == HAL_BUSY)
                            {
                                return_value = RET_NOT_AVAILABLE;
                            }
                            else
                            {
                                KernelPanic();
                            }
                        }
                        else if (test_hal == HAL_BUSY)
                        {
                            return_value = RET_NOT_AVAILABLE;
                        }
                        else if (test_hal == HAL_TIMEOUT)
                        {
                            return_value = RET_TIMEOUT;
                        }
                        else
                        {
                            KernelPanic();
                        }
                    }
                    else if (test_hal == HAL_BUSY)
                    {
                        return_value = RET_NOT_AVAILABLE;
                    }
                    else if (test_hal == HAL_TIMEOUT)
                    {
                        return_value = RET_TIMEOUT;
                    }
                    else
                    {
                        KernelPanic();
                    }
                }
            }
            else if (test_hal == HAL_BUSY)
            {
                return_value = RET_NOT_AVAILABLE;
            }
            else if (test_hal == HAL_TIMEOUT)
            {
                return_value = RET_TIMEOUT;
            }
            else
            {
                KernelPanic();
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpiSD_SendCmd(spisdInst_t *spisd_inst, uint8_t cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size)
 * @brief           Sends a command to the SD card
 * @param[in,out]   spisd_inst   Instance that contains SD handlers
 * @param[in]       cmd         Command to send
 * @param[in]       arg         Command argument
 * @param[out]      answer      Command answer
 * @param[in]       answer_size Command answer size
 * @retval          #RET_INVALID_PARAM if command is invalid, or answer is null pointer but answer_size non null
 * @retval          #RET_TIMEOUT if SD card was not ready or CMD12 still busy
 * @retval          #RET_NOT_AVAILABLE if SPI was not available
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SpiSD_SendCmd(spisdInst_t *spisd_inst, uint8_t cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_hal;

    // Check parameter(s)
    if ((spisd_inst != NULL) && (((answer_size == 0u) && (answer == NULL)) || ((answer_size != 0u) && (answer != NULL))))
    {
        uint32_t tickstart = HAL_GetTick();
        if ((cmd >= 0x40u) && (cmd <= 0x7fu))
        {
            // Wait until transfer complete
            return_value = SpiSD_WaitUntilReady(spisd_inst);
            if (return_value == RET_SUCCESSFUL)
            {
                uint8_t cmd_msg[SPISD_CMD_MSG_SIZE] = { 0 };
                // Build command message with function arguments
                cmd_msg[0] = (uint8_t)(cmd);
                cmd_msg[1] = (uint8_t)((0xff000000u & arg) >> 24u);
                cmd_msg[2] = (uint8_t)((0x00ff0000u & arg) >> 16u);
                cmd_msg[3] = (uint8_t)((0x0000ff00u & arg) >> 8u);
                cmd_msg[4] = (uint8_t)(0x000000ffu & arg);
                cmd_msg[5] = ComputeCommandCRC7(cmd_msg);

                // Send Command
                test_hal = HAL_SPI_Transmit(&spisd_inst->spi_handle_struct, cmd_msg, SPISD_CMD_MSG_SIZE, SPISD_TIMEOUT);
                if (test_hal == HAL_OK)
                {
                    uint8_t command_status = SPI_FILL_CHAR;
                    // Wait until the idle flag is on
                    while ((command_status == SPI_FILL_CHAR) && (test_hal == HAL_OK) && ((HAL_GetTick() - tickstart) < SPISD_TIMEOUT))
                    {
                        test_hal = HAL_SPI_Receive(&spisd_inst->spi_handle_struct, &command_status, 1u, SPISD_TIMEOUT);
                    }

                    // Check Result
                    if ((test_hal == HAL_OK) && (command_status <= SPISD_IDLE_FLAG))
                    {
                        if ((cmd == SPISD_CMD41) && (command_status != 0u))
                        {
                            return_value = RET_TIMEOUT;
                        }
                        else
                        {
                            // If command is CMD12 (STOP_TRANSMISSION) wait until ready
                            if (cmd == SPISD_CMD12)
                            {
                                return_value = SpiSD_WaitUntilReady(spisd_inst);
                            }
                            else
                            {
                                if (answer_size != 0u)
                                {
                                    // Receive answer
                                    test_hal = HAL_SPI_Receive(&spisd_inst->spi_handle_struct, answer, answer_size, SPISD_TIMEOUT);

                                    // Check if everything wents well
                                    if ((test_hal == HAL_OK))
                                    {
                                        return_value = RET_SUCCESSFUL;
                                    }
                                    else if (test_hal == HAL_BUSY)
                                    {
                                        return_value = RET_NOT_AVAILABLE;
                                    }
                                    else if (test_hal == HAL_TIMEOUT)
                                    {
                                        return_value = RET_TIMEOUT;
                                    }
                                    else
                                    {
                                        KernelPanic();
                                    }
                                }
                            }
                        }
                    }
                    else if ((test_hal == HAL_BUSY) || (command_status > SPISD_IDLE_FLAG))
                    {
                        return_value = RET_NOT_AVAILABLE;
                    }
                    else if (test_hal == HAL_TIMEOUT)
                    {
                        return_value = RET_TIMEOUT;
                    }
                    else
                    {
                        KernelPanic();
                    }
                }
                else if (test_hal == HAL_BUSY)
                {
                    return_value = RET_NOT_AVAILABLE;
                }
                else if (test_hal == HAL_TIMEOUT)
                {
                    return_value = RET_TIMEOUT;
                }
                else
                {
                    KernelPanic();
                }
            }
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ComputeCommandCRC7(const uint8_t *cmd_msg)
 * @brief       Computes CRC7 for sd command message and add 1 as byte's LSB
 * @param[in]   cmd_msg Command message for which crc is calculated
 * @return      CRC7 for bits 7 to 1 and 0b1 for bit 0
 */
static uint8_t ComputeCommandCRC7(const uint8_t *cmd_msg)
{
    const uint8_t g_sd_crc7_lookup_table[256] = {
        0x00, 0x12, 0x24, 0x36, 0x48, 0x5a, 0x6c, 0x7e, 0x90, 0x82, 0xb4, 0xa6, 0xd8, 0xca, 0xfc, 0xee, 0x32, 0x20, 0x16, 0x04, 0x7a, 0x68,
        0x5e, 0x4c, 0xa2, 0xb0, 0x86, 0x94, 0xea, 0xf8, 0xce, 0xdc, 0x64, 0x76, 0x40, 0x52, 0x2c, 0x3e, 0x08, 0x1a, 0xf4, 0xe6, 0xd0, 0xc2,
        0xbc, 0xae, 0x98, 0x8a, 0x56, 0x44, 0x72, 0x60, 0x1e, 0x0c, 0x3a, 0x28, 0xc6, 0xd4, 0xe2, 0xf0, 0x8e, 0x9c, 0xaa, 0xb8, 0xc8, 0xda,
        0xec, 0xfe, 0x80, 0x92, 0xa4, 0xb6, 0x58, 0x4a, 0x7c, 0x6e, 0x10, 0x02, 0x34, 0x26, 0xfa, 0xe8, 0xde, 0xcc, 0xb2, 0xa0, 0x96, 0x84,
        0x6a, 0x78, 0x4e, 0x5c, 0x22, 0x30, 0x06, 0x14, 0xac, 0xbe, 0x88, 0x9a, 0xe4, 0xf6, 0xc0, 0xd2, 0x3c, 0x2e, 0x18, 0x0a, 0x74, 0x66,
        0x50, 0x42, 0x9e, 0x8c, 0xba, 0xa8, 0xd6, 0xc4, 0xf2, 0xe0, 0x0e, 0x1c, 0x2a, 0x38, 0x46, 0x54, 0x62, 0x70, 0x82, 0x90, 0xa6, 0xb4,
        0xca, 0xd8, 0xee, 0xfc, 0x12, 0x00, 0x36, 0x24, 0x5a, 0x48, 0x7e, 0x6c, 0xb0, 0xa2, 0x94, 0x86, 0xf8, 0xea, 0xdc, 0xce, 0x20, 0x32,
        0x04, 0x16, 0x68, 0x7a, 0x4c, 0x5e, 0xe6, 0xf4, 0xc2, 0xd0, 0xae, 0xbc, 0x8a, 0x98, 0x76, 0x64, 0x52, 0x40, 0x3e, 0x2c, 0x1a, 0x08,
        0xd4, 0xc6, 0xf0, 0xe2, 0x9c, 0x8e, 0xb8, 0xaa, 0x44, 0x56, 0x60, 0x72, 0x0c, 0x1e, 0x28, 0x3a, 0x4a, 0x58, 0x6e, 0x7c, 0x02, 0x10,
        0x26, 0x34, 0xda, 0xc8, 0xfe, 0xec, 0x92, 0x80, 0xb6, 0xa4, 0x78, 0x6a, 0x5c, 0x4e, 0x30, 0x22, 0x14, 0x06, 0xe8, 0xfa, 0xcc, 0xde,
        0xa0, 0xb2, 0x84, 0x96, 0x2e, 0x3c, 0x0a, 0x18, 0x66, 0x74, 0x42, 0x50, 0xbe, 0xac, 0x9a, 0x88, 0xf6, 0xe4, 0xd2, 0xc0, 0x1c, 0x0e,
        0x38, 0x2a, 0x54, 0x46, 0x70, 0x62, 0x8c, 0x9e, 0xa8, 0xba, 0xc4, 0xd6, 0xe0, 0xf2
    };
    uint8_t crc = 0u;

    // Check parameter(s)
    if (cmd_msg != NULL)
    {
        for (uint32_t i = 0u; i < (SPISD_CMD_MSG_SIZE - 1u); i++)
        {
            crc = g_sd_crc7_lookup_table[crc ^ cmd_msg[i]];
        }

        // Set 1 on the LSB
        crc |= 1u;
    }

    return crc;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @fn              SpisdGenericIRQHandler(void *param)
 * @brief           Generic SPISD IRQ Handler
 */
static void SpisdGenericIRQHandler(void *param)
{
    // Get spi inst
    spiInst_t *spisd_inst = (spiInst_t *)param;

    // Do IRQ
    HAL_SPI_IRQHandler(&spisd_inst->handle_struct);
}