/**
 * @file    drv_ow.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for OW functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/peripherals/drv_ow.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              OwOpen(owInst_t *ow_inst)
 * @brief           Function that initialises an One Wire peripheral
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #RET_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #RET_SUCCESSFUL else
 *
 * @warning         OW is not available on CMSDK
 */
returnCode_t OwOpen(owInst_t *ow_inst)
{
    (void)(ow_inst);
    return RET_SUCCESSFUL;
}

/**
 * @fn          OwWrite(owInst_t *ow_inst, data_t data, length_t length)
 * @brief       Function that writes a message onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   data    Message to write
 * @param[in]   length  Number of byte to write
 * @retval      #RET_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #RET_SUCCESSFUL else
 *
 * @warning         OW is not available on CMSDK
 */
returnCode_t OwWrite(owInst_t *ow_inst, data_t data, length_t length)
{
    (void)(ow_inst);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @fn          OwRead(owInst_t *ow_inst, data_t data, length_t length)
 * @brief       Function that reads a message onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[out]  data    Message read
 * @param[in]   length  Number of byte to read
 * @retval      #RET_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #RET_SUCCESSFUL else
 *
 * @warning         OW is not available on CMSDK
 */
returnCode_t OwRead(owInst_t *ow_inst, data_t data, length_t length)
{
    (void)(ow_inst);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @fn              OwIoctl(owInst_t *ow_inst, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           One Wire IO control function (currently used to init One Wire connection)
 * @param[in]       ow_inst     Instance that contains One Wire parameters handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #RET_SUCCESSFUL else
 *
 * @warning         OW is not available on CMSDK
 */
returnCode_t OwIoctl(owInst_t *ow_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    (void)(ow_inst);
    (void)(cmd);
    (void)(data);
    (void)(data_size);
    return RET_SUCCESSFUL;
}

/**
 * @fn              OwClose(owInst_t *ow_inst)
 * @brief           Function that uninitialises an One Wire peripheral
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #RET_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #RET_SUCCESSFUL else
 *
 * @warning         OW is not available on CMSDK
 */
returnCode_t OwClose(owInst_t *ow_inst)
{
    (void)(ow_inst);
    return RET_SUCCESSFUL;
}
