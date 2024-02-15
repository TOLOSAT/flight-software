/**
 * @file    generic_hal_ecc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC HAL ECC functions
 * @date    15/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#if defined(ECC_AVAILABLE)
/******************************* Include Files *******************************/

#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

#define ECC_SRAM    RAMECC1_Monitor1   /**< ECC Monitor struct for SRAM ECC */
#define ECC_ITCM    RAMECC1_Monitor2   /**< ECC Monitor struct for ITCM ECC */
#define ECC_DTCM0   RAMECC1_Monitor3   /**< ECC Monitor struct for DTCM0 ECC */
#define ECC_DTCM1   RAMECC1_Monitor4   /**< ECC Monitor struct for DTCM1 ECC */

/*************************** Functions Declarations **************************/

extern void ECC_IRQHandler(void);
static void EccErrorHandler(eccInst_t *ecc_inst);
static halStatus_t EccInstanceInitProcedure(eccInst_t *ecc_inst);

/*************************** Variables Definitions ***************************/

static eccInst_t g_ecc_sram = {0};
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
    g_ecc_sram.Instance = ECC_SRAM;
    g_ecc_itcm.Instance = ECC_ITCM;
    g_ecc_dtcm0.Instance = ECC_DTCM0;
    g_ecc_dtcm1.Instance = ECC_DTCM1;

    // Init ECC for SRAM
    return_value = EccInstanceInitProcedure(&g_ecc_sram);
    if (return_value == GEN_HAL_SUCCESSFUL)
    {
        // Init ECC for ITCM
        return_value = EccInstanceInitProcedure(&g_ecc_sram);
        if (return_value == GEN_HAL_SUCCESSFUL)
        {
            // Init ECC for DTCM0
            return_value = EccInstanceInitProcedure(&g_ecc_sram);
            if (return_value == GEN_HAL_SUCCESSFUL)
            {
                // Init ECC for DTCM1
                return_value = EccInstanceInitProcedure(&g_ecc_sram);
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
    (void)(ecc_inst);
    while (1)
    {
        /* Do Nothing */
    }
}

/**
 * @brief  This function handles ECC interrupt (when a bitflip is detected)
 */
void ECC_IRQHandler(void)
{
    // Check if SRAM has a bitflip
    if ((HAL_RAMECC_IsECCSingleErrorDetected(&g_ecc_sram) == 1u) || (HAL_RAMECC_IsECCDoubleErrorDetected(&g_ecc_sram) == 1u))
    {
        HAL_RAMECC_IRQHandler(&g_ecc_sram);
    }

    // Check if ITCM has a bitflip
    if ((HAL_RAMECC_IsECCSingleErrorDetected(&g_ecc_itcm) == 1u) || (HAL_RAMECC_IsECCDoubleErrorDetected(&g_ecc_itcm) == 1u))
    {
        HAL_RAMECC_IRQHandler(&g_ecc_itcm);
    }

    // Check if DTCM0 has a bitflip
    if ((HAL_RAMECC_IsECCSingleErrorDetected(&g_ecc_dtcm0) == 1u) || (HAL_RAMECC_IsECCDoubleErrorDetected(&g_ecc_dtcm0) == 1u))
    {
        HAL_RAMECC_IRQHandler(&g_ecc_dtcm0);
    }

    // Check if DTCM1 has a bitflip
    if ((HAL_RAMECC_IsECCSingleErrorDetected(&g_ecc_dtcm1) == 1u) || (HAL_RAMECC_IsECCDoubleErrorDetected(&g_ecc_dtcm1) == 1u))
    {
        HAL_RAMECC_IRQHandler(&g_ecc_dtcm1);
    }
}

#else
/******************************* Include Files *******************************/

#include "generic_hal.h"

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