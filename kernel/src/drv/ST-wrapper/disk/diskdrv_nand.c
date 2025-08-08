/**
 * @file    diskdrv_nand.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for NAND disk using FMC NAND driver
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/disks.h"
#include "drv/disk/diskdrv_nand.h"
#include "drv/peripherals/drv_gpio.h"
#include "core/irq.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void NANDGenericIRQHandler(void *param);
static NAND_AddressTypeDef NAND_LinearToAddress(uint32_t linear_address);

/*************************** Variables Definitions ***************************/

static NAND_HandleTypeDef nand_inst; /**< NAND flash instance */
static NAND_IDTypeDef nand_id; /**< NAND flash id */

/**
 * @var     write_protection_gpio
 * @brief   GPIO for write protection
 */
static gpioInst_t write_protection_gpio = {
    .port     = NAND_WP_PORT,
    .pin      = NAND_WP_PIN,
    .inout    = GPIO_MODE_OUTPUT_PP,
    .pull     = GPIO_NOPULL,
    .speed    = GPIO_SPEED_FREQ_LOW,
    .irq_no   = IRQ_NONE,
    .callback = NULL,
};

/*************************** Functions Definitions ***************************/

/**
 * @fn          NAND_DiskStatus(uint8_t disk)
 * @brief       Function that gets status of the NAND flash
 * @param[in]   disk    Disk from which we get the status
 * @return      diskStatus_t
 */
