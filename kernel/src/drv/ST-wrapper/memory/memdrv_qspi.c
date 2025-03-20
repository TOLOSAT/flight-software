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
#define QSPI_WRITE_ENABLE_CMD        0x06u /**< Write Enable command */
#define QSPI_READ_CMD                0x03u /**< Quad Output Fast Read command */
#define QSPI_DUMMY_CLOCK_CYCLES_READ 0     /**< Number of dummy cycles for Quad Read */
#define QSPI_WRITE_CMD               0x32u /**< Quad Input Fast Program command */

/*************************** Functions Declarations **************************/

typedef enum {
    SR1 = 0x05,
    SR2 = 0x35,
    SR3 = 0x15
} registerSelect_t;


static void CtxMemReadStatus(uint8_t* status, registerSelect_t reg);
static void CtxMemeErase();
// static void CtxMemWriteStatus(uint8_t* status, registerSelect_t reg);

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
    uint8_t status1;
    uint8_t status2;
    uint8_t status3;

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

    HAL_Delay(10);

    CtxMemReadStatus(&status1, SR1);
    CtxMemReadStatus(&status2, SR2);
    CtxMemReadStatus(&status3, SR3);

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

    HAL_Delay(10);

    CtxMemReadStatus(&status1, SR1);
    CtxMemReadStatus(&status2, SR2);
    CtxMemReadStatus(&status3, SR3);

    uint8_t test_data[32];
    for(int i = 0; i < 32; ++i)
    {
        test_data[i] = (uint8_t) (i + 10);
    }

    uint8_t test_buffer[32] = { 0 };

    CtxMemRead((uint8_t *) &test_buffer, 0, 0, 32);

    CtxMemeErase();

    /* Wait */
    CtxMemReadStatus(&status1, SR1);

    while ((status1 & 0x1) == 1)
    {
        CtxMemReadStatus(&status1, SR1);
    }
    /* End wait */


    CtxMemRead((uint8_t *) &test_buffer, 0, 0, 32);

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

    HAL_Delay(10);

    CtxMemWrite((uint8_t *) &test_data, 0, 0, 32);

    /* Wait */
    CtxMemReadStatus(&status1, SR1);

    while ((status1 & 0x1) == 1)
    {
        CtxMemReadStatus(&status1, SR1);
    }
    /* End wait */

    // CtxMemReadStatus(&status);

    CtxMemRead((uint8_t *) &test_buffer, 0, 0, 32);

    // CtxMemReadStatus(&status);
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

        qspi_command.Instruction = 0xebu;
        qspi_command.AddressSize = QSPI_ADDRESS_24_BITS;

        qspi_command.AddressMode = QSPI_ADDRESS_4_LINES;

        qspi_command.Address = addr + offset;

        qspi_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	    qspi_command.AlternateBytes = QSPI_ALTERNATE_BYTES_NONE;
	    qspi_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_NONE;

        qspi_command.DummyCycles = 6;
        qspi_command.DataMode = QSPI_DATA_4_LINES;
        qspi_command.NbData = len;

        qspi_command.DdrMode = QSPI_DDR_MODE_DISABLE;
        qspi_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
        qspi_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

        if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
            return_value = RET_ERROR;
            ErrorHandler();
        }

        if (HAL_QSPI_Receive(&qspi_inst, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        {
            return_value = RET_ERROR;
            ErrorHandler();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
        ErrorHandler();
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
        qspi_command.Instruction = QSPI_WRITE_CMD;
        qspi_command.AddressMode = QSPI_ADDRESS_1_LINE;
        qspi_command.AddressSize = QSPI_ADDRESS_24_BITS;
        qspi_command.Address = addr + offset;
        qspi_command.DataMode = QSPI_DATA_4_LINES;
        qspi_command.NbData = len;
        qspi_command.DummyCycles = 0;

        qspi_command.DdrMode = QSPI_DDR_MODE_DISABLE;
        qspi_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
        qspi_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

        qspi_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
        qspi_command.AlternateBytes = QSPI_ALTERNATE_BYTES_NONE;
        qspi_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_NONE;


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

static void CtxMemReadStatus(uint8_t* status, registerSelect_t reg)
{
    QSPI_CommandTypeDef qspi_command;

    qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    qspi_command.Instruction = reg;

    qspi_command.AddressMode = QSPI_ADDRESS_NONE;
    qspi_command.AddressSize = QSPI_ADDRESS_NONE;
    qspi_command.Address = 0x0u;

    qspi_command.DummyCycles = 0;
    qspi_command.DataMode = QSPI_DATA_1_LINE;
    qspi_command.NbData = 1;

    qspi_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qspi_command.AlternateBytes = QSPI_ALTERNATE_BYTES_NONE;
	qspi_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_NONE;

    qspi_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	qspi_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	qspi_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        ErrorHandler();
    }

    if (HAL_QSPI_Receive(&qspi_inst, status, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        ErrorHandler();
    }
}

static void CtxMemeErase()
{
    QSPI_CommandTypeDef qspi_command;

    qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    qspi_command.Instruction = 0x20u;

    qspi_command.AddressMode = QSPI_ADDRESS_1_LINE;
    qspi_command.AddressSize = QSPI_ADDRESS_24_BITS;
    qspi_command.Address = 1024u;

    qspi_command.DummyCycles = 0;
    qspi_command.DataMode = QSPI_DATA_NONE;
    qspi_command.NbData = 0;

    qspi_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qspi_command.AlternateBytes = QSPI_ALTERNATE_BYTES_NONE;
	qspi_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_NONE;

    qspi_command.DdrMode = QSPI_DDR_MODE_DISABLE;
	qspi_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	qspi_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        ErrorHandler();
    }
}

// static void CtxMemWriteStatus(uint8_t* status, registerSelect_t reg)
// {
//     QSPI_CommandTypeDef qspi_command;

//     qspi_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
//     qspi_command.Instruction = reg - 0x4u;

//     qspi_command.AddressMode = QSPI_ADDRESS_NONE;
//     qspi_command.AddressSize = QSPI_ADDRESS_NONE;
//     qspi_command.Address = 0x0u;

//     qspi_command.DummyCycles = 0;
//     qspi_command.DataMode = QSPI_DATA_1_LINE;
//     qspi_command.NbData = 1;

//     qspi_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
// 	qspi_command.AlternateBytes = QSPI_ALTERNATE_BYTES_NONE;
// 	qspi_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_NONE;

//     qspi_command.DdrMode = QSPI_DDR_MODE_DISABLE;
// 	qspi_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
// 	qspi_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

//     if (HAL_QSPI_Command(&qspi_inst, &qspi_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//     {
//         ErrorHandler();
//     }

//     if (HAL_QSPI_Transmit(&qspi_inst, status, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//     {
//         ErrorHandler();
//     }
// }