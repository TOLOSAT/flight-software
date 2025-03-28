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

/*************************** Functions Declarations **************************/

#if defined(CONFIG_MEMORY_QSPI_NOR)
static returnCode_t QSPI_MemoryReadStatus(uint8_t *status, statusRegisterSelect_t reg);
static returnCode_t QSPI_MemoryWriteStatus(uint8_t *status, statusRegisterSelect_t reg);
static returnCode_t QSPI_MemoryWriteEnable(void);
static bool QSPI_MemoryIsBusy(void);
#endif /* CONFIG_MEMORY_QSPI_NOR */

/*************************** Variables Definitions ***************************/

#if defined(CONFIG_MEMORY_QSPI_NOR)
static QSPI_HandleTypeDef qspi_inst;
#endif /* CONFIG_MEMORY_QSPI_NOR */

/*************************** Functions Definitions ***************************/

/**
 * @fn          QSPI_MemoryInit(void)
 * @brief       Function that initializes the memory
 * @retval      #RET_SUCCESSFUL if the memory has been initialized
 * @retval      #RET_ERROR if the memory has not been initialized
 */
returnCode_t QSPI_MemoryInit(void)
{
    returnCode_t return_value = RET_SUCCESSFUL;

#if defined(CONFIG_MEMORY_QSPI_NOR)
    uint8_t status;

    /* Initialize the QSPI memory bus */
    qspi_inst.Instance                = QUADSPI;
    qspi_inst.Init.ClockPrescaler     = QSPI_CLOCK_PRESCALER;
    qspi_inst.Init.FifoThreshold      = QSPI_FIFO_THRESHOLD;
    qspi_inst.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_NONE;
    qspi_inst.Init.FlashSize          = QSPI_FLASH_SIZE;
    qspi_inst.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
    qspi_inst.Init.ClockMode          = QSPI_CLOCK_MODE_0;
    qspi_inst.Init.FlashID            = QSPI_FLASH_ID_1;
    qspi_inst.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;

    if (HAL_QSPI_Init(&qspi_inst) != HAL_OK)
    {
        return_value = RET_ERROR;
    }

    /* Enable the Quad Mode */
    if (return_value == RET_SUCCESSFUL)
    {
        return_value = QSPI_MemoryReadStatus(&status, STATUS_REGISTER_2);

        if (return_value == RET_SUCCESSFUL)
        {
            status       |= QSPI_QUAD_ENABLE_BIT;
            return_value  = QSPI_MemoryWriteStatus(&status, STATUS_REGISTER_2);
        }
    }

    // Some tests

    // const uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
    // uint8_t read_data[256] = {0};
    // return_value = QSPI_MemoryErase(0x1024u, sizeof(read_data));
    // QSPI_MemoryReadStatus(&status, STATUS_REGISTER_2);
    // return_value = QSPI_MemoryRead((uint8_t*) &read_data, 0x1024u, sizeof(read_data));
    // return_value = QSPI_MemoryWrite((uint8_t*) &data, 0x1024u, sizeof(data));
    // return_value = QSPI_MemoryRead((uint8_t*) &read_data, 0x1024u, sizeof(read_data));
    // __NOP();
    // return_value = QSPI_MemoryWrite((uint8_t*) &data2, 0x0, 4);
    // return_value = QSPI_MemoryRead((uint8_t*) &read_data, 0x0, 4);
#endif /* CONFIG_MEMORY_QSPI_NOR */

    return return_value;
}

/**
 * @brief       Function that reads from the memory
 * @param[out]  data    Pointer to the data that will be read
 * @param[in]   addr    Address of the data that will be read
 * @param[in]   len     Length of the data that will be read
 * @retval      #RET_INVALID_PARAM if len equal zero or pointer is null
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t QSPI_MemoryRead(uint8_t *data, uint32_t addr, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

#if defined(CONFIG_MEMORY_QSPI_NOR)
    QSPI_CommandTypeDef qspi_command = { 0 };

    // Check parameter(s)
    if ((data != NULL) && (len != 0u))
    {
        qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
        qspi_command.Instruction     = 0x03u;
        qspi_command.AddressSize     = QSPI_ADDRESS_24_BITS;
        qspi_command.AddressMode     = QSPI_ADDRESS_1_LINE;
        qspi_command.Address         = addr;
        qspi_command.DataMode        = QSPI_DATA_1_LINE;
        qspi_command.NbData          = len;
        qspi_command.DummyCycles     = 0;

        if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
            return_value = RET_ERROR;
        }

        if (HAL_QSPI_Receive(&qspi_inst, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
            return_value = RET_ERROR;
        }

        while (QSPI_MemoryIsBusy())
        {
            __NOP();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }
#endif /* CONFIG_MEMORY_QSPI_NOR */

    return return_value;
}

