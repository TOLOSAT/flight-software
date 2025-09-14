/**
 * @file    drv_qspinor.h
 * @author  Théo Bessel
 * @author  Merlin Kooshmanian
 * @brief   Source file for QSPI_NOR flash memory using QSPI bus
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/memories.h"
#include "drv/memories/drv_qspinor.h"
#include "core/irq.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

// Global constant
#define QSPINOR_TIMEOUT                     1000u                       /**< QSPINOR Card Timeout for ST HAL */
#define QSPINOR_MANDATORY_SECTOR_SIZE       4096u                       /**< Mandatory sector size in byte to make the erase easier */
#define QSPINOR_BLOCK_SIZE_IN_SECTORS       1u                          /**< Erase block size in sectors (fixed to 1 for QSPINOR) */
#define QSPINOR_READ_ALTERNATES_BYTES       0xF0u                       /**< Default alternates bytes for read */
#define QSPINOR_READ_ALTERNATES_BYTES_SIZE  QSPI_ALTERNATE_BYTES_8_BITS /**< Default alternates bytes size for read */
#define QSPINOR_READ_DUMMY_CLOCK_CYCLES     4u                          /**< Default dummy clock cycles for read */

// QSPI flash commands
#define QSPINOR_WRITE_ENABLE_CMD            0x06u /**< Write Enable command */
#define QSPINOR_FAST_READ_QUAD_CMD          0xebu /**< Fast Read Quad I/O */
#define QSPINOR_FAST_WRITE_QUAD_CMD         0x32u /**< Fast Write Quad I/O */
#define QSPINOR_SECTOR_ERASE_CMD            0x20u /**< Sector Erase command */

// Status registers constants
#define QSPINOR_REGISTER_READ_OFFSET        5u    /**< Register select read offset */
#define QSPINOR_REGISTER_WRITE_OFFSET       1u    /**< Register select write offset */
#define QSPINOR_STATUS_REGISTER_1_BASE_ADDR 0x00u /**< Register 1 'base address' used by read/write register command */
#define QSPINOR_STATUS_REGISTER_2_BASE_ADDR 0x30u /**< Register 2 'base address' used by read/write register command */
#define QSPINOR_STATUS_REGISTER_3_BASE_ADDR 0x10u /**< Register 3 'base address' used by read/write register command */
#define QSPINOR_QUAD_ENABLE_BIT             0x02u /**< Quad enable bit */
#define QSPINOR_BUSY_BIT                    0x01u /**< Busy bit */

// Getters

/**
 * @brief Get the page size in bytes
 */
#define GET_PAGE_SIZE_BYTES(qspinor_inst)   ((qspinor_inst)->p_conf->page_size)

/**
 * @brief Get the sector size in bytes
 */
#define GET_SECTOR_SIZE_BYTES(qspinor_inst) ((qspinor_inst)->p_conf->sector_size)

/*************************** Functions Declarations **************************/

static void QSPIGenericIRQHandler(void *param);
static returnCode_t QspiNorInitClock(qspinorInst_t *qspinor_inst, const qspinorConf_t *const qspinor_conf);
static returnCode_t QspiNorDeInitClock(qspinorInst_t *qspinor_inst);
static returnCode_t QspiNorSetupIOs(qspinorInst_t *qspinor_inst, const qspinorConf_t *const qspinor_conf);
static bool QspiNorIsBusy(qspinorInst_t *qspinor_inst);
static returnCode_t QspiNorWriteEnable(qspinorInst_t *qspinor_inst);
static returnCode_t QspiNorReadStatusRegister(qspinorInst_t *qspinor_inst, uint8_t reg, uint8_t *content);
static returnCode_t QspiNorWriteStatusRegister(qspinorInst_t *qspinor_inst, uint8_t reg, uint8_t *content);
static returnCode_t QspiNorSetupQSPIMode(qspinorInst_t *qspinor_inst);
static returnCode_t QspiNorEraseSector(qspinorInst_t *qspinor_inst, uint32_t sector_address);
static returnCode_t QspiNorProgramPage(qspinorInst_t *qspinor_inst, uint32_t page_addr, data_t data);
static returnCode_t QspiNorReadPage(qspinorInst_t *qspinor_inst, uint32_t page_addr, data_t data);
static inline uint32_t QspiNorCalcFlashSize(const qspinorConf_t *const qspinor_conf);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              QspiNorOpen(qspinorInst_t *qspinor_inst, const qspinorConf_t *const qspinor_conf)
 * @brief           Function that initialise a QSPI NOR memory
 * @param[in,out]   qspinor_inst   Instance that contains QSPI NOR handlers
 * @param[in]       qspinor_conf   Configuration that contains QSPI NOR parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 */
