/**
 * @file    diskdrv_sd.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for SD card using SDMMC driver
 *
 * @copyright Copyright (c) TOLOSAT 2025
 * Adapted from STMicroelectronic example
 */

/******************************* Include Files *******************************/

#include "drv/disks.h"
#include "drv/disk/diskdrv_sd.h"
#include "core/irq.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#if defined(SDIO)
#define SDMMC1                              SDIO                               /**< Redefinition for compatibility */
#define SDMMC_CLOCK_EDGE_RISING             SDIO_CLOCK_EDGE_RISING             /**< Redefinition for compatibility */
#define SDMMC_CLOCK_POWER_SAVE_DISABLE      SDIO_CLOCK_POWER_SAVE_DISABLE      /**< Redefinition for compatibility */
#define SDMMC_BUS_WIDE_4B                   SDIO_BUS_WIDE_4B                   /**< Redefinition for compatibility */
#define SDMMC_HARDWARE_FLOW_CONTROL_DISABLE SDIO_HARDWARE_FLOW_CONTROL_DISABLE /**< Redefinition for compatibility */
#endif

#define SD_TIMEOUT            30000u     /**< SD Card Timeout for ST HAL */
#define SD_DEFAULT_BLOCK_SIZE 512u       /**< Size of a block in the SD Card */
#define SD_NOT_PRESENT        0x00u      /**< Indicates that no SD card is present */
#define SD_PRESENT            0x01u      /**< Indicates that an SD card is present*/
#define SD_DETECT_PIN         GPIO_PIN_5 /**< GPIO detect pin for SD card */
#define SD_DETECT_PORT        GPIOD      /**< GPIO detect port for SD card */

/*************************** Functions Declarations **************************/

static void SDGenericIRQHandler(void *param);
static returnCode_t SD_WaitUntilReady(void);

/*************************** Variables Definitions ***************************/

static SD_HandleTypeDef sd_card_inst; /**< SD card instance */

/*************************** Functions Definitions ***************************/

/**
 * @fn          SD_DiskStatus(uint8_t disk)
 * @brief       Function that gets status of the SD card
 * @param[in]   disk    Disk from which we get the status
 * @return      diskStatus_t
 */
diskStatus_t SD_DiskStatus(uint8_t disk)
{
    diskStatus_t return_value = STA_NOINIT;

    // Check parameter(s)
    if (disk != DISK0_REF)
    {
        return_value = STA_NODISK;
    }
    else
    {
        HAL_SD_CardStateTypeDef card_state = HAL_SD_GetCardState(&sd_card_inst);
        if (card_state == HAL_SD_CARD_TRANSFER)
        {
            return_value &= ~STA_NOINIT;
        }
        else
        {
            return_value = STA_NODISK;
        }
    }

    return return_value;
}

/**
 * @fn          SD_DiskInit(uint8_t disk)
 * @brief       Function that initialises an SD card with SDMMC
 * @param[in]   disk    Disk that will be initialised
 * @retval      STA_NODISK if disk number is not valid
 * @retval      STA_NOINIT if disk initialisation failed
 * @retval      0 if disk initialization is a success
 */
diskStatus_t SD_DiskInit(uint8_t disk)
{
    diskStatus_t return_value             = STA_NOINIT;
    sd_card_inst.Instance                 = SDMMC1;
    sd_card_inst.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
    sd_card_inst.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    sd_card_inst.Init.BusWide             = SDMMC_BUS_WIDE_4B;
    sd_card_inst.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
    sd_card_inst.Init.ClockDiv            = 8u;

    // Check parameter(s)
    if (disk == DISK0_REF)
    {
        /* HAL SD initialization */
        HAL_StatusTypeDef test_hal = HAL_SD_Init(&sd_card_inst);
        if (test_hal == HAL_OK)
        {
            // Set sd inst as the interrupt parameter to pass it to the interrupt routine
            IRQHandlerParam_t param = (IRQHandlerParam_t)&sd_card_inst;
            // Request the interrupt
            returnCode_t request_status = RequestIRQ(SDMMC1_IRQn, 5u, SDGenericIRQHandler, param);
            if (request_status == RET_SUCCESSFUL)
            {
                return_value &= ~STA_NOINIT;
            }
        }
    }
    else
    {
        return_value = STA_NODISK;
    }

    return return_value;
}

