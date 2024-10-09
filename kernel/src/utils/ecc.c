/**
 * @file    ecc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for ECC functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "utils/ecc.h"

#if defined(CONFIG_ECC)
/***************************** Macros Definitions ****************************/

#define RAMECC_MONITOR_AXI_SRAM     RAMECC1_Monitor1    /**< ECC Monitor struct for AXI SRAM ECC */
#define RAMECC_MONITOR_ITCM         RAMECC1_Monitor2    /**< ECC Monitor struct for ITCM ECC */
#define RAMECC_MONITOR_DTCM0        RAMECC1_Monitor3    /**< ECC Monitor struct for DTCM0 ECC */
#define RAMECC_MONITOR_DTCM1        RAMECC1_Monitor4    /**< ECC Monitor struct for DTCM1 ECC */
#define RAMECC_MONITOR_SRAM1_0      RAMECC2_Monitor1    /**< ECC Monitor struct for SRAM1_0 ECC */
#define RAMECC_MONITOR_SRAM1_1      RAMECC2_Monitor2    /**< ECC Monitor struct for SRAM1_1 ECC */
#define RAMECC_MONITOR_SRAM2_0      RAMECC2_Monitor3    /**< ECC Monitor struct for SRAM2_0 ECC */
#define RAMECC_MONITOR_SRAM2_1      RAMECC2_Monitor4    /**< ECC Monitor struct for SRAM2_1 ECC */
#define RAMECC_MONITOR_SRAM3        RAMECC2_Monitor5    /**< ECC Monitor struct for SRAM3 ECC */
#define RAMECC_MONITOR_SRAM4        RAMECC3_Monitor1    /**< ECC Monitor struct for SRAM4 ECC */
#define RAMECC_MONITOR_BACKUP_RAM   RAMECC3_Monitor2    /**< ECC Monitor struct for Backup RAM ECC */

/*************************** Functions Declarations **************************/

extern void ECC_IRQHandler(void);
static void EccErrorHandler(eccInst_t *ecc_inst);
static returnCode_t EccInstanceInitProcedure(eccInst_t *ecc_inst);
static uint32_t GetMemoryOffset(eccInst_t *ecc_inst);

/*************************** Variables Definitions ***************************/

static eccInst_t g_ecc_rams[NB_ECCRAM] = 
{
    {.Instance = RAMECC_MONITOR_AXI_SRAM},
    {.Instance = RAMECC_MONITOR_ITCM},
    {.Instance = RAMECC_MONITOR_DTCM0},
    {.Instance = RAMECC_MONITOR_DTCM1},
    {.Instance = RAMECC_MONITOR_SRAM1_0},
    {.Instance = RAMECC_MONITOR_SRAM1_1},
    {.Instance = RAMECC_MONITOR_SRAM2_1},
    {.Instance = RAMECC_MONITOR_SRAM2_1},
    {.Instance = RAMECC_MONITOR_SRAM3},
    {.Instance = RAMECC_MONITOR_SRAM4},
    {.Instance = RAMECC_MONITOR_BACKUP_RAM},
};

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitEcc(void)
 * @brief   This function init ECC
 * @retval  #RET_ERROR if the function has encountered an error
 * @retval  #RET_SUCCESSFUL else
 */
