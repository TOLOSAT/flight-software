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
#include "core/irq.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void NANDGenericIRQHandler(void *param);

/*************************** Variables Definitions ***************************/

static NAND_HandleTypeDef nand_inst; /**< NAND card instance */

/*************************** Functions Definitions ***************************/

/**
 * @fn          NAND_DiskStatus(uint8_t disk)
 * @brief       Function that gets status of the NAND card
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
 * @brief       Function that initialises an NAND card with NANDMMC
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
    /* nand_inst.Init */
    nand_inst.Init.NandBank        = FMC_NAND_BANK3;
    nand_inst.Init.Waitfeature     = FMC_NAND_WAIT_FEATURE_ENABLE;
    nand_inst.Init.MemoryDataWidth = FMC_NAND_MEM_BUS_WIDTH_8;
    nand_inst.Init.EccComputation  = FMC_NAND_ECC_DISABLE;
    nand_inst.Init.ECCPageSize     = FMC_NAND_ECC_PAGE_SIZE_4096BYTE;
    nand_inst.Init.TCLRSetupTime   = 0;
    nand_inst.Init.TARSetupTime    = 0;
    /* nand_inst.Config */
    nand_inst.Config.PageSize           = 4096;
    nand_inst.Config.SpareAreaSize      = 256;
    nand_inst.Config.BlockSize          = 64;
    nand_inst.Config.BlockNbr           = 2048;
    nand_inst.Config.PlaneNbr           = 2;
    nand_inst.Config.PlaneSize          = 1;
    nand_inst.Config.ExtraCommandEnable = DISABLE;
    /* ComSpaceTiming */
    ComSpaceTiming.SetupTime     = 0;
    ComSpaceTiming.WaitSetupTime = 2;
    ComSpaceTiming.HoldSetupTime = 1;
    ComSpaceTiming.HiZSetupTime  = 0;
    /* AttSpaceTiming */
    AttSpaceTiming.SetupTime     = 0;
    AttSpaceTiming.WaitSetupTime = 2;
    AttSpaceTiming.HoldSetupTime = 1;
    AttSpaceTiming.HiZSetupTime  = 0;

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
            returnCode_t request_status = RequestIRQ(FMC_IRQn, 5u, NANDGenericIRQHandler, param);
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
 * @fn          NAND_DiskRead(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that reads NAND card blocks using NANDMMC
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
        // To Do
        (void)(data);
        (void)(addr);
        (void)(len);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          NAND_DiskWrite(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that writes NAND card blocks using NANDMMC
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
        // To Do
        (void)(data);
        (void)(addr);
        (void)(len);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              NAND_DiskIoctl(uint8_t disk, uint8_t cmd, void *data)
 * @brief           Function that perfoms io control on the NAND card (get info, change parameters ...)
 * @param[in]       disk    Disk on which we perform the io control
 * @param[in]       cmd     Which type of action is done on the NAND card
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
                // To Do
                break;

            /* Get number of sectors on the disk (DWORD) */
            case GET_SECTOR_COUNT :
                // To Do
                (void)(cmd);
                (void)(data);
                break;

            /* Get R/W sector size (WORD) */
            case GET_SECTOR_SIZE :
                // To Do
                break;

            /* Get erase block size in unit of sector (DWORD) */
            case GET_BLOCK_SIZE :
                // To Do
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