returnCode_t QspiNorOpen(qspinorInst_t *qspinor_inst, const qspinorConf_t *const qspinor_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((qspinor_inst != NULL) && (qspinor_conf != NULL))
    {
        // First check that the sector size is correct (this driver only supports 4kB sectors)
        if (qspinor_conf->sector_size == QSPINOR_MANDATORY_SECTOR_SIZE)
        {
            // Init peripheral clock
            return_value = QspiNorInitClock(qspinor_inst, qspinor_conf);

            if (return_value == RET_SUCCESSFUL)
            {
                // Setup IOs
                return_value = QspiNorSetupIOs(qspinor_inst, qspinor_conf);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Setup QSPI NOR
                    qspinor_inst->handle_struct.Instance                = qspinor_conf->periph;
                    qspinor_inst->handle_struct.Init.ClockPrescaler     = qspinor_conf->prescaler;
                    qspinor_inst->handle_struct.Init.FifoThreshold      = 1u;
                    qspinor_inst->handle_struct.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_NONE;
                    qspinor_inst->handle_struct.Init.FlashSize          = QspiNorCalcFlashSize(qspinor_conf);
                    qspinor_inst->handle_struct.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
                    qspinor_inst->handle_struct.Init.ClockMode          = QSPI_CLOCK_MODE_0;
                    qspinor_inst->handle_struct.Init.FlashID            = QSPI_FLASH_ID_1;
                    qspinor_inst->handle_struct.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;
                    // HAL QSPI initialization
                    HAL_StatusTypeDef test_hal = HAL_QSPI_Init(&qspinor_inst->handle_struct);
                    if (test_hal == HAL_OK)
                    {
                        // Link the conf pointer
                        qspinor_inst->p_conf = qspinor_conf;
                        // Set qspinor inst as the interrupt parameter to pass it to the interrupt routine
                        IRQHandlerParam_t param = (IRQHandlerParam_t)&qspinor_inst->handle_struct;
                        // Request the interrupt
                        return_value = RequestIRQ(qspinor_conf->irq_no, qspinor_conf->irq_prio, QSPIGenericIRQHandler, param);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // Setup QSPI mode
                            return_value = QspiNorSetupQSPIMode(qspinor_inst);
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
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          QspiNorWrite(qspinorInst_t *qspinor_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto an QSPI NOR memory
 * @param[in]   qspinor_inst     Instance that contains QSPI NOR parameters and QSPI NOR Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer from which data will be copied
 * @param[in]   length      Number of sector that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if qspinor timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if qspinor is still sending previous message
 */
returnCode_t QspiNorWrite(qspinorInst_t *qspinor_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((qspinor_inst != NULL) && (length != 0u) && (data != NULL))
    {
        // Bound check on sector range
        if ((sector + length) <= qspinor_inst->p_conf->nb_sector)
        {
            length_t nb_sector_written = 0u;
            // For each sector
            while ((nb_sector_written < length) && (return_value == RET_SUCCESSFUL))
            {
                // Base byte address of current sector
                const uint32_t sector_base_addr = (sector + nb_sector_written) * GET_SECTOR_SIZE_BYTES(qspinor_inst);
                // Erase the sector
                return_value = QspiNorEraseSector(qspinor_inst, sector_base_addr);
                if (return_value == RET_SUCCESSFUL)
                {
                    length_t nb_page_written = 0u;
                    // Destination base index in user buffer for this sector
                    const uint32_t sector_dst_base_idx = nb_sector_written * GET_SECTOR_SIZE_BYTES(qspinor_inst);
                    // For each page
                    while ((nb_page_written < GET_SECTOR_SIZE_BYTES(qspinor_inst) / GET_PAGE_SIZE_BYTES(qspinor_inst))
                           && (return_value == RET_SUCCESSFUL))
                    {
                        // Base byte address of current page
                        const uint32_t page_address = sector_base_addr + (nb_page_written * GET_PAGE_SIZE_BYTES(qspinor_inst));
                        // Destination base index in user buffer for this page
                        const uint32_t dst_index = sector_dst_base_idx + (nb_page_written * GET_PAGE_SIZE_BYTES(qspinor_inst));
                        // Read Page
                        return_value = QspiNorProgramPage(qspinor_inst, page_address, &((data_t)data)[dst_index]);
                        nb_page_written++;
                    }
                    nb_sector_written++;
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
 * @fn          QspiNorRead(qspinorInst_t *qspinor_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an QSPI NOR memory
 * @param[in]   qspinor_inst     Instance that contains QSPI NOR parameters and QSPI NOR Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of sector that will be read
 * @retval      #RET_SUCCESSFUL if data has been read successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if qspinor timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if qspinor is still sending previous message
 */
returnCode_t QspiNorRead(qspinorInst_t *qspinor_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((qspinor_inst != NULL) && (data != NULL) && (length != 0u))
    {
        // Bound check on sector range
        if ((sector + length) <= qspinor_inst->p_conf->nb_sector)
        {
            length_t nb_sector_read = 0u;
            // For each sector
            while ((nb_sector_read < length) && (return_value == RET_SUCCESSFUL))
            {
                length_t nb_page_read = 0u;
                // Base byte address of current sector
                const uint32_t sector_base_addr = (sector + nb_sector_read) * GET_SECTOR_SIZE_BYTES(qspinor_inst);
                // Destination base index in user buffer for this sector
                const uint32_t sector_dst_base_idx = nb_sector_read * GET_SECTOR_SIZE_BYTES(qspinor_inst);
                // For each page
                while ((nb_page_read < GET_SECTOR_SIZE_BYTES(qspinor_inst) / GET_PAGE_SIZE_BYTES(qspinor_inst)) && (return_value == RET_SUCCESSFUL))
                {
                    // Base byte address of current page
                    const uint32_t page_address = sector_base_addr + (nb_page_read * GET_PAGE_SIZE_BYTES(qspinor_inst));
                    // Destination base index in user buffer for this page
                    const uint32_t dst_index = sector_dst_base_idx + (nb_page_read * GET_PAGE_SIZE_BYTES(qspinor_inst));
                    // Read Page
                    return_value = QspiNorReadPage(qspinor_inst, page_address, &((data_t)data)[dst_index]);
                    nb_page_read++;
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
 * @fn              QspiNorIoctl(qspinorInst_t *qspinor_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   qspinor_inst    Instance that contains QSPI NOR handlers
 * @param[in]       cmd             IO Control command
 * @param[in,out]   data            IO Control command
 * @param[in]       data_size       IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t QspiNorIoctl(qspinorInst_t *qspinor_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (qspinor_inst != NULL)
    {
        switch (cmd)
        {
            case IOCTL_MEMORY_GET_STATUS :
                if (data_size == sizeof(memoryStatus_t))
                {
                    if (QspiNorIsBusy(qspinor_inst) == false)
                    {
                        *(memoryStatus_t *)data = MEMORY_READY;
                    }
                    else
                    {
                        return_value = RET_NOT_AVAILABLE;
                    }
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_SYNC :
                // Sync is not required for this QSPI NOR card driver, so do nothing
                break;
            case IOCTL_MEMORY_GET_SECTOR_COUNT :
                if (data_size == sizeof(memorySectorCount_t))
                {
                    *(memorySectorCount_t *)data = qspinor_inst->p_conf->nb_sector;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_GET_SECTOR_SIZE :
                if (data_size == sizeof(memorySectorSize_t))
                {
                    *(memorySectorSize_t *)data = GET_SECTOR_SIZE_BYTES(qspinor_inst);
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_GET_BLOCK_SIZE :
                if (data_size == sizeof(memoryBlockSize_t))
                {
                    *(memoryBlockSize_t *)data = QSPINOR_BLOCK_SIZE_IN_SECTORS;
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
 * @fn              QspiNorClose(qspinorInst_t *qspinor_inst)
 * @brief           Function that desinit the QSPI NOR connection
 * @param[in,out]   qspinor_inst   Instance that contains QSPI NOR handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
returnCode_t QspiNorClose(qspinorInst_t *qspinor_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (qspinor_inst != NULL)
    {
        // TO DO
        (void)QspiNorDeInitClock(qspinor_inst);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              QspiNorInitClock(qspinorInst_t *qspinor_inst, const qspinorConf_t *const qspinor_conf)
 * @brief           Function that setups QSPI NOR peripheral clock
 * @param[in,out]   qspinor_inst   Instance that contains QSPI NOR handlers
 * @param[in]       qspinor_conf   Configuration that contains QSPI NOR parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t QspiNorInitClock(qspinorInst_t *qspinor_inst, const qspinorConf_t *const qspinor_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(qspinor_inst);

    // Check parameter(s)
    if ((qspinor_inst != NULL) && (qspinor_conf != NULL))
    {
#if defined(STM32H7)
        RCC_PeriphCLKInitTypeDef nand_peripheral_clock_settings = { 0 };
        nand_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_QSPI;
        nand_peripheral_clock_settings.QspiClockSelection       = qspinor_conf->clk_src;
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
 * @fn              QspiNorDeInitClock(qspinorInst_t *qspinor_inst)
 * @brief           Function that disables QSPI NOR peripheral clock
 * @param[in,out]   qspinor_inst   Instance that contains QSPI NOR handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t QspiNorDeInitClock(qspinorInst_t *qspinor_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (qspinor_inst != NULL)
    {
        (void)(qspinor_inst);
        __HAL_RCC_QSPI_CLK_DISABLE();
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              QspiNorSetupIOs(qspinorInst_t *qspinor_inst, const qspinorConf_t *const qspinor_conf)
 * @brief           Function that setups IOs
 * @param[in,out]   qspinor_inst   Instance that contains QSPINOR handlers
 * @param[in]       qspinor_conf   Configuration that contains QSPINOR parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this QSPINOR
 */
static returnCode_t QspiNorSetupIOs(qspinorInst_t *qspinor_inst, const qspinorConf_t *const qspinor_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(qspinor_inst);

    // Check parameter(s)
    if ((qspinor_inst != NULL) && (qspinor_conf != NULL))
    {
        // First init SCK IO
        return_value = SetupIO(&qspinor_conf->io_sck);
        if (return_value == RET_SUCCESSFUL)
        {
            // Then init D0 IO
            return_value = SetupIO(&qspinor_conf->io_d0);
            if (return_value == RET_SUCCESSFUL)
            {
                // Then init D1 IO
                return_value = SetupIO(&qspinor_conf->io_d1);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Then init D2 IO
                    return_value = SetupIO(&qspinor_conf->io_d2);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        // Then init D3 IO
                        return_value = SetupIO(&qspinor_conf->io_d3);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // Then init CS IO
                            return_value = SetupIO(&qspinor_conf->io_cs);
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
 * @fn              QspiNorIsBusy(qspinorInst_t *qspinor_inst)
 * @brief           Indicates if the QSPI NOR is busy or not
 * @param[in,out]   qspinor_inst   Instance that contains QSPI NOR handlers
 * @return          true if the memory is busy else false
 */
static bool QspiNorIsBusy(qspinorInst_t *qspinor_inst)
{
    bool busy = true;

    // Check parameter(s)
    if (qspinor_inst != NULL)
    {
        QSPI_CommandTypeDef qspi_command = { 0 };
        qspi_command.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
        qspi_command.Instruction         = QSPINOR_STATUS_REGISTER_1_BASE_ADDR + QSPINOR_REGISTER_READ_OFFSET;
        qspi_command.DataMode            = QSPI_DATA_1_LINE;
        qspi_command.NbData              = 0x1u;
        // Send read register command
        HAL_StatusTypeDef test_hal = HAL_QSPI_Command(&qspinor_inst->handle_struct, &qspi_command, QSPINOR_TIMEOUT);
        if (test_hal == HAL_OK)
        {
            uint8_t content = 0u;
            // Read register data
            test_hal = HAL_QSPI_Receive(&qspinor_inst->handle_struct, &content, QSPINOR_TIMEOUT);
            if (test_hal == HAL_OK)
            {
                busy = ((content & QSPINOR_BUSY_BIT) == QSPINOR_BUSY_BIT);
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
        // Can't handle an error in this function so KernelPanic
        KernelPanic();
    }

    return busy;
}

/**
 * @fn              QspiNorWriteEnable(qspinorInst_t *qspinor_inst)
 * @brief           Sends the Write Enable command to the NOR flash
 * @param[in,out]   qspinor_inst   Instance that contains QSPI NOR handlers
 * @retval          #RET_SUCCESSFUL if the command was sent successfully
 * @retval          #RET_NOT_AVAILABLE if the QSPI bus is busy
 * @retval          #RET_TIMEOUT if the QSPI timed out
 * @retval          #RET_ERROR if the QSPI encountered an error
 * @retval          #RET_INVALID_PARAM if instance pointer is null
 */
static returnCode_t QspiNorWriteEnable(qspinorInst_t *qspinor_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (qspinor_inst != NULL)
    {
        QSPI_CommandTypeDef qspi_command = { 0 };
        qspi_command.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
        qspi_command.Instruction         = QSPINOR_WRITE_ENABLE_CMD;
        qspi_command.DataMode            = QSPI_DATA_NONE;
        qspi_command.NbData              = 0u;

        HAL_StatusTypeDef test_hal = HAL_QSPI_Command(&qspinor_inst->handle_struct, &qspi_command, QSPINOR_TIMEOUT);
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
 * @fn              QspiNorReadStatusRegister(qspinorInst_t *qspinor_inst, uint8_t reg, uint8_t *content)
 * @brief           Read a status register of the NOR flash
 * @param[in,out]   qspinor_inst   Instance that contains QSPI NOR handlers
 * @param[in]       reg             Register status base address
 * @param[out]      content         Content from the status register
 * @retval          #RET_INVALID_PARAM if the status is a null pointer or reg is not a valid register
 * @retval          #RET_NOT_AVAILABLE if the QSPI bus is not available
 * @retval          #RET_TIMEOUT if the QSPI timeouted
 * @retval          #RET_ERROR if the QPSI encountered an error
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t QspiNorReadStatusRegister(qspinorInst_t *qspinor_inst, uint8_t reg, uint8_t *content)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((qspinor_inst != NULL) && (content != NULL)
        && ((reg == QSPINOR_STATUS_REGISTER_1_BASE_ADDR) || (reg == QSPINOR_STATUS_REGISTER_2_BASE_ADDR)
            || (reg == QSPINOR_STATUS_REGISTER_3_BASE_ADDR)))
    {
        // Check if QSPI NOR available
        if (QspiNorIsBusy(qspinor_inst) == false)
        {
            QSPI_CommandTypeDef qspi_command = { 0 };
            qspi_command.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
            qspi_command.Instruction         = reg + QSPINOR_REGISTER_READ_OFFSET;
            qspi_command.DataMode            = QSPI_DATA_1_LINE;
            qspi_command.NbData              = 0x1u;

            // Send read register command
            HAL_StatusTypeDef test_hal = HAL_QSPI_Command(&qspinor_inst->handle_struct, &qspi_command, QSPINOR_TIMEOUT);
            if (test_hal == HAL_OK)
            {
                // Read register data
                test_hal = HAL_QSPI_Receive(&qspinor_inst->handle_struct, content, QSPINOR_TIMEOUT);
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
        else
        {
            return_value = RET_NOT_AVAILABLE;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              QspiNorWriteStatusRegister(qspinorInst_t *qspinor_inst, uint8_t reg, uint8_t *content)
 * @brief           Write a status register of the NOR flash
 * @param[in,out]   qspinor_inst   Instance that contains QSPI NOR handlers
 * @param[in]       reg Register status base address
 * @param[in]       content  Content to write the status register
 * @retval          #RET_INVALID_PARAM if the status is a null pointer or reg is not a valid register
 * @retval          #RET_NOT_AVAILABLE if the QSPI bus is not available
 * @retval          #RET_TIMEOUT if the QSPI timeouted
 * @retval          #RET_ERROR if the QPSI encountered an error
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t QspiNorWriteStatusRegister(qspinorInst_t *qspinor_inst, uint8_t reg, uint8_t *content)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((qspinor_inst != NULL) && (content != NULL)
        && ((reg == QSPINOR_STATUS_REGISTER_1_BASE_ADDR) || (reg == QSPINOR_STATUS_REGISTER_2_BASE_ADDR)
            || (reg == QSPINOR_STATUS_REGISTER_3_BASE_ADDR)))
    {
        // Check if QSPI NOR available
        if (QspiNorIsBusy(qspinor_inst) == false)
        {
            // First enable write
            return_value = QspiNorWriteEnable(qspinor_inst);
            if (return_value == RET_SUCCESSFUL)
            {
                QSPI_CommandTypeDef qspi_command = { 0 };
                qspi_command.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
                qspi_command.Instruction         = reg + QSPINOR_REGISTER_WRITE_OFFSET;
                qspi_command.DataMode            = QSPI_DATA_1_LINE;
                qspi_command.NbData              = 0x1u;

                // Send write register command
                HAL_StatusTypeDef test_hal = HAL_QSPI_Command(&qspinor_inst->handle_struct, &qspi_command, QSPINOR_TIMEOUT);
                if (test_hal == HAL_OK)
                {
                    // Then write register data
                    test_hal = HAL_QSPI_Transmit(&qspinor_inst->handle_struct, content, QSPINOR_TIMEOUT);
                    if (test_hal == HAL_OK)
                    {
                        uint32_t tickstart = HAL_GetTick();

                        // Loop until QSPI is ready or timeouted
                        while ((QspiNorIsBusy(qspinor_inst) == true) && ((HAL_GetTick() - tickstart) < QSPINOR_TIMEOUT))
                        {
                            __NOP();
                        }
                        if ((HAL_GetTick() - tickstart) >= QSPINOR_TIMEOUT)
                        {
                            return_value = RET_TIMEOUT;
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
        else
        {
            return_value = RET_NOT_AVAILABLE;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              QspiNorSetupQSPIMode(qspinorInst_t *qspinor_inst, uint8_t reg, uint8_t *status)
 * @brief           Setup the QUAD SPI mode for the NOR flash
 * @param[in,out]   qspinor_inst   Instance that contains QSPI NOR handlers
 * @retval          #RET_INVALID_PARAM if the status is a null pointer or reg is not a valid register
 * @retval          #RET_NOT_AVAILABLE if the QSPI bus is not available
 * @retval          #RET_TIMEOUT if the QSPI timeouted
 * @retval          #RET_ERROR if the QPSI encountered an error
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t QspiNorSetupQSPIMode(qspinorInst_t *qspinor_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (qspinor_inst != NULL)
    {
        uint8_t status = 0u;
        // Read status
        return_value = QspiNorReadStatusRegister(qspinor_inst, QSPINOR_STATUS_REGISTER_2_BASE_ADDR, &status);
        if (return_value == RET_SUCCESSFUL)
        {
            // Enable QSPI mode by switching on the QUAD ENABLE bit
            status |= QSPINOR_QUAD_ENABLE_BIT;
            // Read status
            return_value = QspiNorWriteStatusRegister(qspinor_inst, QSPINOR_STATUS_REGISTER_2_BASE_ADDR, &status);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              QspiNorEraseSector(qspinorInst_t *qspinor_inst, uint32_t sector_address)
 * @brief           Erase a sector from the NOR flash
 * @param[in,out]   qspinor_inst    Instance that contains QSPI NOR handlers
 * @param[in]       sector_address  Sector address in bytes
 * @retval          #RET_INVALID_PARAM if the there is a null pointer
 * @retval          #RET_INVALID_PARAM if the sector address is not aligned with sector size
 * @retval          #RET_NOT_AVAILABLE if the QSPI bus is not available
 * @retval          #RET_TIMEOUT if the QSPI timeouted
 * @retval          #RET_ERROR if the QPSI encountered an error
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t QspiNorEraseSector(qspinorInst_t *qspinor_inst, uint32_t sector_address)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((qspinor_inst != NULL) && (sector_address % GET_SECTOR_SIZE_BYTES(qspinor_inst) == 0u))
    {
        // Check if QSPI NOR available
        if (QspiNorIsBusy(qspinor_inst) == false)
        {
            // First enable write
            return_value = QspiNorWriteEnable(qspinor_inst);
            if (return_value == RET_SUCCESSFUL)
            {
                QSPI_CommandTypeDef qspi_command = { 0 };
                qspi_command.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
                qspi_command.Instruction         = QSPINOR_SECTOR_ERASE_CMD;
                qspi_command.AddressMode         = QSPI_ADDRESS_1_LINE;
                qspi_command.AddressSize         = QSPI_ADDRESS_24_BITS;
                qspi_command.Address             = sector_address;

                // Send a sector erase command
                HAL_StatusTypeDef test_hal = HAL_QSPI_Command(&qspinor_inst->handle_struct, &qspi_command, QSPINOR_TIMEOUT);
                if (test_hal == HAL_OK)
                {
                    uint32_t tickstart = HAL_GetTick();

                    // Loop until QSPI is ready or timeouted
                    while ((QspiNorIsBusy(qspinor_inst) == true) && ((HAL_GetTick() - tickstart) < QSPINOR_TIMEOUT))
                    {
                        __NOP();
                    }
                    if ((HAL_GetTick() - tickstart) >= QSPINOR_TIMEOUT)
                    {
                        return_value = RET_TIMEOUT;
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
            return_value = RET_NOT_AVAILABLE;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              QspiNorProgramPage(qspinorInst_t *qspinor_inst, uint32_t page_addr, data_t data)
 * @brief           Program a page from the NOR flash
 * @param[in,out]   qspinor_inst    Instance that contains QSPI NOR handlers
 * @param[in]       page_addr       Page address in bytes
 * @param[in]       data            Page data
 * @retval          #RET_INVALID_PARAM if the there is a null pointer
 * @retval          #RET_INVALID_PARAM if the page address is not aligned with page size
 * @retval          #RET_NOT_AVAILABLE if the QSPI bus is not available
 * @retval          #RET_TIMEOUT if the QSPI timeouted
 * @retval          #RET_ERROR if the QPSI encountered an error
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t QspiNorProgramPage(qspinorInst_t *qspinor_inst, uint32_t page_addr, data_t data)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((qspinor_inst != NULL) && (data != NULL) && (page_addr % GET_PAGE_SIZE_BYTES(qspinor_inst) == 0u))
    {
        // Check if QSPI NOR available
        if (QspiNorIsBusy(qspinor_inst) == false)
        {
            // First enable write
            return_value = QspiNorWriteEnable(qspinor_inst);
            if (return_value == RET_SUCCESSFUL)
            {
                QSPI_CommandTypeDef qspi_command = { 0 };
                qspi_command.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
                qspi_command.Instruction         = QSPINOR_FAST_WRITE_QUAD_CMD;
                qspi_command.AddressMode         = QSPI_ADDRESS_1_LINE;
                qspi_command.AddressSize         = QSPI_ADDRESS_24_BITS;
                qspi_command.Address             = page_addr;
                qspi_command.DataMode            = QSPI_DATA_4_LINES;
                qspi_command.NbData              = GET_PAGE_SIZE_BYTES(qspinor_inst);

                // Send a page program command
                HAL_StatusTypeDef test_hal = HAL_QSPI_Command(&qspinor_inst->handle_struct, &qspi_command, QSPINOR_TIMEOUT);
                if (test_hal == HAL_OK)
                {
                    // Then transmit page data
                    test_hal = HAL_QSPI_Transmit(&qspinor_inst->handle_struct, data, QSPINOR_TIMEOUT);
                    if (test_hal == HAL_OK)
                    {
                        uint32_t tickstart = HAL_GetTick();

                        // Loop until QSPI is ready or timeouted
                        while ((QspiNorIsBusy(qspinor_inst) == true) && ((HAL_GetTick() - tickstart) < QSPINOR_TIMEOUT))
                        {
                            __NOP();
                        }
                        if ((HAL_GetTick() - tickstart) >= QSPINOR_TIMEOUT)
                        {
                            return_value = RET_TIMEOUT;
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
        else
        {
            return_value = RET_NOT_AVAILABLE;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              QspiNorReadPage(qspinorInst_t *qspinor_inst, uint32_t page_addr, data_t data)
 * @brief           Read a page from the NOR flash
 * @param[in,out]   qspinor_inst    Instance that contains QSPI NOR handlers
 * @param[in]       page_addr       Page address in bytes
 * @param[out]      data            Page data
 * @retval          #RET_INVALID_PARAM if the there is a null pointer
 * @retval          #RET_INVALID_PARAM if the page address is not aligned with page size
 * @retval          #RET_NOT_AVAILABLE if the QSPI bus is not available
 * @retval          #RET_TIMEOUT if the QSPI timeouted
 * @retval          #RET_ERROR if the QPSI encountered an error
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t QspiNorReadPage(qspinorInst_t *qspinor_inst, uint32_t page_addr, data_t data)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((qspinor_inst != NULL) && (data != NULL) && (page_addr % GET_PAGE_SIZE_BYTES(qspinor_inst) == 0u))
    {
        // Check if QSPI NOR available
        if (QspiNorIsBusy(qspinor_inst) == false)
        {
            QSPI_CommandTypeDef qspi_command = { 0 };
            qspi_command.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
            qspi_command.Instruction         = QSPINOR_FAST_READ_QUAD_CMD;
            qspi_command.AddressMode         = QSPI_ADDRESS_4_LINES;
            qspi_command.AddressSize         = QSPI_ADDRESS_24_BITS;
            qspi_command.Address             = page_addr;
            qspi_command.AlternateByteMode   = QSPI_ALTERNATE_BYTES_4_LINES;
            qspi_command.AlternateBytes      = QSPINOR_READ_ALTERNATES_BYTES;
            qspi_command.AlternateBytesSize  = QSPINOR_READ_ALTERNATES_BYTES_SIZE;
            qspi_command.DummyCycles         = QSPINOR_READ_DUMMY_CLOCK_CYCLES;
            qspi_command.DataMode            = QSPI_DATA_4_LINES;
            qspi_command.NbData              = GET_PAGE_SIZE_BYTES(qspinor_inst);

            // Send a read page command
            HAL_StatusTypeDef test_hal = HAL_QSPI_Command(&qspinor_inst->handle_struct, &qspi_command, QSPINOR_TIMEOUT);
            if (test_hal == HAL_OK)
            {
                // Then read page data
                test_hal = HAL_QSPI_Receive(&qspinor_inst->handle_struct, data, QSPINOR_TIMEOUT);
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
        else
        {
            return_value = RET_NOT_AVAILABLE;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          QspiNorCalcFlashSize(const qspinorConf_t *const qspinor_conf)
 * @brief       Determines the flash size according to the ST HAL from QSPI NOR flash conf
 * @param[in]   qspinor_conf Configuration that contains QSPI NOR parameters
 * @return      FlashSize is effectively the number of address bits minus 1 required to address the flash memory
 */
static inline uint32_t QspiNorCalcFlashSize(const qspinorConf_t *const qspinor_conf)
{
    uint32_t flash_size = 0u;

    // Flash total size in bytes
    uint64_t total_size = (uint64_t)qspinor_conf->sector_size * (uint64_t)qspinor_conf->nb_sector;

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
    // Get qspinor inst
    QSPI_HandleTypeDef *qspi_inst = (QSPI_HandleTypeDef *)param;

    // Do IRQ
    HAL_QSPI_IRQHandler(qspi_inst);
}
