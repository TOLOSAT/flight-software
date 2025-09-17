/**
 * @file    drv_qspimram.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for QSPI_MRAM flash memory using QSPI bus
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/memories.h"
#include "drv/memories/drv_qspimram.h"
#include "core/irq.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

// Global constant
#define QSPIMRAM_TIMEOUT                 1000u /**< QSPIMRAM Card Timeout for ST HAL */
#define QSPIMRAM_VIRTUAL_SECTOR_SIZE     512u  /**< Virtual sector size in byte */
#define QSPIMRAM_BLOCK_SIZE_IN_SECTORS   1u    /**< Erase block size in sectors (fixed to 1 for QSPIMRAM) */
#define QSPIMRAM_READ_DUMMY_CLOCK_CYCLES 12u   /**< Default dummy clock cycles for read */

// QSPI flash commands
#define QSPIMRAM_WRITE_ENABLE_CMD        0x06u /**< Write Enable command */
#define QSPIMRAM_QSPI_ENABLE_CMD         0x38u /**< Enable QSPI command */
#define QSPIMRAM_FAST_READ_QUAD_CMD      0x0bu /**< Fast Read Quad I/O (4-4-4) */
#define QSPIMRAM_FAST_WRITE_QUAD_CMD     0xd2u /**< Fast Write Quad I/O (4-4-4) */

/*************************** Functions Declarations **************************/

static void QSPIGenericIRQHandler(void *param);
static returnCode_t QspiMramInitClock(qspimramInst_t *qspimram_inst, const qspimramConf_t *const qspimram_conf);
static returnCode_t QspiMramDeInitClock(qspimramInst_t *qspimram_inst);
static returnCode_t QspiMramSetupIOs(qspimramInst_t *qspimram_inst, const qspimramConf_t *const qspimram_conf);
static returnCode_t QspiMramWriteEnable(qspimramInst_t *qspimram_inst);
static returnCode_t QspiMramSetupQSPIMode(qspimramInst_t *qspimram_inst);
static inline uint32_t QspiMramCalcFlashSize(const qspimramConf_t *const qspimram_conf);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              QspiMramOpen(qspimramInst_t *qspimram_inst, const qspimramConf_t *const qspimram_conf)
 * @brief           Function that initialise a QSPI MRAM memory
 * @param[in,out]   qspimram_inst   Instance that contains QSPI MRAM handlers
 * @param[in]       qspimram_conf   Configuration that contains QSPI MRAM parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 */
