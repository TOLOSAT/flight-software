/**
 * @file    drv_sd.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for SD card memory using SDIO bus
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/memories.h"
#include "drv/memories/drv_sd.h"
#include "core/irq.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define SD_TIMEOUT               1000u /**< SD Card Timeout for ST HAL */
#define SD_BLOCK_SIZE_IN_SECTORS 1u    /**< Erase block size in sectors (fixed to 1 for SD) */

#if defined(STM32F4)
#define SDMMC1_BASE SDIO_BASE /**< Redefinition for compatibility */
#endif

/*************************** Functions Declarations **************************/

static void SDGenericIRQHandler(void *param);
static returnCode_t SdInitClock(sdInst_t *sd_inst, const sdConf_t *const sd_conf);
static returnCode_t SdDeInitClock(sdInst_t *sd_inst);
static returnCode_t SdSetupIOs(sdInst_t *sd_inst, const sdConf_t *const sd_conf);
static returnCode_t SdWaitUntilReady(sdInst_t *sd_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              SdOpen(sdInst_t *sd_inst, const sdConf_t *const sd_conf)
 * @brief           Function that initialise a SD memory
 * @param[in,out]   sd_inst   Instance that contains SD handlers
 * @param[in]       sd_conf   Configuration that contains SD parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 */
returnCode_t SdOpen(sdInst_t *sd_inst, const sdConf_t *const sd_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((sd_inst != NULL) && (sd_conf != NULL))
    {
        // Init peripheral clock
        return_value = SdInitClock(sd_inst, sd_conf);
        if (return_value == RET_SUCCESSFUL)
        {
            // Setup IOs
            return_value = SdSetupIOs(sd_inst, sd_conf);
            if (return_value == RET_SUCCESSFUL)
            {
                // Setup SD
                sd_inst->handle_struct.Instance                 = sd_conf->periph;
                sd_inst->handle_struct.Init.ClockEdge           = sd_conf->clock_edge;
                sd_inst->handle_struct.Init.ClockPowerSave      = sd_conf->clock_power_save;
                sd_inst->handle_struct.Init.BusWide             = sd_conf->bus_width;
                sd_inst->handle_struct.Init.HardwareFlowControl = sd_conf->hw_flow_ctrl;
                sd_inst->handle_struct.Init.ClockDiv            = sd_conf->prescaler;

                // HAL SD initialization
                HAL_StatusTypeDef test_hal = HAL_SD_Init(&sd_inst->handle_struct);
                if (test_hal == HAL_OK)
                {
                    // Link the conf pointer
                    sd_inst->p_conf = sd_conf;
                    // Set sd inst as the interrupt parameter to pass it to the interrupt routine
                    IRQHandlerParam_t param = (IRQHandlerParam_t)&sd_inst->handle_struct;
                    // Request the interrupt
                    return_value = RequestIRQ(sd_conf->irq_no, sd_conf->irq_prio, SDGenericIRQHandler, param);
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
 * @fn          SdWrite(sdInst_t *sd_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto an SD memory
 * @param[in]   sd_inst     Instance that contains SD parameters and SD Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer from which data will be copied
 * @param[in]   length      Number of block that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if sd timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if sd is still sending previous message
 */
returnCode_t SdWrite(sdInst_t *sd_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((sd_inst != NULL) && (length != 0u) && (data != NULL))
    {
        HAL_StatusTypeDef test_hal = HAL_SD_WriteBlocks_IT(&sd_inst->handle_struct, (uint8_t *)data, sector, length); // cppcheck-suppress
                                                                                                                      // misra-c2012-11.8; Low-level
                                                                                                                      // drivers don't use the const
                                                                                                                      // argument so it has to
                                                                                                                      // disappear somewhere
        if (test_hal == HAL_OK)
        {
            return_value = SdWaitUntilReady(sd_inst);
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
 * @fn          SdRead(sdInst_t *sd_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an SD memory
 * @param[in]   sd_inst     Instance that contains SD parameters and SD Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of block that will be read
 * @retval      #RET_SUCCESSFUL if data has been read successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if sd timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if sd is still sending previous message
 */
returnCode_t SdRead(sdInst_t *sd_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((sd_inst != NULL) && (length != 0u) && (data != NULL))
    {
        HAL_StatusTypeDef test_hal = HAL_SD_ReadBlocks_IT(&sd_inst->handle_struct, data, sector, length);
        if (test_hal == HAL_OK)
        {
            return_value = SdWaitUntilReady(sd_inst);
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
 * @fn              SdIoctl(sdInst_t *sd_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   sd_inst     Instance that contains SD handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t SdIoctl(sdInst_t *sd_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (sd_inst != NULL)
    {
        HAL_SD_CardInfoTypeDef CardInfo;
        switch (cmd)
        {
            case IOCTL_MEMORY_GET_STATUS :
                if (data_size == sizeof(memoryStatus_t))
                {
                    HAL_SD_CardStateTypeDef card_state = HAL_SD_GetCardState(&sd_inst->handle_struct);
                    if (card_state == HAL_SD_CARD_TRANSFER)
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
                // Sync is not required for this SD card driver, so do nothing
                break;
            case IOCTL_MEMORY_GET_SECTOR_COUNT :
                if (data_size == sizeof(memorySectorCount_t))
                {
                    if (HAL_SD_GetCardInfo(&sd_inst->handle_struct, &CardInfo) == HAL_OK)
                    {
                        *(memorySectorCount_t *)data = CardInfo.LogBlockNbr;
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
                break;
            case IOCTL_MEMORY_GET_SECTOR_SIZE :
                if (data_size == sizeof(memorySectorSize_t))
                {
                    if (HAL_SD_GetCardInfo(&sd_inst->handle_struct, &CardInfo) == HAL_OK)
                    {
                        *(memorySectorSize_t *)data = CardInfo.LogBlockSize;
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
                break;
            case IOCTL_MEMORY_GET_BLOCK_SIZE :
                if (data_size == sizeof(memoryBlockSize_t))
                {
                    if (HAL_SD_GetCardInfo(&sd_inst->handle_struct, &CardInfo) == HAL_OK)
                    {
                        *(memoryBlockSize_t *)data = SD_BLOCK_SIZE_IN_SECTORS;
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
 * @fn              SdClose(sdInst_t *sd_inst)
 * @brief           Function that desinit the SD connection
 * @param[in,out]   sd_inst   Instance that contains SD handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
returnCode_t SdClose(sdInst_t *sd_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (sd_inst != NULL)
    {
        HAL_SD_DeInit(&sd_inst->handle_struct);
        (void)SdDeInitClock(sd_inst);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SdInitClock(sdInst_t *sd_inst, const sdConf_t *const sd_conf)
 * @brief           Function that setups SD peripheral clock
 * @param[in,out]   sd_inst   Instance that contains SD handlers
 * @param[in]       sd_conf   Configuration that contains SD parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t SdInitClock(sdInst_t *sd_inst, const sdConf_t *const sd_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(sd_inst);

    // Check parameter(s)
    if ((sd_inst != NULL) && (sd_conf != NULL))
    {
        // Select the peripheral clock
        switch ((uintptr_t)sd_conf->periph)
        {
            case SDMMC1_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef sd_peripheral_clock_settings = { 0 };
                sd_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SDMMC;
                sd_peripheral_clock_settings.SdmmcClockSelection      = sd_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&sd_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SDMMC1_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SDMMC1_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#if defined(SDMMC2)
            case SDMMC2_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef sd_peripheral_clock_settings = { 0 };
                sd_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SDMMC;
                sd_peripheral_clock_settings.SdmmcClockSelection      = sd_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&sd_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SDMMC2_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SDMMC2_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* SDMMC2_BASE */
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
 * @fn              SdDeInitClock(sdInst_t *sd_inst)
 * @brief           Function that disables SD peripheral clock
 * @param[in,out]   sd_inst   Instance that contains SD handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t SdDeInitClock(sdInst_t *sd_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (sd_inst != NULL)
    {
        // Select the peripheral clock
        switch ((uintptr_t)sd_inst->p_conf->periph)
        {
            case SDMMC1_BASE :
            {
                __HAL_RCC_SDMMC1_CLK_DISABLE();
                break;
            }
#if defined(SDMMC2)
            case SDMMC2_BASE :
            {
                __HAL_RCC_SDMMC2_CLK_DISABLE();
                break;
            }
#endif /* SDMMC2_BASE */
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
 * @fn              SdSetupIOs(sdInst_t *sd_inst, const sdConf_t *const sd_conf)
 * @brief           Function that setups IOs
 * @param[in,out]   sd_inst   Instance that contains SD handlers
 * @param[in]       sd_conf   Configuration that contains SD parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this SD
 */
static returnCode_t SdSetupIOs(sdInst_t *sd_inst, const sdConf_t *const sd_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(sd_inst);

    // Check parameter(s)
    if ((sd_inst != NULL) && (sd_conf != NULL))
    {
        // First init CMD IO
        return_value = SetupIO(&sd_conf->io_cmd);
        if (return_value == RET_SUCCESSFUL)
        {
            // Then init CLK IO
            return_value = SetupIO(&sd_conf->io_clk);
            if (return_value == RET_SUCCESSFUL)
            {
                // Then init D0 IO
                return_value = SetupIO(&sd_conf->io_d0);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Then init D1 IO
                    return_value = SetupIO(&sd_conf->io_d1);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        // Then init D2 IO
                        return_value = SetupIO(&sd_conf->io_d2);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // Then init D3 IO
                            return_value = SetupIO(&sd_conf->io_d3);
                        }
                    }
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
 * @fn              SdWaitUntilReady(sdInst_t *sd_inst)
 * @brief           Wait until the SD card is ready
 * @param[in,out]   sd_inst   Instance that contains SD handlers
 * @retval          #RET_TIMEOUT if the SD card wasn't ready after SD_TIMEOUT amount of time
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SdWaitUntilReady(sdInst_t *sd_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_SD_CardStateTypeDef sd_state;

    uint32_t tickstart = HAL_GetTick();

    // First wait until the SD instance is ready
    while ((sd_inst->handle_struct.State == HAL_SD_STATE_BUSY) && ((HAL_GetTick() - tickstart) < SD_TIMEOUT))
    {
        __NOP();
    }

    // Then wait until the SD card returns in TRANSFER state
    do
    {
        sd_state = HAL_SD_GetCardState(&sd_inst->handle_struct);
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
