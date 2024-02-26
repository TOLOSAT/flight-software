/**
 * @file    generic_hal_ecc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC HAL ECC functions
 * @date    15/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "generic_hal.h"

#if defined(ECC_AVAILABLE)
/***************************** Macros Definitions ****************************/

#define ECC_AXI_SRAM    RAMECC1_Monitor1    /**< ECC Monitor struct for AXI SRAM ECC */
#define ECC_ITCM        RAMECC1_Monitor2    /**< ECC Monitor struct for ITCM ECC */
#define ECC_DTCM0       RAMECC1_Monitor3    /**< ECC Monitor struct for DTCM0 ECC */
#define ECC_DTCM1       RAMECC1_Monitor4    /**< ECC Monitor struct for DTCM1 ECC */
#define ECC_SRAM1_0     RAMECC2_Monitor1    /**< ECC Monitor struct for SRAM1_0 ECC */
#define ECC_SRAM1_1     RAMECC2_Monitor2    /**< ECC Monitor struct for SRAM1_1 ECC */
#define ECC_SRAM2_0     RAMECC2_Monitor3    /**< ECC Monitor struct for SRAM2_0 ECC */
#define ECC_SRAM2_1     RAMECC2_Monitor4    /**< ECC Monitor struct for SRAM2_1 ECC */
#define ECC_SRAM3       RAMECC2_Monitor5    /**< ECC Monitor struct for SRAM3 ECC */
#define ECC_SRAM4       RAMECC3_Monitor1    /**< ECC Monitor struct for SRAM4 ECC */
#define ECC_BACKUP_RAM  RAMECC3_Monitor2    /**< ECC Monitor struct for Backup RAM ECC */

/*************************** Functions Declarations **************************/

extern void ECC_IRQHandler(void);
static void EccErrorHandler(eccInst_t *ecc_inst);
static halStatus_t EccInstanceInitProcedure(eccInst_t *ecc_inst);
static uint32_t GetMemoryOffset(eccInst_t *ecc_inst);

/*************************** Variables Definitions ***************************/

static eccInst_t g_ecc_axi_sram = {0};
static eccInst_t g_ecc_itcm = {0};
static eccInst_t g_ecc_dtcm0 = {0};
static eccInst_t g_ecc_dtcm1 = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn      EccInit(void)
 * @brief   This function init ECC
 * @retval  #GEN_HAL_ERROR if the function has encountered an error
 * @retval  #GEN_HAL_SUCCESSFUL else
 */
halStatus_t EccInit(void)
{
    // Variables Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // First Set Up each ecc instance
    g_ecc_axi_sram.Instance = ECC_AXI_SRAM;
    g_ecc_itcm.Instance = ECC_ITCM;
    g_ecc_dtcm0.Instance = ECC_DTCM0;
    g_ecc_dtcm1.Instance = ECC_DTCM1;

    // Init ECC for SRAM
    return_value = EccInstanceInitProcedure(&g_ecc_axi_sram);
    if (return_value == GEN_HAL_SUCCESSFUL)
    {
        // Init ECC for ITCM
        return_value = EccInstanceInitProcedure(&g_ecc_itcm);
        if (return_value == GEN_HAL_SUCCESSFUL)
        {
            // Init ECC for DTCM0
            return_value = EccInstanceInitProcedure(&g_ecc_dtcm0);
            if (return_value == GEN_HAL_SUCCESSFUL)
            {
                // Init ECC for DTCM1
                return_value = EccInstanceInitProcedure(&g_ecc_dtcm1);
                if (return_value == GEN_HAL_SUCCESSFUL)
                {
                    // Enable interrupts
                    HAL_NVIC_SetPriority(ECC_IRQn, 1, 0);
                    HAL_NVIC_EnableIRQ(ECC_IRQn);
                }
                else
                {
                    return_value = GEN_HAL_ERROR;
                }
            }
            else
            {
                return_value = GEN_HAL_ERROR;
            }
        }
        else
        {
            return_value = GEN_HAL_ERROR;
        }
    }
    else
    {
        return_value = GEN_HAL_ERROR;
    }

    return return_value;
}

/**
 * @fn      EccInstanceInitProcedure(eccInst_t *ecc_inst)
 * @brief   Init ECC Instance
 * @param   ecc_inst ECC instance we want to init
 * @retval  #GEN_HAL_ERROR if an error occured
 * @retval  #GEN_HAL_SUCCESSFUL else
 */
static halStatus_t EccInstanceInitProcedure(eccInst_t *ecc_inst)
{
    // Variables Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;
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
                    return_value = GEN_HAL_ERROR;
                }
            }
            else
            {
                return_value = GEN_HAL_ERROR;
            }
        }
        else
        {
            return_value = GEN_HAL_ERROR;
        }
    }
    else
    {
        return_value = GEN_HAL_ERROR;
    }

    return return_value;
}

/**
 * @brief  Uncorrectable error has been detected
 */