returnCode_t QspiMramOpen(qspimramInst_t *qspimram_inst, const qspimramConf_t *const qspimram_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((qspimram_inst != NULL) && (qspimram_conf != NULL))
    {
        // Init peripheral clock
        return_value = QspiMramInitClock(qspimram_inst, qspimram_conf);

        if (return_value == RET_SUCCESSFUL)
        {
            // Setup IOs
            return_value = QspiMramSetupIOs(qspimram_inst, qspimram_conf);
            if (return_value == RET_SUCCESSFUL)
            {
                // Setup QSPI MRAM
                qspimram_inst->handle_struct.Instance                = qspimram_conf->periph;
                qspimram_inst->handle_struct.Init.ClockPrescaler     = qspimram_conf->prescaler;
                qspimram_inst->handle_struct.Init.FifoThreshold      = 1u;
                qspimram_inst->handle_struct.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_NONE;
                qspimram_inst->handle_struct.Init.FlashSize          = QspiMramCalcFlashSize(qspimram_conf);
                qspimram_inst->handle_struct.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
                qspimram_inst->handle_struct.Init.ClockMode          = QSPI_CLOCK_MODE_0;
                qspimram_inst->handle_struct.Init.FlashID            = QSPI_FLASH_ID_1;
                qspimram_inst->handle_struct.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;
                // HAL QSPI initialization
                HAL_StatusTypeDef test_hal = HAL_QSPI_Init(&qspimram_inst->handle_struct);
                if (test_hal == HAL_OK)
                {
                    // Link the conf pointer
                    qspimram_inst->p_conf = qspimram_conf;
                    // Set qspimram inst as the interrupt parameter to pass it to the interrupt routine
                    IRQHandlerParam_t param = (IRQHandlerParam_t)&qspimram_inst->handle_struct;
                    // Request the interrupt
                    return_value = RequestIRQ(qspimram_conf->irq_no, qspimram_conf->irq_prio, QSPIGenericIRQHandler, param);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        // Setup QSPI mode
                        return_value = QspiMramSetupQSPIMode(qspimram_inst);
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
 * @fn          QspiMramWrite(qspimramInst_t *qspimram_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto an QSPI MRAM memory
 * @param[in]   qspimram_inst     Instance that contains QSPI MRAM parameters and QSPI MRAM Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer from which data will be copied
 * @param[in]   length      Number of sector that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if qspimram timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if qspimram is still sending previous message
 */
returnCode_t QspiMramWrite(qspimramInst_t *qspimram_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((qspimram_inst != NULL) && (length != 0u) && (data != NULL))
    {
        // Bound check on sector range
        if ((sector + length) <= (qspimram_inst->p_conf->size / QSPIMRAM_VIRTUAL_SECTOR_SIZE))
        {
            length_t nb_sector_written = 0u;
            // For each sector
            while ((nb_sector_written < length) && (return_value == RET_SUCCESSFUL))
            {
                // Base byte address of current sector
                const uint32_t sector_base_addr = (sector + nb_sector_written) * QSPIMRAM_VIRTUAL_SECTOR_SIZE;
                // Destination base index in user buffer for this sector
                const uint32_t sector_dst_base_idx = nb_sector_written * QSPIMRAM_VIRTUAL_SECTOR_SIZE;

                // First enable write
                return_value = QspiMramWriteEnable(qspimram_inst);
                if (return_value == RET_SUCCESSFUL)
                {
                    QSPI_CommandTypeDef qspi_command = { 0 };
                    qspi_command.InstructionMode     = QSPI_INSTRUCTION_4_LINES;
                    qspi_command.Instruction         = QSPIMRAM_FAST_WRITE_QUAD_CMD;
                    qspi_command.AddressMode         = QSPI_ADDRESS_4_LINES;
                    qspi_command.AddressSize         = QSPI_ADDRESS_24_BITS;
                    qspi_command.Address             = sector_base_addr;
                    qspi_command.DataMode            = QSPI_DATA_4_LINES;
                    qspi_command.NbData              = QSPIMRAM_VIRTUAL_SECTOR_SIZE;

                    // Send a page program command
                    HAL_StatusTypeDef test_hal = HAL_QSPI_Command(&qspimram_inst->handle_struct, &qspi_command, QSPIMRAM_TIMEOUT);
                    if (test_hal == HAL_OK)
                    {
                        // Then transmit page data
                        test_hal = HAL_QSPI_Transmit(&qspimram_inst->handle_struct, &((data_t)data)[sector_dst_base_idx], QSPIMRAM_TIMEOUT);
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

                nb_sector_written++;
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
 * @fn          QspiMramRead(qspimramInst_t *qspimram_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an QSPI MRAM memory
 * @param[in]   qspimram_inst     Instance that contains QSPI MRAM parameters and QSPI MRAM Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of sector that will be read
 * @retval      #RET_SUCCESSFUL if data has been read successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if qspimram timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if qspimram is still sending previous message
 */
returnCode_t QspiMramRead(qspimramInst_t *qspimram_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((qspimram_inst != NULL) && (data != NULL) && (length != 0u))
    {
        // Bound check on sector range
        if ((sector + length) <= (qspimram_inst->p_conf->size / QSPIMRAM_VIRTUAL_SECTOR_SIZE))
        {
            length_t nb_sector_read = 0u;
            // For each sector
            while ((nb_sector_read < length) && (return_value == RET_SUCCESSFUL))
            {
                // Base byte address of current sector
                const uint32_t sector_base_addr = (sector + nb_sector_read) * QSPIMRAM_VIRTUAL_SECTOR_SIZE;
                // Destination base index in user buffer for this sector
                const uint32_t sector_dst_base_idx = nb_sector_read * QSPIMRAM_VIRTUAL_SECTOR_SIZE;

                // Setup read command
                QSPI_CommandTypeDef qspi_command = { 0 };
                qspi_command.InstructionMode     = QSPI_INSTRUCTION_4_LINES;
                qspi_command.Instruction         = QSPIMRAM_FAST_READ_QUAD_CMD;
                qspi_command.AddressMode         = QSPI_ADDRESS_4_LINES;
                qspi_command.AddressSize         = QSPI_ADDRESS_24_BITS;
                qspi_command.Address             = sector_base_addr;
                qspi_command.DummyCycles         = QSPIMRAM_READ_DUMMY_CLOCK_CYCLES;
                qspi_command.DataMode            = QSPI_DATA_4_LINES;
                qspi_command.NbData              = QSPIMRAM_VIRTUAL_SECTOR_SIZE;

                // Send a read page command
                HAL_StatusTypeDef test_hal = HAL_QSPI_Command(&qspimram_inst->handle_struct, &qspi_command, QSPIMRAM_TIMEOUT);
                if (test_hal == HAL_OK)
                {
                    // Then read page data
                    test_hal = HAL_QSPI_Receive(&qspimram_inst->handle_struct, &((data_t)data)[sector_dst_base_idx], QSPIMRAM_TIMEOUT);
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

                nb_sector_read++;
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
 * @fn              QspiMramIoctl(qspimramInst_t *qspimram_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   qspimram_inst    Instance that contains QSPI MRAM handlers
 * @param[in]       cmd             IO Control command
 * @param[in,out]   data            IO Control command
 * @param[in]       data_size       IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t QspiMramIoctl(qspimramInst_t *qspimram_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (qspimram_inst != NULL)
    {
        switch (cmd)
        {
            case IOCTL_MEMORY_GET_STATUS :
                if (data_size == sizeof(memoryStatus_t))
                {
                    *(memoryStatus_t *)data = MEMORY_READY;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_SYNC :
                // Sync is not required for this QSPI MRAM card driver, so do nothing
                break;
            case IOCTL_MEMORY_GET_SECTOR_COUNT :
                if (data_size == sizeof(memorySectorCount_t))
                {
                    *(memorySectorCount_t *)data = qspimram_inst->p_conf->size / QSPIMRAM_VIRTUAL_SECTOR_SIZE;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_GET_SECTOR_SIZE :
                if (data_size == sizeof(memorySectorSize_t))
                {
                    *(memorySectorSize_t *)data = QSPIMRAM_VIRTUAL_SECTOR_SIZE;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_GET_BLOCK_SIZE :
                if (data_size == sizeof(memoryBlockSize_t))
                {
                    *(memoryBlockSize_t *)data = QSPIMRAM_BLOCK_SIZE_IN_SECTORS;
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
 * @fn              QspiMramClose(qspimramInst_t *qspimram_inst)
 * @brief           Function that desinit the QSPI MRAM connection
 * @param[in,out]   qspimram_inst   Instance that contains QSPI MRAM handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
returnCode_t QspiMramClose(qspimramInst_t *qspimram_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (qspimram_inst != NULL)
    {
        // TO DO
        (void)QspiMramDeInitClock(qspimram_inst);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              QspiMramInitClock(qspimramInst_t *qspimram_inst, const qspimramConf_t *const qspimram_conf)
 * @brief           Function that setups QSPI MRAM peripheral clock
 * @param[in,out]   qspimram_inst   Instance that contains QSPI MRAM handlers
 * @param[in]       qspimram_conf   Configuration that contains QSPI MRAM parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t QspiMramInitClock(qspimramInst_t *qspimram_inst, const qspimramConf_t *const qspimram_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(qspimram_inst);

    // Check parameter(s)
    if ((qspimram_inst != NULL) && (qspimram_conf != NULL))
    {
#if defined(STM32H7)
        RCC_PeriphCLKInitTypeDef nand_peripheral_clock_settings = { 0 };
        nand_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_QSPI;
        nand_peripheral_clock_settings.QspiClockSelection       = qspimram_conf->clk_src;
        if (HAL_RCCEx_PeriphCLKConfig(&nand_peripheral_clock_settings) == HAL_OK)
        {
            __HAL_RCC_QSPI_CLK_ENABLE();
        }
        else
        {
            return_value = RET_ERROR;
        }
#elif defined(STM32F4)
        __HAL_RCC_QSPI_CLK_ENABLE();
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
 * @fn              QspiMramDeInitClock(qspimramInst_t *qspimram_inst)
 * @brief           Function that disables QSPI MRAM peripheral clock
 * @param[in,out]   qspimram_inst   Instance that contains QSPI MRAM handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t QspiMramDeInitClock(qspimramInst_t *qspimram_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (qspimram_inst != NULL)
    {
        (void)(qspimram_inst);
        __HAL_RCC_QSPI_CLK_DISABLE();
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              QspiMramSetupIOs(qspimramInst_t *qspimram_inst, const qspimramConf_t *const qspimram_conf)
 * @brief           Function that setups IOs
 * @param[in,out]   qspimram_inst   Instance that contains QSPIMRAM handlers
 * @param[in]       qspimram_conf   Configuration that contains QSPIMRAM parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this QSPIMRAM
 */
static returnCode_t QspiMramSetupIOs(qspimramInst_t *qspimram_inst, const qspimramConf_t *const qspimram_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(qspimram_inst);

    // Check parameter(s)
    if ((qspimram_inst != NULL) && (qspimram_conf != NULL))
    {
        // First init SCK IO
        return_value = SetupIO(&qspimram_conf->io_sck);
        if (return_value == RET_SUCCESSFUL)
        {
            // Then init D0 IO
            return_value = SetupIO(&qspimram_conf->io_d0);
            if (return_value == RET_SUCCESSFUL)
            {
                // Then init D1 IO
                return_value = SetupIO(&qspimram_conf->io_d1);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Then init D2 IO
                    return_value = SetupIO(&qspimram_conf->io_d2);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        // Then init D3 IO
                        return_value = SetupIO(&qspimram_conf->io_d3);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // Then init CS IO
                            return_value = SetupIO(&qspimram_conf->io_cs);
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
 * @fn              QspiMramWriteEnable(qspimramInst_t *qspimram_inst)
 * @brief           Sends the Write Enable command to the MRAM flash
 * @param[in,out]   qspimram_inst   Instance that contains QSPI MRAM handlers
 * @retval          #RET_SUCCESSFUL if the command was sent successfully
 * @retval          #RET_NOT_AVAILABLE if the QSPI bus is busy
 * @retval          #RET_TIMEOUT if the QSPI timed out
 * @retval          #RET_ERROR if the QSPI encountered an error
 * @retval          #RET_INVALID_PARAM if instance pointer is null
 */
static returnCode_t QspiMramWriteEnable(qspimramInst_t *qspimram_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (qspimram_inst != NULL)
    {
        QSPI_CommandTypeDef qspi_command = { 0 };
        qspi_command.InstructionMode     = QSPI_INSTRUCTION_4_LINES;
        qspi_command.Instruction         = QSPIMRAM_WRITE_ENABLE_CMD;
        qspi_command.DataMode            = QSPI_DATA_NONE;
        qspi_command.NbData              = 0u;

        HAL_StatusTypeDef test_hal = HAL_QSPI_Command(&qspimram_inst->handle_struct, &qspi_command, QSPIMRAM_TIMEOUT);
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
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              QspiMramSetupQSPIMode(qspimramInst_t *qspimram_inst, uint8_t reg, uint8_t *status)
 * @brief           Setup the QUAD SPI mode for the MRAM flash
 * @param[in,out]   qspimram_inst   Instance that contains QSPI MRAM handlers
 * @retval          #RET_INVALID_PARAM if the status is a null pointer or reg is not a valid register
 * @retval          #RET_NOT_AVAILABLE if the QSPI bus is not available
 * @retval          #RET_TIMEOUT if the QSPI timeouted
 * @retval          #RET_ERROR if the QPSI encountered an error
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t QspiMramSetupQSPIMode(qspimramInst_t *qspimram_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (qspimram_inst != NULL)
    {
        QSPI_CommandTypeDef qspi_command = { 0 };
        qspi_command.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
        qspi_command.Instruction         = QSPIMRAM_QSPI_ENABLE_CMD;
        qspi_command.DataMode            = QSPI_DATA_NONE;
        qspi_command.NbData              = 0u;

        HAL_StatusTypeDef test_hal = HAL_QSPI_Command(&qspimram_inst->handle_struct, &qspi_command, QSPIMRAM_TIMEOUT);
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
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          QspiMramCalcFlashSize(const qspimramConf_t *const qspimram_conf)
 * @brief       Determines the flash size according to the ST HAL from QSPI MRAM flash conf
 * @param[in]   qspimram_conf Configuration that contains QSPI MRAM parameters
 * @return      FlashSize is effectively the number of address bits minus 1 required to address the flash memory
 */
static inline uint32_t QspiMramCalcFlashSize(const qspimramConf_t *const qspimram_conf)
{
    uint32_t flash_size = 0u;

    // Flash total size in bytes
    uint64_t total_size = (uint64_t)qspimram_conf->size;

    // Obtain log2 of the total size
    uint32_t n = 0u;
    while (total_size > 1u)
    {
        total_size >>= 1;
        n++;
    }

    if (n > 0u)
    {
        flash_size = n - 1u; // FlashSize = log2(total_size) - 1
    }

    return flash_size;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @fn      QSPIGenericIRQHandler(void *param)
 * @brief   Generic QSPI IRQ Handler
 */
static void QSPIGenericIRQHandler(void *param)
{
    // Get qspimram inst
    QSPI_HandleTypeDef *qspi_inst = (QSPI_HandleTypeDef *)param;

    // Do IRQ
    HAL_QSPI_IRQHandler(qspi_inst);
}
