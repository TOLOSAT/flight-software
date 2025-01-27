/**
 * @file    drv_wdg.c
 * @author  Matteo Planchet
 * @author  Mathis Steinberger
 * @brief   Source file for WDG functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_wdg.h"

/***************************** Macros Definitions ****************************/

#define WDG_BASE_FREC_KHZ      32u                                                              /**< Watchdog base clock (LSI) frequency in kHz */
#define WDG_PRESCALER          IWDG_PRESCALER_32                                                /**< Watchdog counter prescaler*/
#define WDG_MAX_TIMEOUT_MS     ((4096u * 4u *(1u << IWDG_PRESCALER_32)) / WDG_BASE_FREC_KHZ)    /**< Watchdog maximum timeout value in ms */

/**
 * @def     MS_TO_WDG_COUNTER_VALUE(timeout_ms)
 * @brief   Calculates the IWDG counter value for a given timeout in ms
 * @see     STM32WB-IWDG Revision 1.0
 */
#define MS_TO_WDG_COUNTER_VALUE(timeout_ms) ((((timeout_ms) * WDG_BASE_FREC_KHZ) / (4u * (1u << WDG_PRESCALER))) - 1u)

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static IWDG_HandleTypeDef wdg_inst = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitWatchDog(uint32_t timeout_ms)
 * @brief   Initialises the watchdog
 * @retval  #RET_ERROR if watchdog init failed
 * @retval  #RET_SUCCESSFUL else
 */
returnCode_t InitWatchDog(uint32_t timeout_ms)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_val;

    // Function Core
    if (timeout_ms < WDG_MAX_TIMEOUT_MS)
    {
        wdg_inst.Instance = WATCHDOG_REF;
        wdg_inst.Init.Prescaler = WDG_PRESCALER;
        wdg_inst.Init.Reload = MS_TO_WDG_COUNTER_VALUE(timeout_ms);
    #if defined(STM32H7)
        wdg_inst.Init.Window = MS_TO_WDG_COUNTER_VALUE(timeout_ms);
    #endif
        test_val = HAL_IWDG_Init(&wdg_inst);
        if (test_val != HAL_OK) {
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
 * @fn    PetWatchDog()
 * @brief  Reloads the watchdog
 * @return Nothing
 */
void PetWatchDog(void)
{
    __HAL_IWDG_RELOAD_COUNTER(&wdg_inst);
}
