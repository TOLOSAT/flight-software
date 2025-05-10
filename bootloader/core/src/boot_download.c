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

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

#if defined(CONFIG_USB_OTG)
static USBD_HandleTypeDef usb_device;
#endif

/*************************** Functions Definitions ***************************/

/**
 * @fn      DownloadModeInit(void)
 * @brief   Specific initialisation for upload mode
 */
void DownloadModeInit(void)
{
#if defined(CONFIG_USB_OTG)
    /* Init Device Library, add supported class and start the library. */
    if (USBD_Init(&usb_device, &FS_Desc, DEVICE_FS) != USBD_OK)
    {
        ErrorHandler();
    }
    if (USBD_RegisterClass(&usb_device, &USBD_MSC) != USBD_OK)
    {
        ErrorHandler();
    }
    if (USBD_MSC_RegisterStorage(&usb_device, &USBD_Storage_Interface_fops_FS) != USBD_OK)
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
