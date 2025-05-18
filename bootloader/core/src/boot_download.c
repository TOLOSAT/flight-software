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

#if defined(CONFIG_USB_OTG)
#include "usbd_core.h"
#include "usbd_msc.h"
#include "usb-otg/usbd_desc.h"
#include "usb-otg/usbd_storage_if.h"
#endif

/***************************** Macros Definitions ****************************/

#if defined(CONFIG_USB_OTG)
#define DOWNLOAD_MODE_PIN_STATE GPIO_PIN_RESET /**< Pin state indicating that the user has selected upload mode */
#define UPLOAD_DELAY            1000u          /**< Delay (in ms) during when the button is polled */
#endif

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      IsDownloadMode(void)
 * @brief   Indicates if the download mode is selected or not
 * @retval  true if the mode is download mode
 * @retval  false else
 */
bool IsDownloadMode(void)
{
    bool is_download_mode = false;

#if defined(CONFIG_USB_OTG)
    uint32_t start_time = HAL_GetTick();

    // Check the pin state repeatedly during UPLOAD_DELAY
    while (((HAL_GetTick() - start_time) < UPLOAD_DELAY) && (is_download_mode == false))
    {
        if (HAL_GPIO_ReadPin(USER_BUTTON_PORT, USER_BUTTON_PIN) == DOWNLOAD_MODE_PIN_STATE)
        {
            is_download_mode = true;
        }
    }
#endif /* CONFIG_USB_OTG */

    return is_download_mode;
}

/**
 * @fn      DownloadModeInit(void)
 * @brief   Specific initialisation for upload mode
 */
void DownloadModeInit(void)
{
#if defined(CONFIG_USB_OTG)
    static USBD_HandleTypeDef usb_device = { 0 };

    /* Init Device Library, add supported class and start the library. */
    if (USBD_Init(&usb_device, &g_usbd_descriptor_if, DEVICE_FS) != USBD_OK)
    {
        ErrorHandler();
    }
    if (USBD_RegisterClass(&usb_device, &USBD_MSC) != USBD_OK)
    {
        ErrorHandler();
    }
    if (USBD_MSC_RegisterStorage(&usb_device, &g_usbd_storage_if) != USBD_OK)
    {
        ErrorHandler();
    }
    if (USBD_Start(&usb_device) != USBD_OK)
    {
        ErrorHandler();
    }

    HAL_PWREx_EnableUSBVoltageDetector();
#endif
}

/**
 * @fn      DownloadMode(void)
 * @brief   Download mode procesure
 * @return  Nothing
 *
 * @note The download mode procedure is just
 * @note an infinite loop because because Mass
 * @note Storage USB device is handled by irqs
 */
void DownloadMode(void)
{
    // Infinite loop until new reset
    while (1)
    {
        HAL_GPIO_TogglePin(LED_ERROR_PORT, LED_ERROR_PIN);
        HAL_GPIO_TogglePin(LED_STATUS_PORT, LED_STATUS_PIN);
        HAL_Delay(500u);
    }
}
