/**
 ******************************************************************************
 * @file           : usbd_desc.h
 * @version        : v1.0_Cube
 * @brief          : Header for usbd_conf.c file.
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

#ifndef USBD_DESC_H
#define USBD_DESC_H

/******************************* Include Files *******************************/

#include "usbd_def.h"

/***************************** Macros Definitions ****************************/

#define DEVICE_ID1            (UID_BASE)
#define DEVICE_ID2            (UID_BASE + 0x4)
#define DEVICE_ID3            (UID_BASE + 0x8)

#define USB_SIZ_STRING_SERIAL 0x1A

/*************************** Variables Declarations **************************/

extern USBD_DescriptorsTypeDef g_usbd_descriptor_if;

#endif /* USBD_DESC_H */