returnCode_t InitEcc(void)
{
    // Variables Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Init ECC for SRAM
    eccRamId_t ecc_ram_index = 0;
    while ((ecc_ram_index < NB_ECCRAM) && (return_value == RET_SUCCESSFUL))
    {
        return_value = EccInstanceInitProcedure(&g_ecc_rams[ecc_ram_index]);
        ecc_ram_index++;
    }

    // If all init went right, enable interrupts
    if (return_value == RET_SUCCESSFUL)
    {
        HAL_NVIC_SetPriority(ECC_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(ECC_IRQn);
    }

    return return_value;
}

/**
 * @fn      EccInstanceInitProcedure(eccInst_t *ecc_inst)
 * @brief   Init ECC Instance
 * @param   ecc_inst ECC instance we want to init
 * @retval  #RET_ERROR if an error occured
 * @retval  #RET_SUCCESSFUL else
 */
static returnCode_t EccInstanceInitProcedure(eccInst_t *ecc_inst)
{
    // Variables Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_hal = HAL_OK;

    // Function core
    test_hal = HAL_RAMECC_Init(ecc_inst);
    if (test_hal == HAL_OK)
    {
        test_hal = HAL_RAMECC_RegisterCallback(ecc_inst, EccErrorHandler);
        if (test_hal == HAL_OK)
        {
            test_hal = HAL_RAMECC_EnableNotification(ecc_inst, RAMECC_IT_MONITOR_ALL);
            if (test_hal == HAL_OK)
            {
                test_hal = HAL_RAMECC_StartMonitor(ecc_inst);
                if (test_hal != HAL_OK)
                {
                    return_value = RET_ERROR;
                }
            }
            else
            {
                return_value = RET_ERROR;
            }
        }
        else
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
 * @brief  Uncorrectable error has been detected
 */
static void EccErrorHandler(eccInst_t *ecc_inst)
{
    // Correct errors according to the memory type (64 bits, interleaved, 32 bits)
    if ((ecc_inst->Instance == RAMECC_MONITOR_AXI_SRAM) || (ecc_inst->Instance == RAMECC_MONITOR_ITCM))
    {
        // 64 bits memories
        uint64_t *addr = (uint64_t *)(GetMemoryOffset(ecc_inst) + HAL_RAMECC_GetFailingAddress(ecc_inst)*8u);
        uint64_t data = ((uint64_t)HAL_RAMECC_GetFailingDataHigh(ecc_inst) << 32u) + (uint64_t)HAL_RAMECC_GetFailingDataLow(ecc_inst);
        *addr = data;
    }
    else if ((ecc_inst->Instance == RAMECC_MONITOR_DTCM0) || (ecc_inst->Instance == RAMECC_MONITOR_DTCM1))
    {
        // DTCM memories (which are interlevead so the procedure is not the same)
        uint64_t *addr = (uint64_t *)(GetMemoryOffset(ecc_inst) + HAL_RAMECC_GetFailingAddress(ecc_inst)*8u);
        uint64_t data = HAL_RAMECC_GetFailingDataLow(ecc_inst);
        *addr = data;
    }
    else
    {
        // 32 bits memories
        uint32_t *addr = (uint32_t *)(GetMemoryOffset(ecc_inst) + HAL_RAMECC_GetFailingAddress(ecc_inst)*4u);
        uint32_t data = HAL_RAMECC_GetFailingDataLow(ecc_inst);
        *addr = data;
    }
}

/**
 * @fn      GetMemoryOffset(eccInst_t *ecc_inst)
 * @brief   Get memory start address that the ECC instance is looking for
 * @param   ecc_inst 
 * @return  Memory Offset
 */
static uint32_t GetMemoryOffset(eccInst_t *ecc_inst)
{
    uint32_t offset_memory = 0u;
    if (ecc_inst->Instance == RAMECC_MONITOR_AXI_SRAM)
    {
        offset_memory = D1_AXISRAM_BASE;
    }
    else if (ecc_inst->Instance == RAMECC_MONITOR_ITCM)
    {
        offset_memory = D1_ITCMRAM_BASE;
    }
    else if (ecc_inst->Instance == RAMECC_MONITOR_DTCM0)
    {
        offset_memory = D1_DTCMRAM_BASE;
    }
    else if (ecc_inst->Instance == RAMECC_MONITOR_DTCM1)
    {
        offset_memory = D1_DTCMRAM_BASE + 4u;
    }
    else if (ecc_inst->Instance == RAMECC_MONITOR_SRAM1_0)
    {
        offset_memory = D2_AHBSRAM_BASE;
    }
    else if (ecc_inst->Instance == RAMECC_MONITOR_SRAM1_1)
    {
        offset_memory = D2_AHBSRAM_BASE + (64u*1024u);
    }
    else if (ecc_inst->Instance == RAMECC_MONITOR_SRAM2_0)
    {
        offset_memory = D2_AHBSRAM_BASE + (128u*1024u);
    }
    else if (ecc_inst->Instance == RAMECC_MONITOR_SRAM2_1)
    {
        offset_memory = D2_AHBSRAM_BASE + (192u*1024u);
    }
    else if (ecc_inst->Instance == RAMECC_MONITOR_SRAM3)
    {
        offset_memory = D2_AHBSRAM_BASE + (256u*1024u);
    }
    else if (ecc_inst->Instance == RAMECC_MONITOR_SRAM4)
    {
        offset_memory = D3_SRAM_BASE;
    }
    else if (ecc_inst->Instance == RAMECC_MONITOR_BACKUP_RAM)
    {
        offset_memory = D3_BKPSRAM_BASE;
    }
    else
    {
        offset_memory = 0u;
    }

    return offset_memory;
}

/**
 * @brief  This function handles ECC interrupt (when a bitflip is detected)
 */
void ECC_IRQHandler(void)
{
    // Check which RAM has triggered ECCRAM IRQ
    for(eccRamId_t ecc_ram_index = 0; ecc_ram_index < NB_ECCRAM; ecc_ram_index++)
    {
        // Check if there is one bitflip
        if (HAL_RAMECC_IsECCSingleErrorDetected(&g_ecc_rams[ecc_ram_index]) == 1u)
        {
            HAL_RAMECC_IRQHandler(&g_ecc_rams[ecc_ram_index]);
        }

        // Check if more than one bitflip occured 
        if (HAL_RAMECC_IsECCDoubleErrorDetected(&g_ecc_rams[ecc_ram_index]) == 1u)
        {
            while(1)
            {
                /* Do Nothing */
            }
        }
    }
}

#else
/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitEcc(void)
 * @brief   This function init ECC
 * @retval  #RET_SUCCESSFUL always
 */
returnCode_t InitEcc(void)
{
    // Do nothing because ECC is not available
    return RET_SUCCESSFUL;
}

#endif