/**
 * @brief       Function that writes into the memory
 * @param[in]   data    Pointer to the data that will be written
 * @param[in]   addr    Address of the data that will be written
 * @param[in]   len     Length of the data that will be written
 * @retval      #RET_INVALID_PARAM if len equal zero
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t QSPI_MemoryWrite(const uint8_t *data, uint32_t addr, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

#if defined(CONFIG_MEMORY_QSPI_NOR)
    QSPI_CommandTypeDef qspi_command = { 0 };

    // Check parameter(s)
    if ((data != NULL) && (len != 0u))
    {
        qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
        qspi_command.Instruction     = QSPI_WRITE_CMD;
        qspi_command.AddressMode     = QSPI_ADDRESS_1_LINE;
        qspi_command.AddressSize     = QSPI_ADDRESS_24_BITS;
        qspi_command.Address         = addr;
        qspi_command.DataMode        = QSPI_DATA_4_LINES;
        qspi_command.NbData          = len;

        return_value = QSPI_MemoryErase(addr, len);

        if (return_value == RET_SUCCESSFUL)
        {
            return_value = QSPI_MemoryWriteEnable();

            if (return_value == RET_SUCCESSFUL)
            {
                if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
                {
                    return_value = RET_ERROR;
                }

                if (HAL_QSPI_Transmit(&qspi_inst, (uint8_t *)data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
                {
                    return_value = RET_ERROR;
                }

                while (QSPI_MemoryIsBusy())
                {
                    __NOP();
                }
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }
#endif /* CONFIG_MEMORY_QSPI_NOR */

    return return_value;
}

/**
 * @brief       Function that erases the memory
 * @param[in]   addr    Address of the data that will be erased
 * @param[in]   len     Length of the data that will be erased
 * @retval      #RET_INVALID_PARAM if len equal zero
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t QSPI_MemoryErase(uint32_t addr, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)addr;
    (void)len;

#if defined(CONFIG_MEMORY_QSPI_NOR)
    QSPI_CommandTypeDef qspi_command = { 0 };

    qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    qspi_command.Instruction     = QSPI_ERASE_CMD;
    qspi_command.AddressMode     = QSPI_ADDRESS_1_LINE;
    qspi_command.AddressSize     = QSPI_ADDRESS_24_BITS;
    qspi_command.Address         = addr;

    return_value = QSPI_MemoryWriteEnable();

    if (return_value == RET_SUCCESSFUL)
    {
        if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
            return_value = RET_ERROR;
        }

        while (QSPI_MemoryIsBusy())
        {
            __NOP();
        }
    }
#endif /* CONFIG_MEMORY_QSPI_NOR */

    return return_value;
}

#if defined(CONFIG_MEMORY_QSPI_NOR)
static returnCode_t QSPI_MemoryReadStatus(uint8_t *status, statusRegisterSelect_t reg)
{
    QSPI_CommandTypeDef qspi_command = { 0 };
    returnCode_t return_value        = RET_SUCCESSFUL;

    qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    qspi_command.Instruction     = reg + QSPI_REGISTER_READ_OFFSET;
    qspi_command.DataMode        = QSPI_DATA_1_LINE;
    qspi_command.NbData          = 0x1u;

    if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return_value = RET_ERROR;
    }

    if (HAL_QSPI_Receive(&qspi_inst, status, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return_value = RET_ERROR;
    }

    return return_value;
}

static __unused returnCode_t QSPI_MemoryWriteStatus(uint8_t *status, statusRegisterSelect_t reg)
{
    QSPI_CommandTypeDef qspi_command = { 0 };
    returnCode_t return_value        = RET_SUCCESSFUL;

    qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    qspi_command.Instruction     = reg + QSPI_REGISTER_WRITE_OFFSET;
    qspi_command.DataMode        = QSPI_DATA_1_LINE;
    qspi_command.NbData          = 0x1u;

    if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return_value = RET_ERROR;
    }

    if (HAL_QSPI_Transmit(&qspi_inst, status, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return_value = RET_ERROR;
    }

    return return_value;
}

static returnCode_t QSPI_MemoryWriteEnable(void)
{
    QSPI_CommandTypeDef qspi_command = { 0 };
    returnCode_t return_value        = RET_SUCCESSFUL;

    qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    qspi_command.Instruction     = QSPI_WRITE_ENABLE_CMD;

    if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return_value = RET_ERROR;
    }

    while (QSPI_MemoryIsBusy())
    {
        __NOP();
    }

    return return_value;
}

static bool QSPI_MemoryIsBusy(void)
{
    uint8_t status;
    bool return_value = false;

    if (QSPI_MemoryReadStatus(&status, STATUS_REGISTER_1) == RET_SUCCESSFUL)
    {
        return_value = (status & 0x1u) == 0x1u;
    }

    return return_value;
}
#endif /* CONFIG_MEMORY_QSPI_NOR */
