/**
 * @file    drv_nand.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for NAND memory using FMC
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/memories.h"
#include "drv/memories/drv_nand.h"
#include "core/irq.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t NandInitClock(nandInst_t *nand_inst, const nandConf_t *const nand_conf);
static returnCode_t NandDeInitClock(nandInst_t *nand_inst);
static returnCode_t NandSetupIOs(nandInst_t *nand_inst, const nandConf_t *const nand_conf);
static NAND_AddressTypeDef NAND_LinearToAddress(nandInst_t *nand_inst, uint32_t linear_address);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              NandOpen(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
 * @brief           Function that initialise a NAND memory
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @param[in]       nand_conf   Configuration that contains NAND parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 */
returnCode_t NandOpen(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((nand_inst != NULL) && (nand_conf != NULL))
    {
        // Init peripheral clock
        return_value = NandInitClock(nand_inst, nand_conf);
        if (return_value == RET_SUCCESSFUL)
        {
            // Setup IOs
            return_value = NandSetupIOs(nand_inst, nand_conf);
            if (return_value == RET_SUCCESSFUL)
            {
                // Setup NAND
                nand_inst->handle_struct.Instance                  = nand_conf->periph;
                nand_inst->handle_struct.Init.NandBank             = nand_conf->bank;
                nand_inst->handle_struct.Init.Waitfeature          = nand_conf->wait_feature;
                nand_inst->handle_struct.Init.MemoryDataWidth      = nand_conf->data_width;
                nand_inst->handle_struct.Init.EccComputation       = nand_conf->ecc_computation;
                nand_inst->handle_struct.Init.ECCPageSize          = nand_conf->ecc_page_size;
                nand_inst->handle_struct.Init.TCLRSetupTime        = nand_conf->TCLR_setup_time;
                nand_inst->handle_struct.Init.TARSetupTime         = nand_conf->TAR_setup_time;
                nand_inst->handle_struct.Config.PageSize           = nand_conf->page_size;
                nand_inst->handle_struct.Config.SpareAreaSize      = nand_conf->spare_area_size;
                nand_inst->handle_struct.Config.BlockSize          = nand_conf->block_size;
                nand_inst->handle_struct.Config.BlockNbr           = nand_conf->nb_block;
                nand_inst->handle_struct.Config.PlaneNbr           = nand_conf->nb_plane;
                nand_inst->handle_struct.Config.PlaneSize          = nand_conf->plane_size;
                nand_inst->handle_struct.Config.ExtraCommandEnable = nand_conf->extra_cmd;
                /* HAL NAND initialization */
                HAL_StatusTypeDef test_hal = HAL_NAND_Init(&nand_inst->handle_struct, (FMC_NAND_PCC_TimingTypeDef *)&nand_conf->com_space_timing,
                                                           (FMC_NAND_PCC_TimingTypeDef *)&nand_conf->attr_space_timing);
                if (test_hal == HAL_OK)
                {
                    // Link the conf pointer
                    nand_inst->p_conf = nand_conf;
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
 * @fn          NandWrite(nandInst_t *nand_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto an NAND memory
 * @param[in]   nand_inst     Instance that contains NAND parameters and NAND Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer from which data will be copied
 * @param[in]   length      Number of block that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if nand timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if nand is still sending previous message
 */
returnCode_t NandWrite(nandInst_t *nand_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((nand_inst != NULL) && (length != 0u) && (data != NULL))
    {
        NAND_AddressTypeDef nand_addr = NAND_LinearToAddress(nand_inst, sector);
        HAL_StatusTypeDef test_hal    = HAL_NAND_Write_Page_8b(&nand_inst->handle_struct, &nand_addr, (uint8_t *)data, length);
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
 * @fn          NandRead(nandInst_t *nand_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an NAND memory
 * @param[in]   nand_inst     Instance that contains NAND parameters and NAND Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of block that will be read
 * @retval      #RET_SUCCESSFUL if data has been read successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if nand timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if nand is still sending previous message
 */
returnCode_t NandRead(nandInst_t *nand_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((nand_inst != NULL) && (length != 0u) && (data != NULL))
    {
        NAND_AddressTypeDef nand_addr = NAND_LinearToAddress(nand_inst, sector);
        HAL_StatusTypeDef test_hal    = HAL_NAND_Read_Page_8b(&nand_inst->handle_struct, &nand_addr, data, length);
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
 * @fn              NandIoctl(nandInst_t *nand_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   nand_inst     Instance that contains NAND handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t NandIoctl(nandInst_t *nand_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (nand_inst != NULL)
    {
        switch (cmd)
        {
            case IOCTL_MEMORY_GET_STATUS :
                if (data_size == sizeof(memoryStatus_t))
                {
                    HAL_NAND_StateTypeDef status = HAL_NAND_GetState(&nand_inst->handle_struct);
                    if (status == HAL_NAND_STATE_READY)
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
                // Sync is not required for this NAND card driver, so do nothing
                break;
            case IOCTL_MEMORY_GET_SECTOR_COUNT :
                if (data_size == sizeof(memorySectorCount_t))
                {
                    *(memorySectorCount_t *)data =
                        (uint32_t)(nand_inst->p_conf->nb_plane * nand_inst->p_conf->plane_size * nand_inst->p_conf->block_size);
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_GET_SECTOR_SIZE :
                if (data_size == sizeof(memorySectorSize_t))
                {
                    *(memorySectorSize_t *)data = nand_inst->p_conf->page_size;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_GET_BLOCK_SIZE :
                if (data_size == sizeof(memoryBlockSize_t))
                {
                    *(memoryBlockSize_t *)data = nand_inst->p_conf->block_size;
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
 * @fn              NandClose(nandInst_t *nand_inst)
 * @brief           Function that desinit the NAND connection
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
returnCode_t NandClose(nandInst_t *nand_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (nand_inst != NULL)
    {
        HAL_NAND_DeInit(&nand_inst->handle_struct);
        (void)NandDeInitClock(nand_inst);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @brief   Converts a linear page address into a NAND physical address.
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @param[in]  linear_address  Linear address in number of pages
 * @return     NAND_AddressTypeDef structure with Page, Block, Plane
 */
static NAND_AddressTypeDef NAND_LinearToAddress(nandInst_t *nand_inst, uint32_t linear_address)
{
    NAND_AddressTypeDef addr;

    addr.Page  = linear_address % nand_inst->p_conf->block_size;
    addr.Block = (linear_address / nand_inst->p_conf->block_size) % nand_inst->p_conf->plane_size;
    addr.Plane = (linear_address / (nand_inst->p_conf->block_size * nand_inst->p_conf->plane_size));

    return addr;
}

/**
 * @fn              NandInitClock(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
 * @brief           Function that setups NAND peripheral clock
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @param[in]       nand_conf   Configuration that contains NAND parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t NandInitClock(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(nand_inst);

    // Check parameter(s)
    if ((nand_inst != NULL) && (nand_conf != NULL))
    {
#if defined(STM32H7)
        RCC_PeriphCLKInitTypeDef nand_peripheral_clock_settings = { 0 };
        nand_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_FMC;
        nand_peripheral_clock_settings.FmcClockSelection        = nand_conf->clk_src;
        if (HAL_RCCEx_PeriphCLKConfig(&nand_peripheral_clock_settings) == HAL_OK)
        {
            __HAL_RCC_FMC_CLK_ENABLE();
        }
        else
        {
            return_value = RET_ERROR;
        }
#elif defined(STM32F4)
        __HAL_RCC_FMC_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              NandDeInitClock(nandInst_t *nand_inst)
 * @brief           Function that disables NAND peripheral clock
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t NandDeInitClock(nandInst_t *nand_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (nand_inst != NULL)
    {
        (void)(nand_inst);
        __HAL_RCC_FMC_CLK_DISABLE();
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              NandSetupIOs(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
 * @brief           Function that setups IOs
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @param[in]       nand_conf   Configuration that contains NAND parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this NAND
 */
static returnCode_t NandSetupIOs(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(nand_inst);

    // Check parameter(s)
    if ((nand_inst != NULL) && (nand_conf != NULL))
    {
        // First Init CLE IO
        return_value = SetupIO(&nand_conf->io_cle);

        // Init ALE IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_ale);
        }

        // Init NCE IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_nce);
        }

        // Init NOE IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_noe);
        }

        // Init NWE IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_nwe);
        }

        // Init NWAIT IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_nwait);
        }

        // Init D0 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d0);
        }

        // Init D1 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d1);
        }

        // Init D2 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d2);
        }

        // Init D3 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d3);
        }

        // Init D4 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d4);
        }

        // Init D5 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d5);
        }

        // Init D6 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d6);
        }

        // Init D7 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d7);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
