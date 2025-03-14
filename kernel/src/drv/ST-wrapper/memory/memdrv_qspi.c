/**
 * @file    memdrv_qspi.c
 * @author  Théo Bessel
 * @brief   Source file for QSPI Memory driver
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/memory/memdrv_qspi.h"
#include "fdir/fdir.h"
#include <string.h>

/***************************** Macros Definitions ****************************/

// QUADSPI defines
#define QSPI_WRITE_ENABLE_CMD        0x06 /**< Write Enable command */
#define QSPI_OUT_FAST_READ_CMD       0x6B /**< Quad Output Fast Read command */
#define QSPI_DUMMY_CLOCK_CYCLES_READ 8   /**< Number of dummy cycles for Quad Read */
#define QSPI_IN_FAST_PROG_CMD        0x32 /**< Quad Input Fast Program command */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

QSPI_HandleTypeDef qspi_inst;

#if defined(CONFIG_CTX_MEM_QSPI_NAND)
extern uint32_t __qspi_start__;
extern uint32_t __qspi_end__;

#endif /* CONFIG_CTX_MEM_QSPI_NAND */

/*************************** Functions Definitions ***************************/

void QspiNandInit(void)
{
    QSPI_CommandTypeDef qspi_command;
    QSPI_MemoryMappedTypeDef qspi_mem_mapped;
    // HAL_StatusTypeDef status;

    /* Initialize the QSPI memory bus */
    qspi_inst.Instance                = QUADSPI;
    qspi_inst.Init.ClockPrescaler     = 4;
    qspi_inst.Init.FifoThreshold      = 1;
    qspi_inst.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_NONE;
    qspi_inst.Init.FlashSize          = 22;
    qspi_inst.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
    qspi_inst.Init.ClockMode          = QSPI_CLOCK_MODE_0;
    qspi_inst.Init.FlashID            = QSPI_FLASH_ID_1;
    qspi_inst.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;

    // HAL_QSPI_DeInit(&qspi_inst);
    if (HAL_QSPI_Init(&qspi_inst) != HAL_OK)
    {
        ErrorHandler();
    }

    /* Enable the QSPI write operations */
    qspi_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    qspi_command.Instruction       = QSPI_WRITE_ENABLE_CMD;
    qspi_command.AddressMode       = QSPI_ADDRESS_NONE;
    qspi_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    qspi_command.DataMode          = QSPI_DATA_NONE;
    qspi_command.DummyCycles       = 0;
    qspi_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    qspi_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    qspi_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        ErrorHandler();
    }

    // /* 4. Déverrouillage des blocs (optionnel, si nécessaire) */
    // qspi_command.Instruction = 0x98; // Global Block Unlock (98h)
    // qspi_command.DataMode = QSPI_DATA_NONE;

    // status = HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);

    // if (status != HAL_OK)
    // {
    //     ErrorHandler();
    // }

    /* Enable the QSPI memory mapped mode */
    qspi_command.InstructionMode      = QSPI_INSTRUCTION_1_LINE;
    qspi_command.AddressSize          = QSPI_ADDRESS_24_BITS;
    qspi_command.AlternateByteMode    = QSPI_ALTERNATE_BYTES_NONE;
    qspi_command.DdrMode              = QSPI_DDR_MODE_DISABLE;
    qspi_command.DdrHoldHalfCycle     = QSPI_DDR_HHC_ANALOG_DELAY;
    qspi_command.SIOOMode             = QSPI_SIOO_INST_EVERY_CMD;
    qspi_command.AddressMode          = QSPI_ADDRESS_1_LINE;
    qspi_command.DataMode             = QSPI_DATA_4_LINES;
    qspi_command.NbData               = 0;
    qspi_command.Address              = 0;
    qspi_command.Instruction          = QSPI_OUT_FAST_READ_CMD;
    qspi_command.DummyCycles          = QSPI_DUMMY_CLOCK_CYCLES_READ;
    qspi_mem_mapped.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;

    if (HAL_QSPI_MemoryMapped(&qspi_inst, &qspi_command, &qspi_mem_mapped) != HAL_OK)
    {
        ErrorHandler();
    }
}

/**
 * @fn          CtxMemRead(uint8_t *data, uint32_t addr, uint32_t offset, uint32_t len)
 * @brief       Function that reads the context memory
 * @param[out]  data    Pointer to the data that will be read
 * @param[in]   addr    Address of the data that will be read
 * @param[in]   offset  Offset in the data that will be read
 * @param[in]   len     Number of block that will be read
 * @retval      #RET_INVALID_PARAM if len equal zero or pointer is null
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t CtxMemRead(uint8_t *data, uint32_t addr, uint32_t offset, uint32_t len)
{
    QSPI_CommandTypeDef qspi_command;
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((data != NULL) && (len != 0u))
    {
        qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
        qspi_command.Instruction = QSPI_OUT_FAST_READ_CMD;
        qspi_command.AddressMode = QSPI_ADDRESS_1_LINE;
        qspi_command.AddressSize = QSPI_ADDRESS_32_BITS;
        qspi_command.Address = addr + offset;
        qspi_command.DataMode = QSPI_DATA_4_LINES;
        qspi_command.NbData = len;
        qspi_command.DummyCycles = QSPI_DUMMY_CLOCK_CYCLES_READ;
        qspi_command.DdrMode = QSPI_DDR_MODE_DISABLE;
        qspi_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
        qspi_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

        if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
            return_value = RET_ERROR;
        }

        if (HAL_QSPI_Receive(&qspi_inst, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
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

/**
 * @fn          CtxMemWrite(const uint8_t *data, uint32_t addr, uint32_t offset, uint32_t len)
 * @brief       Function that writes into the context memory
 * @param[in]   data    Pointer to the data that will be written
 * @param[in]   addr    Address of the data that will be written
 * @param[in]   offset  Offset in the data that will be written
 * @param[in]   len     Length of the data that will be written
 * @retval      #RET_INVALID_PARAM if len equal zero or pointer is null
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t CtxMemWrite(const uint8_t *data, uint32_t addr, uint32_t offset, uint32_t len)
{
    QSPI_CommandTypeDef qspi_command;
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((data != NULL) && (len != 0u))
    {
        qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
        qspi_command.Instruction = QSPI_IN_FAST_PROG_CMD;
        qspi_command.AddressMode = QSPI_ADDRESS_1_LINE;
        qspi_command.AddressSize = QSPI_ADDRESS_32_BITS;
        qspi_command.Address = addr + offset;
        qspi_command.DataMode = QSPI_DATA_4_LINES;
        qspi_command.NbData = len;
        qspi_command.DummyCycles = 0;
        qspi_command.DdrMode = QSPI_DDR_MODE_DISABLE;
        qspi_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
        qspi_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

        if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
            return_value = RET_ERROR; // ERROR Happening here : TODO : read the datasheet()
        }

        if (HAL_QSPI_Transmit(&qspi_inst, (uint8_t *)data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
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