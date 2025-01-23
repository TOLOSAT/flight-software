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

#define T_LSI (1./32u) /**< Time of the LSI clock, in seconds*/

/**
 * @def     MS_TO_WDG_COUNTER_VALUE(timeout_ms)
 * @brief   Calculates the IWDG counter value for a given timeout in ms
 * @see     STM32WB-IWDG Revision 1.0
 * IWDG_PRESCALER_4 = 4 * 2^0, so PR = 0
 */
#define MS_TO_WDG_COUNTER_VALUE(timeout_ms) (((timeout_ms) / (T_LSI * 4 * (1<<IWDG_PRESCALER_4))) - 1u)

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static IWDG_HandleTypeDef wdg_inst = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitWatchDog()
 * @brief   Initialises the watchdog
 * @retval  #RET_ERROR if watchdog init failed
 * @retval  #RET_SUCCESSFUL else
 */
returnCode_t InitWatchDog(int timeout_ms)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_val;

    // Function Core
    wdg_inst.Instance = IWDG;
    wdg_inst.Init.Prescaler = IWDG_PRESCALER_4;
    wdg_inst.Init.Reload = MS_TO_WDG_COUNTER_VALUE(timeout_ms);
    test_val = HAL_IWDG_Init(&wdg_inst);
    if (test_val != HAL_OK) {
        return_value = RET_ERROR;
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
