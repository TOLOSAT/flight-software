/**
 * @file    usbd_storage_if.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for USB storage interface layer
 * @note    Based on the STM32 usbd_msc_storage_template.h
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef USBD_STORAGE_IF_H
#define USBD_STORAGE_IF_H

/******************************* Include Files *******************************/

#include "boot_types.h"
#include "usbd_msc.h"

/*************************** Variables Declarations **************************/

extern USBD_StorageTypeDef g_usbd_storage_if;

#endif /* USBD_STORAGE_IF_H */
