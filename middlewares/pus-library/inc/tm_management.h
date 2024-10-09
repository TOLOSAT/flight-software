/**
 * @file    tm_management.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TM management
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup tm_management TM management
 * @brief PUS telemetry (TM) management layer.
 * @{
 */

#ifndef TM_MANAGEMENT_H
#define TM_MANAGEMENT_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern uint16_t g_tm_counter;

/*************************** Functions Declarations **************************/

extern returnCode_t SendTM(pusTM_t *tm, deviceNo_t dev_tm);
extern returnCode_t BuildTM(pusTM_t *tm, pusService_t service, pusSubService_t subservice, pusData_t *data, uint16_t data_size);
extern returnCode_t FormatTM(pusTM_t *tm);
extern void EraseTM(pusTM_t *tm);

#endif /* TM_MANAGEMENT_H */

/** 
 * @}
 * @}
 * @}
 */