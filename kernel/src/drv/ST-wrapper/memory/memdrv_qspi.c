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

/***************************** Macros Definitions ****************************/

#define QSPI_READ_CMD                0xebu /**< Quad Output Fast Read command */
#define QSPI_WRITE_CMD               0x32u /**< Quad Input Fast Program command */
#define QSPI_SECTOR_ERASE_CMD        0x20u /**< Sector Erase command */

#define QSPI_DUMMY_CLOCK_CYCLES_READ 6u   /**< Number of dummy cycles for Quad Read */
#define QSPI_CLOCK_PRESCALER         255u /**< Clock prescaler */
#define QSPI_FIFO_THRESHOLD          1u   /**< FIFO threshold */
#define QSPI_FLASH_SIZE              22u  /**< Flash size */

#define QSPI_REGISTER_READ_OFFSET    5u /**< Register select read offset */
#define QSPI_REGISTER_WRITE_OFFSET   1u /**< Register select write offset */

#define QSPI_WRITE_ENABLE_CMD        0x06u /**< Write Enable command */
#define QSPI_QUAD_ENABLE_BIT         0x2u  /**< Quad enable bit */

/***************************** Types Definitions *****************************/

/**
 * @enum     statusRegisterSelect_t
 * @brief    Status register select enumeration
 */
typedef enum
{
    STATUS_REGISTER_1 = 0x00u,
    STATUS_REGISTER_2 = 0x30u,
    STATUS_REGISTER_3 = 0x10u
} statusRegisterSelect_t;

/*************************** Functions Declarations **************************/

static returnCode_t QSPI_MemoryReadStatus(uint8_t *status, statusRegisterSelect_t reg);
static returnCode_t QSPI_MemoryWriteStatus(uint8_t *status, statusRegisterSelect_t reg);
static returnCode_t QSPI_MemoryWriteEnable(void);
static bool QSPI_MemoryIsBusy(void);

/*************************** Variables Definitions ***************************/

static QSPI_HandleTypeDef qspi_inst;

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

    if (HAL_QSPI_DeInit(&qspi_inst) != HAL_OK)
    {
        return_value = RET_ERROR;
    }

    if (return_value == RET_SUCCESSFUL)
    {
        if (HAL_QSPI_Init(&qspi_inst) != HAL_OK)
        {
            return_value = RET_ERROR;
        }
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

    while (QSPI_MemoryIsBusy())
    {
        __NOP();
    }

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
    returnCode_t return_value        = RET_SUCCESSFUL;
    QSPI_CommandTypeDef qspi_command = { 0 };

    while (QSPI_MemoryIsBusy())
    {
        __NOP();
    }

    // Check parameter(s)
    if ((data != NULL) && (len != 0u) && (len <= 256u)) // TODO : Implement the case weather we wand to read/write more than 256B
    {
        qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
        qspi_command.Instruction     = QSPI_READ_CMD;
        qspi_command.AddressSize     = QSPI_ADDRESS_24_BITS;
        qspi_command.AddressMode     = QSPI_ADDRESS_4_LINES;
        qspi_command.Address         = addr;
        qspi_command.DataMode        = QSPI_DATA_4_LINES;
        qspi_command.NbData          = len;
        qspi_command.DummyCycles     = QSPI_DUMMY_CLOCK_CYCLES_READ;

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
returnCode_t QSPI_MemoryWrite(uint8_t *data, uint32_t addr, uint32_t len)
{
    returnCode_t return_value        = RET_SUCCESSFUL;
    QSPI_CommandTypeDef qspi_command = { 0 };

    while (QSPI_MemoryIsBusy())
    {
        __NOP();
    }

    // Check parameter(s)
    if ((data != NULL) && (len != 0u) && (len <= 256u)) // TODO : Implement the case weather we wand to read/write more than 256B
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
    returnCode_t return_value        = RET_SUCCESSFUL;
    QSPI_CommandTypeDef qspi_command = { 0 };

    while (QSPI_MemoryIsBusy())
    {
        __NOP();
    }

    // Unused
    (void)(len);

    qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    qspi_command.Instruction     = QSPI_SECTOR_ERASE_CMD;
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

    return return_value;
}

static returnCode_t QSPI_MemoryReadStatus(uint8_t *status, statusRegisterSelect_t reg)
{
    QSPI_CommandTypeDef qspi_command = { 0 };
    returnCode_t return_value        = RET_SUCCESSFUL;

    qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    qspi_command.Instruction     = reg + (statusRegisterSelect_t)QSPI_REGISTER_READ_OFFSET;
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
    qspi_command.Instruction     = reg + (statusRegisterSelect_t)QSPI_REGISTER_WRITE_OFFSET;
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