static void EccErrorHandler(eccInst_t *ecc_inst)
{
#if defined(MPU_AVAILABLE)
    // First Disable MPU
    MPU->CTRL = 0x00u;
#endif
    // Correct errors according to the memory type (64 bits, interleaved, 32 bits)
    if ((ecc_inst->Instance == ECC_AXI_SRAM) || (ecc_inst->Instance == ECC_ITCM))
    {
        // 64 bits memories
        uint64_t *addr = (uint64_t *)(GetMemoryOffset(ecc_inst) + HAL_RAMECC_GetFailingAddress(ecc_inst)*8u);
        uint64_t data = ((uint64_t)HAL_RAMECC_GetFailingDataHigh(ecc_inst) << 32u) + (uint64_t)HAL_RAMECC_GetFailingDataLow(ecc_inst);
        *addr = data;
    }
    else if ((ecc_inst->Instance == ECC_DTCM0) || (ecc_inst->Instance == ECC_DTCM1))
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
#if defined(MPU_AVAILABLE)
    // Finally Enable MPU
    MPU->CTRL = 0x05u; // Enable MPU and Background Access for priviledged function (0b101)
#endif
}

/**
 * @brief  This function handles ECC interrupt (when a bitflip is detected)
 */
void ECC_IRQHandler(void)
{
    // Check if SRAM has one bitflip
    if (HAL_RAMECC_IsECCSingleErrorDetected(&g_ecc_axi_sram) == 1u)
    {
        HAL_RAMECC_IRQHandler(&g_ecc_axi_sram);
    }

    // Check if ITCM has one bitflip
    if (HAL_RAMECC_IsECCSingleErrorDetected(&g_ecc_itcm) == 1u)
    {
        HAL_RAMECC_IRQHandler(&g_ecc_itcm);
    }

    // Check if DTCM0 has one bitflip
    if (HAL_RAMECC_IsECCSingleErrorDetected(&g_ecc_dtcm0) == 1u)
    {
        HAL_RAMECC_IRQHandler(&g_ecc_dtcm0);
    }

    // Check if DTCM1 has one bitflip
    if (HAL_RAMECC_IsECCSingleErrorDetected(&g_ecc_dtcm1) == 1u)
    {
        HAL_RAMECC_IRQHandler(&g_ecc_dtcm1);
    }

    // Check if more than one bitflip occured 
    if ((HAL_RAMECC_IsECCDoubleErrorDetected(&g_ecc_axi_sram) == 1u) ||
        (HAL_RAMECC_IsECCDoubleErrorDetected(&g_ecc_itcm) == 1u) ||
        (HAL_RAMECC_IsECCDoubleErrorDetected(&g_ecc_dtcm0) == 1u) ||
        (HAL_RAMECC_IsECCDoubleErrorDetected(&g_ecc_dtcm1) == 1u))
    {
        while(1)
        {
            /* Do Nothing */
        }
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
    if (ecc_inst->Instance == ECC_AXI_SRAM)
    {
        offset_memory = D1_AXISRAM_BASE;
    }
    else if (ecc_inst->Instance == ECC_ITCM)
    {
        offset_memory = D1_ITCMRAM_BASE;
    }
    else if (ecc_inst->Instance == ECC_DTCM0)
    {
        offset_memory = D1_DTCMRAM_BASE;
    }
    else if (ecc_inst->Instance == ECC_DTCM1)
    {
        offset_memory = D1_DTCMRAM_BASE + 4u;
    }
    else if (ecc_inst->Instance == ECC_SRAM1_0)
    {
        offset_memory = D2_AXISRAM_BASE;
    }
    else if (ecc_inst->Instance == ECC_SRAM1_1)
    {
        offset_memory = D2_AXISRAM_BASE + (64u*1024u);
    }
    else if (ecc_inst->Instance == ECC_SRAM2_0)
    {
        offset_memory = D2_AXISRAM_BASE + (128u*1024u);
    }
    else if (ecc_inst->Instance == ECC_SRAM2_1)
    {
        offset_memory = D2_AXISRAM_BASE + (192u*1024u);
    }
    else if (ecc_inst->Instance == ECC_SRAM3)
    {
        offset_memory = D2_AXISRAM_BASE + (256u*1024u);
    }
    else if (ecc_inst->Instance == ECC_SRAM4)
    {
        offset_memory = D3_SRAM_BASE;
    }
    else if (ecc_inst->Instance == ECC_BACKUP_RAM)
    {
        offset_memory = D3_BKPSRAM_BASE;
    }
    else
    {
        offset_memory = 0u;
    }

    return offset_memory;
}

#else
/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      EccInit(void)
 * @brief   This function init ECC
 * @retval  #GEN_HAL_SUCCESSFUL always
 */
halStatus_t EccInit(void)
{
    // Do nothing because ECC is not available
    return GEN_HAL_SUCCESSFUL;
}

#endif