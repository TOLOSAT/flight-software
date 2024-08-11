/**
 * @file    generic_hal_i2c.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC HAL I2C functions
 * @date    30/04/2023
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
 * @fn              I2cOpen(i2cInst_t *i2c_inst)
 * @brief           Function that initialise a I2C connection
 * @param[in,out]   i2c_inst Instance that contains I2C parameters and I2C Handler
 * @retval          #GEN_HAL_SUCCESSFUL always
 * 
 * @warning         I2C is not available on CMSDK
 */
halStatus_t IN_GENERIC_HAL_TEXT_SECTION I2cOpen(i2cInst_t *i2c_inst)
{
    (void)(i2c_inst);
    return GEN_HAL_SUCCESSFUL;
}

/**
 * @fn          I2cWrite(i2cInst_t *i2c_inst, i2cSlaveAddr_t slave_addr, i2cMsg_t *msg, i2cMsgLength_t length)
 * @brief       Function that write over a I2C connection
 * @param[in]   i2c_inst Instance that contains I2C parameters and I2C Handler
 * @param[in]   slave_addr Adress of the slave to which the message will be send
 * @param[in]   msg Message we want to send
 * @param[in]   length Size of the message we want to sent
 * @retval      #GEN_HAL_SUCCESSFUL always
 * 
 * @warning     I2C is not available on CMSDK
 */
halStatus_t IN_GENERIC_HAL_TEXT_SECTION I2cWrite(i2cInst_t *i2c_inst, i2cSlaveAddr_t slave_addr, i2cMsg_t *msg, i2cMsgLength_t length)
{
    (void)(i2c_inst);
    (void)(slave_addr);
    (void)(msg);
    (void)(length);
    return GEN_HAL_SUCCESSFUL;
}

/**
 * @fn          I2cRead(i2cInst_t *i2c_inst, i2cSlaveAddr_t slave_addr, i2cMsg_t *msg, i2cMsgLength_t length)
 * @brief       Function that read over I2C connection
 * @param[in]   i2c_inst Instance that contains I2C parameters and I2C Handler
 * @param[in]   slave_addr Adress of the slave to which the message will be requested
 * @param[out]  msg Message we want to receive
 * @param[in]   length Size of the message we want to receive
 * @retval      #GEN_HAL_SUCCESSFUL always
 * 
 * @warning     I2C is not available on CMSDK
 */
halStatus_t IN_GENERIC_HAL_TEXT_SECTION I2cRead(i2cInst_t *i2c_inst, i2cSlaveAddr_t slave_addr, i2cMsg_t *msg, i2cMsgLength_t length)
{
    (void)(i2c_inst);
    (void)(slave_addr);
    (void)(msg);
    (void)(length);
    return GEN_HAL_SUCCESSFUL;
}

/**
 * @fn              I2cIoctl(i2cInst_t *i2c_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   i2c_inst Instance that contains I2C parameters and I2C Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #GEN_HAL_SUCCESSFUL always
 * 
 * @warning         I2C is not available on CMSDK
 */
halStatus_t IN_GENERIC_HAL_TEXT_SECTION I2cIoctl(i2cInst_t *i2c_inst, halIoCtlCmd_t io_cmd)
{
    (void)(i2c_inst);
    (void)(io_cmd);
    return GEN_HAL_SUCCESSFUL;
}

/**
 * @fn              I2cClose(i2cInst_t *i2c_inst)
 * @brief           Function that desinit the I2C connection and puts defaults parameters
 * @param[in,out]   i2c_inst Instance that contains I2C parameters and I2C Handler
 * @retval          #GEN_HAL_SUCCESSFUL always
 * 
 * @warning         I2C is not available on CMSDK
 */
halStatus_t IN_GENERIC_HAL_TEXT_SECTION I2cClose(i2cInst_t *i2c_inst)
{
    (void)(i2c_inst);
    return GEN_HAL_SUCCESSFUL;
}