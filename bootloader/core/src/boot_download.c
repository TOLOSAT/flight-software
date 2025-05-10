/**
 * @file    boot_download.c
 * @author  Merlin Kooshmanian
 * @brief   Source file that includes function for SW download through USB OTG
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "boot_download.h"
#include "boot_misc.h"
#include "boot_fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      StartSoftware(void)
 * @brief   Start the newly updated software
 * @return  Nothing
 */
void DownloadMode(void)
{
    // Infinite loop until new reset
    // because Mass Storage Class USB
    // device is handled by interrupts
    while (1)
    {
        HAL_GPIO_TogglePin(LED_ERROR_PORT, LED_ERROR_PIN);
        HAL_GPIO_TogglePin(LED_STATUS_PORT, LED_STATUS_PIN);
        HAL_Delay(500);
    }
}