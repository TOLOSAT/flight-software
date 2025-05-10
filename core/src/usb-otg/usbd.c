/**
 ******************************************************************************
 * @file           : usbd.c
 * @version        : v1.0_Cube
 * @brief          : This file implements the USB Device
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "usbd_core.h"
#include "usbd_msc.h"
#include "usb-otg/usbd.h"
#include "usb-otg/usbd_desc.h"
#include "usb-otg/usbd_storage_if.h"
#include "boot_fdir.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* USB Device Core handle declaration. */
USBD_HandleTypeDef hUsbDeviceFS;

/**
 * Init USB device Library, add supported class and start the library
 * @retval None
 */
void MX_USB_DEVICE_Init(void)
{
    /* Init Device Library, add supported class and start the library. */
    if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
    {
        ErrorHandler();
    }
    if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_MSC) != USBD_OK)
    {
        ErrorHandler();
    }
    if (USBD_MSC_RegisterStorage(&hUsbDeviceFS, &USBD_Storage_Interface_fops_FS) != USBD_OK)
    {
        ErrorHandler();
    }
    if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
    {
        ErrorHandler();
    }

    HAL_PWREx_EnableUSBVoltageDetector();
}

/**
 * @}
 */

/**
 * @}
 */
