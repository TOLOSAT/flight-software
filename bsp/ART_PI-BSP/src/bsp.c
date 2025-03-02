/**
 * @file    bsp.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for bsp init
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "bsp.h"

/***************************** Macros Definitions ****************************/

// QUADSPI defines
#define QSPI_WRITE_ENABLE_CMD                 0x06 /**< Write Enable command */
#define QSPI_OUT_FAST_READ_CMD                0x6B /**< Quad Output Fast Read command */
#define QSPI_DUMMY_CLOCK_CYCLES_READ          10   /**< Number of dummy cycles for Quad Read */

// SDRAM defines
#define SDRAM_REFRESH_COUNT                   ((uint32_t)0x02A5) // TO DO : re-evaluate
#define SDRAM_LOAD_TO_ACTIVE_DELAY            2                  // TO DO : re-evaluate
#define SDRAM_EXIT_SELF_REFRESH_DELAY         8                  // TO DO : re-evaluate
#define SDRAM_SELF_REFRESH_TIME               6                  // TO DO : re-evaluate
#define SDRAM_ROW_CYCLE_DELAY                 6                  // TO DO : re-evaluate
#define SDRAM_WRITE_RECOVERY_TIME             2                  // TO DO : re-evaluate
#define SDRAM_RP_DELAY                        2                  // TO DO : re-evaluate
#define SDRAM_RCD_DELAY                       2                  // TO DO : re-evaluate
#define SDRAM_MODEREG_BURST_LENGTH_2          ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   ((uint16_t)0x0000)
#define SDRAM_MODEREG_CAS_LATENCY_3           ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE  ((uint16_t)0x0200)

/*************************** Functions Declarations **************************/

extern void ErrorHandler(void);

static void QspiNandInit(void);
static void SdramInit(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      SystemClock_Config
 * @brief   System Clock Configuration
 */
returnCode_t SystemClock_Config(void)
{
    returnCode_t return_value            = RET_SUCCESSFUL;
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    /** Initializes the RCC Oscillator Powers */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
    {
        // Wait until the oscillator power is on
    }

    /** Initializes the RCC Oscillators */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.LSEState       = RCC_LSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 2;
    RCC_OscInitStruct.PLL.PLLN       = 64;
    RCC_OscInitStruct.PLL.PLLP       = 2;
    RCC_OscInitStruct.PLL.PLLQ       = 8;
    RCC_OscInitStruct.PLL.PLLR       = 2;
    RCC_OscInitStruct.PLL.PLLRGE     = RCC_PLL1VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL  = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN   = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK)
    {
        /** Initializes the CPU, AHB and APB buses clocks */
        RCC_ClkInitStruct.ClockType =
            RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
        RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
        RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
        RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
        if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_ERROR;
    }

    return return_value;
}

/**
 * @fn      BSPLateInit(void)
 * @brief   This function will initialise some BSP specifique peripherals
 * @return  Nothing
 */
void BSPLateInit(void)
{
    QspiNandInit();
    SdramInit();
}

/**
 * @fn      QspiNandInit(void)
 * @brief   This function will initialise the QSPI peripheral for NAND flash
 * @return  Nothing
 */
static void QspiNandInit(void)
{
    static QSPI_HandleTypeDef qspi_inst;
    QSPI_CommandTypeDef qspi_command;         /**< QSPI command */
    QSPI_MemoryMappedTypeDef qspi_mem_mapped; /**< QSPI memory map operation */

    /* Initialize the QSPI memory bus */
    qspi_inst.Instance                = QUADSPI;
    qspi_inst.Init.ClockPrescaler     = 2;
    qspi_inst.Init.FifoThreshold      = 1;
    qspi_inst.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_NONE;
    qspi_inst.Init.FlashSize          = 1;
    qspi_inst.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
    qspi_inst.Init.ClockMode          = QSPI_CLOCK_MODE_0; /**< Clock mode 0 = low */
    qspi_inst.Init.FlashID            = QSPI_FLASH_ID_1;
    qspi_inst.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;

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
 * @brief Init SDRAM
 */
static void SdramInit(void)
{
    SDRAM_HandleTypeDef sdram_inst       = { 0 };
    FMC_SDRAM_TimingTypeDef sdram_timing = { 0 };
    FMC_SDRAM_CommandTypeDef command     = { 0 };

    // Initialise SDRAM Instance
    sdram_inst.Instance                = FMC_SDRAM_DEVICE;
    sdram_inst.Init.SDBank             = FMC_SDRAM_BANK1;
    sdram_inst.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9;
    sdram_inst.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_13;
    sdram_inst.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;
    sdram_inst.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
    sdram_inst.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;
    sdram_inst.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    sdram_inst.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;
    sdram_inst.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
    sdram_inst.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;

    // Initialise SDRA timing Instance
    sdram_timing.LoadToActiveDelay    = SDRAM_LOAD_TO_ACTIVE_DELAY;
    sdram_timing.ExitSelfRefreshDelay = SDRAM_EXIT_SELF_REFRESH_DELAY;
    sdram_timing.SelfRefreshTime      = SDRAM_SELF_REFRESH_TIME;
    sdram_timing.RowCycleDelay        = SDRAM_ROW_CYCLE_DELAY;
    sdram_timing.WriteRecoveryTime    = SDRAM_WRITE_RECOVERY_TIME;
    sdram_timing.RPDelay              = SDRAM_RP_DELAY;
    sdram_timing.RCDDelay             = SDRAM_RCD_DELAY;

    // Initialise SDRAM
    if (HAL_SDRAM_Init(&sdram_inst, &sdram_timing) != HAL_OK)
    {
        ErrorHandler();
    }

    /* Configure a clock configuration enable command */
    command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
    command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    command.AutoRefreshNumber      = 1;
    command.ModeRegisterDefinition = 0;
    if (HAL_SDRAM_SendCommand(&sdram_inst, &command, 0x1000) != HAL_OK)
    {
        ErrorHandler();
    }

    /* Insert 100 ms delay */
    /* interrupt is not enable, just to delay some time. */
    HAL_Delay(100);

    /* Configure a PALL (precharge all) command */
    command.CommandMode            = FMC_SDRAM_CMD_PALL;
    command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    command.AutoRefreshNumber      = 1;
    command.ModeRegisterDefinition = 0;
    if (HAL_SDRAM_SendCommand(&sdram_inst, &command, 0x1000) != HAL_OK)
    {
        ErrorHandler();
    }

    /* Configure a Auto-Refresh command */
    command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    command.AutoRefreshNumber      = 8;
    command.ModeRegisterDefinition = 0;
    if (HAL_SDRAM_SendCommand(&sdram_inst, &command, 0x1000) != HAL_OK)
    {
        ErrorHandler();
    }

    /* Program the external memory mode register */
    command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
    command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    command.AutoRefreshNumber      = 1;
    command.ModeRegisterDefinition = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2 | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL | SDRAM_MODEREG_CAS_LATENCY_3
                                     | SDRAM_MODEREG_OPERATING_MODE_STANDARD | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
    if (HAL_SDRAM_SendCommand(&sdram_inst, &command, 0x1000) != HAL_OK)
    {
        ErrorHandler();
    }

    /* Set the device refresh counter */
    if (HAL_SDRAM_ProgramRefreshRate(&sdram_inst, SDRAM_REFRESH_COUNT) != HAL_OK)
    {
        ErrorHandler();
    }
}