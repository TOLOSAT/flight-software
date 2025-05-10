/**
 ******************************************************************************
 * @file           : usbd.h
 * @version        : v1.0_Cube
 * @brief          : Header for usb_device.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DEVICE__H__
#define __USB_DEVICE__H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "usbd_def.h"

/** @addtogroup USBD_OTG_DRIVER
 * @{
 */

/** @defgroup USBD_DEVICE USBD_DEVICE
 * @brief Device file for Usb otg low level driver.
 * @{
 */

/** @defgroup USBD_DEVICE_Exported_Variables USBD_DEVICE_Exported_Variables
 * @brief Public variables.
 * @{
 */

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
 * @}
 */

/** @defgroup USBD_DEVICE_Exported_FunctionsPrototype USBD_DEVICE_Exported_FunctionsPrototype
 * @brief Declaration of public functions for Usb device.
 * @{
 */

/** USB Device initialization function. */
void MX_USB_DEVICE_Init(void);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __USB_DEVICE__H__ */