diskStatus_t NAND_DiskStatus(uint8_t disk)
{
    diskStatus_t return_value = STA_NOINIT;

    // Check parameter(s)
    if (disk != DISK0_REF)
    {
        return_value = STA_NODISK;
    }
    else
    {
        HAL_NAND_StateTypeDef status = HAL_NAND_GetState(&nand_inst);
        if (status == HAL_NAND_STATE_READY)
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
 * @fn          NAND_DiskInit(uint8_t disk)
 * @brief       Function that initialises an NAND flash with NANDMMC
 * @param[in]   disk    Disk that will be initialised
 * @retval      STA_NODISK if disk number is not valid
 * @retval      STA_NOINIT if disk initialisation failed
 * @retval      0 if disk initialization is a success
 */
diskStatus_t NAND_DiskInit(uint8_t disk)
{
    diskStatus_t return_value                 = STA_NOINIT;
    FMC_NAND_PCC_TimingTypeDef ComSpaceTiming = { 0 };
    FMC_NAND_PCC_TimingTypeDef AttSpaceTiming = { 0 };
    nand_inst.Instance                        = FMC_NAND_DEVICE;
    // Configuration NAND
    nand_inst.Init.NandBank             = NAND_BANK;
    nand_inst.Init.Waitfeature          = NAND_WAIT_FEATURE;
    nand_inst.Init.MemoryDataWidth      = NAND_MEMORY_BUS_WIDTH;
    nand_inst.Init.EccComputation       = NAND_ECC_COMPUTATION;
    nand_inst.Init.ECCPageSize          = NAND_ECC_PAGE_SIZE;
    nand_inst.Init.TCLRSetupTime        = NAND_TCLR_SETUP_TIME;
    nand_inst.Init.TARSetupTime         = NAND_TAR_SETUP_TIME;
    nand_inst.Config.PageSize           = NAND_PAGE_SIZE;
    nand_inst.Config.SpareAreaSize      = NAND_SPARE_AREA_SIZE;
    nand_inst.Config.BlockSize          = NAND_BLOCK_SIZE_IN_PAGES;
    nand_inst.Config.BlockNbr           = NAND_BLOCK_COUNT;
    nand_inst.Config.PlaneNbr           = NAND_PLANE_COUNT;
    nand_inst.Config.PlaneSize          = NAND_PLANE_SIZE_IN_BLOCKS;
    nand_inst.Config.ExtraCommandEnable = NAND_EXTRA_COMMAND;
    // Timing configurations
    ComSpaceTiming.SetupTime     = NAND_TIMING_SETUP_TIME;
    ComSpaceTiming.WaitSetupTime = NAND_TIMING_WAIT_TIME;
    ComSpaceTiming.HoldSetupTime = NAND_TIMING_HOLD_TIME;
    ComSpaceTiming.HiZSetupTime  = NAND_TIMING_HIZ_TIME;
    AttSpaceTiming.SetupTime     = NAND_TIMING_SETUP_TIME;
    AttSpaceTiming.WaitSetupTime = NAND_TIMING_WAIT_TIME;
    AttSpaceTiming.HoldSetupTime = NAND_TIMING_HOLD_TIME;
    AttSpaceTiming.HiZSetupTime  = NAND_TIMING_HIZ_TIME;

    // Check parameter(s)
    if (disk == DISK0_REF)
    {
        /* HAL NAND initialization */
        HAL_StatusTypeDef test_hal = HAL_NAND_Init(&nand_inst, &ComSpaceTiming, &AttSpaceTiming);
        if (test_hal == HAL_OK)
        {
            // Set nand inst as the interrupt parameter to pass it to the interrupt routine
            IRQHandlerParam_t param = (IRQHandlerParam_t)&nand_inst;
            // Request the interrupt
            return_value = RequestIRQ(FMC_IRQn, 5u, NANDGenericIRQHandler, param);
            if (return_value == RET_SUCCESSFUL)
            {
                // Setup WP GPIO
                return_value = GpioOpen(&write_protection_gpio);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Then Enable Write
                    return_value = GpioWrite(&write_protection_gpio, GPIO_PIN_RESET);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        // Then Read NAND ID
                        test_hal = HAL_NAND_Read_ID(&nand_inst, &nand_id);
                        if (test_hal == HAL_OK)
                        {
                            return_value &= ~STA_NOINIT;
                        }
                    }
                }
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
 * @fn          NAND_DiskRead(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that reads NAND flash blocks using NANDMMC
 * @param[in]   disk    Disk that is read
 * @param[out]  data    Pointer to the data that will be read
 * @param[in]   addr    Address of the data that will be read
 * @param[in]   len     Number of block that will be read
 * @retval      #RET_INVALID_PARAM if disk does not exist, len equal zero, pointer is null
 * @retval      #RET_TIMEOUT if disk is not available
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t NAND_DiskRead(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((disk == DISK0_REF) && (len != 0u) && (data != NULL))
    {
        NAND_AddressTypeDef nand_addr = NAND_LinearToAddress(addr);
        HAL_StatusTypeDef test_hal    = HAL_NAND_Read_Page_8b(&nand_inst, &nand_addr, data, len);
        if (test_hal != HAL_OK)
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
 * @fn          NAND_DiskWrite(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that writes NAND flash blocks using NANDMMC
 * @param[in]   disk    Disk that is written
 * @param[in]   data    Pointer to the data that will be written
 * @param[in]   addr    Address of the data that will be written
 * @param[in]   len     Number of block that will be written
 * @retval      #RET_INVALID_PARAM if disk does not exist, len equal zero, pointer is null
 * @retval      #RET_TIMEOUT if disk is not available
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t NAND_DiskWrite(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((disk == DISK0_REF) && (len != 0u) && (data != NULL))
    {
        NAND_AddressTypeDef nand_addr = NAND_LinearToAddress(addr);
        HAL_StatusTypeDef test_hal    = HAL_NAND_Write_Page_8b(&nand_inst, &nand_addr, (uint8_t *)data, len); // cppcheck-suppress misra-c2012-11.8;
                                                                                                              // Low-level drivers don't use the const
                                                                                                              // argument so it has to disappear
                                                                                                              // somewhere
        if (test_hal != HAL_OK)
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
 * @fn              NAND_DiskIoctl(uint8_t disk, uint8_t cmd, void *data)
 * @brief           Function that perfoms io control on the NAND flash (get info, change parameters ...)
 * @param[in]       disk    Disk on which we perform the io control
 * @param[in]       cmd     Which type of action is done on the NAND flash
 * @param[in,out]   data    Data shared depending of command
 * @retval          #RET_INVALID_PARAM if the io control is not available for this device
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t NAND_DiskIoctl(uint8_t disk, uint8_t cmd, void *data)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((NAND_DiskStatus(disk) & STA_NOINIT) == STA_NOINIT)
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
                // Sync is not required for this NAND driver, so do nothing
                break;

            /* Get number of sectors on the disk (DWORD) */
            case GET_SECTOR_COUNT :
                if (data != NULL)
                {
                    // Total pages = blocks per plane * planes * pages per block
                    *(uint32_t *)data = (uint32_t)(NAND_BLOCK_COUNT * NAND_PLANE_COUNT * NAND_BLOCK_SIZE_IN_PAGES);
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;

            /* Get R/W sector size (WORD) */
            case GET_SECTOR_SIZE :
                if (data != NULL)
                {
                    *(uint16_t *)data = NAND_PAGE_SIZE; // 4096 for example
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;

            /* Get erase block size in unit of sector (DWORD) */
            case GET_BLOCK_SIZE :
                if (data != NULL)
                {
                    *(uint32_t *)data = NAND_BLOCK_SIZE_IN_PAGES; // sectors per block
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
 * @brief   Converts a linear page address into a NAND physical address.
 * @param[in]  linear_address  Linear address in number of pages
 * @return     NAND_AddressTypeDef structure with Page, Block, Plane
 */
static NAND_AddressTypeDef NAND_LinearToAddress(uint32_t linear_address)
{
    NAND_AddressTypeDef addr;

    addr.Page  = linear_address % NAND_BLOCK_SIZE_IN_PAGES;
    addr.Block = (linear_address / NAND_BLOCK_SIZE_IN_PAGES) % NAND_PLANE_SIZE_IN_BLOCKS;
    addr.Plane = (linear_address / (NAND_BLOCK_SIZE_IN_PAGES * NAND_PLANE_SIZE_IN_BLOCKS));

    return addr;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @fn      NANDGenericIRQHandler(void *param)
 * @brief   Generic NAND IRQ Handler
 */
static void NANDGenericIRQHandler(void *param)
{
    // Get nand inst
    NAND_HandleTypeDef *nand_inst = (NAND_HandleTypeDef *)param;

    // Do IRQ
    HAL_NAND_IRQHandler(nand_inst);
}
