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

#define TIMEOUT_MS 500u /**< Timeout of the watchdog, in milliseconds >*/
#define T_LSI (1u/32000u) /**< Time of the LSI clock, in seconds >*/

/**
 * @def     MS_TO_WDG_COUNTER_VALUE(timeout_ms)
 * @brief   Calculates the IWDG counter value for a given timeout in ms
 * @see <a href="https://www.st.com/resource/en/product_training/STM32WB-WDG_TIMERS-Independent-Watchdog-IWDG.pdf#page=10">Reference</a>
 * IWDG_PRESCALER_4 = 4 * 2^0, so PR = 0
 */
#define MS_TO_WDG_COUNTER_VALUE(timeout_ms) (((timeout_ms) / (T_LSI * IWDG_PRESCALER_4)) - 1u)

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static IWDG_HandleTypeDef wdg_inst = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn    InitWatchDog()
 * @brief  Initialises the watchdog 
 *
 * @return returnCode_t 
 */
returnCode_t InitWatchDog(void) 
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_val;

    // Function Core
    wdg_inst.Instance = IWDG;
    wdg_inst.Init.Prescaler = IWDG_PRESCALER_4;
    wdg_inst.Init.Reload = MS_TO_WDG_COUNTER_VALUE(TIMEOUT_MS);
    test_val = HAL_IWDG_Init(&wdg_inst);
    if (test_val != HAL_OK) {
        return_value = RET_ERROR;
    }

    return return_value;
}

/**
 * @fn    PetWatchDog()
 * @brief  Reloads the watchdog
 * 
 * @return Nothing
 */
void PetWatchDog(void)
{
    __HAL_IWDG_RELOAD_COUNTER(&wdg_inst);
}