/**
 * @fn          SD_DiskRead(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that reads SD card blocks using SDMMC
 * @param[in]   disk    Disk that is read
 * @param[out]  data    Pointer to the data that will be read
 * @param[in]   addr    Address of the data that will be read
 * @param[in]   len     Number of block that will be read
 * @retval      #RET_INVALID_PARAM if disk does not exist, len equal zero, pointer is null
 * @retval      #RET_TIMEOUT if disk is not available
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SD_DiskRead(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((disk == DISK0_REF) && (len != 0u) && (data != NULL))
    {
        HAL_StatusTypeDef test_hal = HAL_SD_ReadBlocks_IT(&sd_card_inst, data, addr, len);
        if (test_hal == HAL_OK)
        {
            return_value = SD_WaitUntilReady();
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
 * @fn          SD_DiskWrite(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that writes SD card blocks using SDMMC
 * @param[in]   disk    Disk that is written
 * @param[in]   data    Pointer to the data that will be written
 * @param[in]   addr    Address of the data that will be written
 * @param[in]   len     Number of block that will be written
 * @retval      #RET_INVALID_PARAM if disk does not exist, len equal zero, pointer is null
 * @retval      #RET_TIMEOUT if disk is not available
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SD_DiskWrite(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((disk == DISK0_REF) && (len != 0u) && (data != NULL))
    {
        HAL_StatusTypeDef test_hal = HAL_SD_WriteBlocks_IT(&sd_card_inst, (uint8_t *)data, addr, len); // cppcheck-suppress misra-c2012-11.8;
                                                                                                       // Low-level drivers don't use the
                                                                                                       // const argument so it has to
                                                                                                       // disappear somewhere
        if (test_hal == HAL_OK)
        {
            return_value = SD_WaitUntilReady();
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
 * @fn              SD_DiskIoctl(uint8_t disk, uint8_t cmd, void *data)
 * @brief           Function that perfoms io control on the SD card (get info, change parameters ...)
 * @param[in]       disk    Disk on which we perform the io control
 * @param[in]       cmd     Which type of action is done on the SD card
 * @param[in,out]   data    Data shared depending of command
 * @retval          #RET_INVALID_PARAM if the io control is not available for this device
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t SD_DiskIoctl(uint8_t disk, uint8_t cmd, void *data)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_SD_CardInfoTypeDef CardInfo;

    // Check parameter(s)
    if ((SD_DiskStatus(disk) & STA_NOINIT) == STA_NOINIT)
    {
        KernelPanic();
    }
    else
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        switch (cmd)
        {
            /* Make sure that no pending write process */
            case CTRL_SYNC :
                // Sync is not required for thois SD card driver, so do nothing
                break;

            /* Get number of sectors on the disk (DWORD) */
            case GET_SECTOR_COUNT :
                test_val = HAL_SD_GetCardInfo(&sd_card_inst, &CardInfo);
                if (test_val == HAL_OK)
                {
                    *(DWORD *)data = CardInfo.LogBlockNbr;
                }
                break;

            /* Get R/W sector size (WORD) */
            case GET_SECTOR_SIZE :
                test_val = HAL_SD_GetCardInfo(&sd_card_inst, &CardInfo);
                if (test_val == HAL_OK)
                {
                    *(WORD *)data = CardInfo.LogBlockSize;
                }
                break;

            /* Get erase block size in unit of sector (DWORD) */
            case GET_BLOCK_SIZE :
                test_val = HAL_SD_GetCardInfo(&sd_card_inst, &CardInfo);
                if (test_val == HAL_OK)
                {
                    *(DWORD *)data = CardInfo.LogBlockSize / SD_DEFAULT_BLOCK_SIZE;
                }
                break;

            default :
                return_value = RET_INVALID_PARAM;
                break;
        }

        if (return_value != RET_INVALID_PARAM)
        {
            switch (test_val)
            {
                case HAL_OK :
                    return_value = RET_SUCCESSFUL;
                    break;
                case HAL_TIMEOUT :
                    return_value = RET_TIMEOUT;
                    break;
                case HAL_BUSY :
                    return_value = RET_NOT_AVAILABLE;
                    break;
                default :
                    KernelPanic();
                    break;
            }
        }
    }

    return return_value;
}

/**
 * @fn      SD_WaitUntilReady(void)
 * @brief   Wait until the SD card is ready
 * @retval  #RET_TIMEOUT if the SD card wasn't ready after SD_TIMEOUT amount of time
 * @retval  #RET_SUCCESSFUL else
 */
static returnCode_t SD_WaitUntilReady(void)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_SD_CardStateTypeDef sd_state;

    uint32_t tickstart = HAL_GetTick();

    // First wait until the SD instance is ready
    while ((sd_card_inst.State == HAL_SD_STATE_BUSY) && ((HAL_GetTick() - tickstart) < SD_TIMEOUT))
    {
        __NOP();
    }

    // Then wait until the SD card returns in TRANSFER state
    do
    {
        sd_state = HAL_SD_GetCardState(&sd_card_inst);
    } while ((sd_state != HAL_SD_CARD_TRANSFER) && ((HAL_GetTick() - tickstart) < SD_TIMEOUT));

    // Check if timeouted or not
    if (sd_state != HAL_SD_CARD_TRANSFER)
    {
        return_value = RET_TIMEOUT;
    }

    return return_value;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @fn      SDGenericIRQHandler(void *param)
 * @brief   Generic SD IRQ Handler
 */
static void SDGenericIRQHandler(void *param)
{
    // Get sd inst
    SD_HandleTypeDef *sd_inst = (SD_HandleTypeDef *)param;

    // Do IRQ
    HAL_SD_IRQHandler(sd_inst);
}
