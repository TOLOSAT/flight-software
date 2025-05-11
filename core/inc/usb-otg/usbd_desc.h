/**
 * @file    usbd_desc.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for USB descriptor interface layer
 * @note    Based on the STM32 usbd_desc_template.h
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef USBD_DESC_H
#define USBD_DESC_H

/******************************* Include Files *******************************/

#include "boot_types.h"
#include "usbd_def.h"

/***************************** Macros Definitions ****************************/

/*************************** Variables Declarations **************************/

extern USBD_DescriptorsTypeDef g_usbd_descriptor_if;

#endif /* USBD_DESC_H */
