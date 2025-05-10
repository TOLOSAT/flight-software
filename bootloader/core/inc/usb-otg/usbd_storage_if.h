/**
 ******************************************************************************
 * @file           : usbd_storage_if.h
 * @version        : v1.0_Cube
 * @brief          : Header for usbd_storage_if.c file.
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

#ifndef __USBD_STORAGE_IF_H__
#define __USBD_STORAGE_IF_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_msc.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
 * @brief For Usb device.
 * @{
 */

/** @defgroup USBD_STORAGE USBD_STORAGE
 * @brief Header file for the usb_storage_if.c file
 * @{
 */

/** @defgroup USBD_STORAGE_Exported_Defines USBD_STORAGE_Exported_Defines
 * @brief Defines.
 * @{
 */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Exported_Types USBD_STORAGE_Exported_Types
 * @brief Types.
 * @{
 */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Exported_Macros USBD_STORAGE_Exported_Macros
 * @brief Aliases.
 * @{
 */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Exported_Variables USBD_STORAGE_Exported_Variables
 * @brief Public variables.
 * @{
 */

/** STORAGE Interface callback. */
extern USBD_StorageTypeDef USBD_Storage_Interface_fops_FS;

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Exported_FunctionsPrototype USBD_STORAGE_Exported_FunctionsPrototype
 * @brief Public functions declaration.
 * @{
 */

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

#endif /* __USBD_STORAGE_IF_H__ */
