/**
 * @file    common.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for drivers common functions (e.g. HAL init)
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/common.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitHal(void)
 * @brief   Function that init the choosen HAL and sysclock
 * @return  Nothing
 */
void InitHal(void)
{
    // Init clocks and power
    __HAL_RCC_SYSCFG_CLK_ENABLE();
#if defined(STM32F4)
    __HAL_RCC_PWR_CLK_ENABLE();
#endif

    // Init HAL
    HAL_StatusTypeDef test_hal = HAL_Init();

    // Check return value
    if (test_hal == HAL_OK)
    {
        returnCode_t test = SystemClock_Config();
        if (test != RET_SUCCESSFUL)
        {
            KernelPanic();
        }
    }
    else
    {
        KernelPanic();
    }
}

/**
 * @fn          SetupIO(const IOConf_t *const io_conf)
 * @brief       Initialize a single IO according to the configuration.
 * @param[in]   io  GPIO configuration (port/pin/mode/pull/speed/alternate)
 * @retval      #RET_SUCCESSFUL on success
 * @retval      #RET_INVALID_PARAM if the port is invalid
 */
returnCode_t SetupIO(const IOConf_t *const io_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (io_conf != NULL)
    {
        // First init IO clock
        switch ((uintptr_t)io_conf->port)
        {
            case GPIOA_BASE :
                __HAL_RCC_GPIOA_CLK_ENABLE();
                break;
            case GPIOB_BASE :
                __HAL_RCC_GPIOB_CLK_ENABLE();
                break;
            case GPIOC_BASE :
                __HAL_RCC_GPIOC_CLK_ENABLE();
                break;
#if defined(GPIOD_BASE)
            case GPIOD_BASE :
                __HAL_RCC_GPIOD_CLK_ENABLE();
                break;
#endif
#if defined(GPIOE_BASE)
            case GPIOE_BASE :
                __HAL_RCC_GPIOE_CLK_ENABLE();
                break;
#endif
#if defined(GPIOF_BASE)
            case GPIOF_BASE :
                __HAL_RCC_GPIOF_CLK_ENABLE();
                break;
#endif
#if defined(GPIOG_BASE)
            case GPIOG_BASE :
                __HAL_RCC_GPIOG_CLK_ENABLE();
                break;
#endif
#if defined(GPIOH_BASE)
            case GPIOH_BASE :
                __HAL_RCC_GPIOH_CLK_ENABLE();
                break;
#endif
#if defined(GPIOI_BASE)
            case GPIOI_BASE :
                __HAL_RCC_GPIOI_CLK_ENABLE();
                break;
#endif
#if defined(GPIOJ_BASE)
            case GPIOJ_BASE :
                __HAL_RCC_GPIOJ_CLK_ENABLE();
                break;
#endif
#if defined(GPIOK_BASE)
            case GPIOK_BASE :
                __HAL_RCC_GPIOK_CLK_ENABLE();
                break;
#endif
            default :
                return_value = RET_INVALID_PARAM;
                break;
        }

        if (return_value == RET_SUCCESSFUL)
        {
            // Then set RX IO :
            GPIO_InitTypeDef gpio_init = {
                .Pin       = io_conf->pin,      //
                .Mode      = io_conf->mode,     //
                .Pull      = io_conf->pull,     //
                .Speed     = io_conf->speed,    //
                .Alternate = io_conf->alternate //
            };
            HAL_GPIO_Init(io_conf->port, &gpio_init);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SetUpDMA(DMAHandleStruct_t *dma_handle, const DMAConf_t *const dma_conf)
 * @brief           Function that setup DMA if it exists
 * @param[in,out]   dma_handle   Instance that contains DMA handlers
 * @param[in]       dma_conf     Configuration that contains DMA parameters
 * @retval          #RET_INVALID_PARAM if there is a null pointer or is not a DMA
 * @retval          #RET_ERROR if DMA initialisation failed
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t SetUpDMA(DMAHandleStruct_t *dma_handle, const DMAConf_t *const dma_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_hal;

    // Check parameter(s)
    if ((dma_handle != NULL) && (dma_conf != NULL) && ((DMA_FROM_STREAM(dma_conf->ref) == DMA1) || (DMA_FROM_STREAM(dma_conf->ref) == DMA2)))
    {
        if (DMA_FROM_STREAM(dma_conf->ref) == DMA1)
        {
            __HAL_RCC_DMA1_CLK_ENABLE();
        }
        else
        {
            __HAL_RCC_DMA2_CLK_ENABLE();
        }

        // Setup DMA RX
        dma_handle->Instance = dma_conf->ref;
#if defined(STM32H7)
        dma_handle->Init.Request = dma_conf->channel;
#elif defined(STM32F4)
        dma_handle->Init.Channel = dma_conf->channel;
#else
#error
#endif
        dma_handle->Init.Direction           = dma_conf->direction;
        dma_handle->Init.PeriphInc           = DMA_PINC_DISABLE;
        dma_handle->Init.MemInc              = DMA_MINC_ENABLE;
        dma_handle->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        dma_handle->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        dma_handle->Init.Mode                = DMA_NORMAL;
        dma_handle->Init.Priority            = DMA_PRIORITY_LOW;
        dma_handle->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;

        // Init DMA RX
        test_hal = HAL_DMA_Init(dma_handle);
        if (test_hal != HAL_OK)
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
