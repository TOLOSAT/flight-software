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
 * @fn              OwOpen(owDesc_t *ow_desc, const owConf_t *const ow_conf)
 * @brief           Function that initialises an One Wire peripheral
 * @param[in,out]   ow_desc   Descriptor that contains OW handlers
 * @param[in]       ow_conf   Configuration that contains OW parameters
 * @retval          #RET_INVALID_PARAM if ow_desc is a null pointer
 * @retval          #RET_SUCCESSFUL else
 *
 * @warning         OW is not available on CMSDK
 */
returnCode_t OwOpen(owDesc_t *ow_desc, const owConf_t *const ow_conf)
{
    (void)(ow_desc);
    (void)(ow_conf);
    return RET_SUCCESSFUL;
}

/**
 * @fn          OwWrite(owDesc_t *ow_desc, data_t data, length_t length)
 * @brief       Function that writes a message onto One Wire
 * @param[in]   ow_desc Instance that contains One Wire parameters handlers
 * @param[in]   data    Message to write
 * @param[in]   length  Number of byte to write
 * @retval      #RET_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #RET_SUCCESSFUL else
 *
 * @warning         OW is not available on CMSDK
 */
returnCode_t OwWrite(owDesc_t *ow_desc, data_t data, length_t length)
{
    (void)(ow_desc);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @fn          OwRead(owDesc_t *ow_desc, data_t data, length_t length)
 * @brief       Function that reads a message onto One Wire
 * @param[in]   ow_desc Instance that contains One Wire parameters handlers
 * @param[out]  data    Message read
 * @param[in]   length  Number of byte to read
 * @retval      #RET_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #RET_SUCCESSFUL else
 *
 * @warning         OW is not available on CMSDK
 */
returnCode_t OwRead(owDesc_t *ow_desc, data_t data, length_t length)
{
    (void)(ow_desc);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @fn              OwIoctl(owDesc_t *ow_desc, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           One Wire IO control function (currently used to init One Wire connection)
 * @param[in]       ow_desc     Instance that contains One Wire parameters handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if ow_desc is a null pointer
 * @retval          #RET_SUCCESSFUL else
 *
 * @warning         OW is not available on CMSDK
 */
returnCode_t OwIoctl(owDesc_t *ow_desc, uint32_t cmd, void *data, uint32_t data_size)
{
    (void)(ow_desc);
    (void)(cmd);
    (void)(data);
    (void)(data_size);
    return RET_SUCCESSFUL;
}

/**
 * @fn              OwClose(owDesc_t *ow_desc)
 * @brief           Function that uninitialises an One Wire peripheral
 * @param[in,out]   ow_desc Instance that contains One Wire parameters handlers
 * @retval          #RET_INVALID_PARAM if ow_desc is a null pointer
 * @retval          #RET_SUCCESSFUL else
 *
 * @warning         OW is not available on CMSDK
 */
returnCode_t OwClose(owDesc_t *ow_desc)
{
    (void)(ow_desc);
    return RET_SUCCESSFUL;
}
