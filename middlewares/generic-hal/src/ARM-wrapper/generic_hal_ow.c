/**
 * @file    generic_hal_ow.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC HAL OW functions
 * @date    11/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              OwOpen(owInst_t *ow_inst)
 * @brief           Function that initialises an One Wire peripheral
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #GEN_HAL_SUCCESSFUL always
 * 
 * @warning         OW is not available on CMSDK
 */
halStatus_t IN_OW_TEXT_SECTION OwOpen(owInst_t *ow_inst)
{
    (void)(ow_inst);
    return GEN_HAL_SUCCESSFUL;
}

/**
 * @fn          OwWrite(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
 * @brief       Function that writes a message onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   msg Message to write
 * @param[in]   length Number of byte to write
 * @retval      #GEN_HAL_SUCCESSFUL always
 * 
 * @warning     OW is not available on CMSDK
 */
halStatus_t IN_OW_TEXT_SECTION OwWrite(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
{
    (void)(ow_inst);
    (void)(msg);
    (void)(length);
    return GEN_HAL_SUCCESSFUL;
}

/**
 * @fn          OwRead(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
 * @brief       Function that reads a message onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[out]  msg Message read
 * @param[in]   length Number of byte to read
 * @retval      #GEN_HAL_SUCCESSFUL always
 * 
 * @warning     OW is not available on CMSDK
 */
halStatus_t IN_OW_TEXT_SECTION OwRead(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
{
    (void)(ow_inst);
    (void)(msg);
    (void)(length);
    return GEN_HAL_SUCCESSFUL;
}

/**
 * @fn              OwIoctl(owInst_t *ow_inst, halIoCtlCmd_t io_cmd)
 * @brief           One Wire IO control function (currently used to init One Wire connection)
 * @param[in]       ow_inst Instance that contains One Wire parameters handlers
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #GEN_HAL_SUCCESSFUL always
 * 
 * @warning         OW is not available on CMSDK
 */
halStatus_t IN_OW_TEXT_SECTION OwIoctl(owInst_t *ow_inst, halIoCtlCmd_t io_cmd)
{
    (void)(ow_inst);
    (void)(io_cmd);
    return GEN_HAL_SUCCESSFUL;
}

/**
 * @fn              OwClose(owInst_t *ow_inst)
 * @brief           Function that uninitialises an One Wire peripheral
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #GEN_HAL_SUCCESSFUL always
 * 
 * @warning         OW is not available on CMSDK
 */
halStatus_t IN_OW_TEXT_SECTION OwClose(owInst_t *ow_inst)
{
    (void)(ow_inst);
    return GEN_HAL_SUCCESSFUL;
}
