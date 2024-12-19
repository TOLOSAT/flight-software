/**
 * @file    drv_WDG.c
 * @author  Matteo Planchet
 * @author  Mathis Steinberger
 * @brief   Source file for WDG functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_wdg.h"

/***************************** Macros Definitions ****************************/

#define WDG_DOWN_COUNTER_VALUE 3999u   /**< Down counter value for 0.5s */
// TODO: Make a formula macro with the timeout as input

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static IWDG_HandleTypeDef wdg_inst = {0};

/*************************** Functions Definitions ***************************/

returnCode_t InitWatchDog(void) 
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_val;

    // Function Core
    wdg_inst.Instance = IWDG;
    wdg_inst.Init.Prescaler = IWDG_PRESCALER_4;
    wdg_inst.Init.Reload = WDG_DOWN_COUNTER_VALUE;
    test_val = HAL_IWDG_Init(&wdg_inst);
    if (test_val != HAL_OK) {
        return_value = RET_ERROR;
    }

    return return_value;
}
void PetWatchDog(void)
{
    __HAL_IWDG_RELOAD_COUNTER(&wdg_inst);
}
