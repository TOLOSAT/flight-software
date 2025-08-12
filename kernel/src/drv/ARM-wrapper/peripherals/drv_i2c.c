/**
 * @file    drv_i2c.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for I2C functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/peripherals/drv_i2c.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              I2cOpen(i2cDesc_t *i2c_desc, const i2cConf_t *const i2c_conf)
 * @brief           Function that initialise a I2C connection
 * @param[in,out]   i2c_desc   Descriptor that contains I2C handlers
 * @param[in]       i2c_conf   Configuration that contains I2C parameters
 * @retval          #RET_SUCCESSFUL always
 *
 * @warning         I2C is not available on CMSDK
 */
returnCode_t I2cOpen(i2cDesc_t *i2c_desc, const i2cConf_t *const i2c_conf)
{
    (void)(i2c_desc);
    (void)(i2c_conf);
    return RET_SUCCESSFUL;
}

/**
 * @fn          I2cWrite(i2cDesc_t *i2c_desc, data_t data, length_t length)
 * @brief       Function that write over a I2C connection
 * @param[in]   i2c_desc    Instance that contains I2C parameters and I2C Handler
 * @param[in]   data        Message we want to send
 * @param[in]   length      Size of the message we want to sent
 * @retval      #RET_SUCCESSFUL always
 *
 * @warning     I2C is not available on CMSDK
 */
returnCode_t I2cWrite(i2cDesc_t *i2c_desc, data_t data, length_t length)
{
    (void)(i2c_desc);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @fn          I2cRead(i2cDesc_t *i2c_desc, data_t data, length_t length)
 * @brief       Function that read over I2C connection
 * @param[in]   i2c_desc    Instance that contains I2C parameters and I2C Handler
 * @param[out]  data        Message we want to receive
 * @param[in]   length      Size of the message we want to receive
 * @retval      #RET_SUCCESSFUL always
 *
 * @warning     I2C is not available on CMSDK
 */
returnCode_t I2cRead(i2cDesc_t *i2c_desc, data_t data, length_t length)
{
    (void)(i2c_desc);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @fn              I2cIoctl(i2cDesc_t *i2c_desc, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   i2c_desc    Instance that contains I2C parameters and I2C Handler
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_SUCCESSFUL always
 *
 * @warning         I2C is not available on CMSDK
 */
returnCode_t I2cIoctl(i2cDesc_t *i2c_desc, uint32_t cmd, void *data, uint32_t data_size)
{
    (void)(i2c_desc);
    (void)(cmd);
    (void)(data);
    (void)(data_size);
    return RET_SUCCESSFUL;
}

/**
 * @fn              I2cClose(i2cDesc_t *i2c_desc)
 * @brief           Function that desinit the I2C connection and puts defaults parameters
 * @param[in,out]   i2c_desc    Instance that contains I2C parameters and I2C Handler
 * @retval          #RET_SUCCESSFUL always
 *
 * @warning         I2C is not available on CMSDK
 */
returnCode_t I2cClose(i2cDesc_t *i2c_desc)
{
    (void)(i2c_desc);
    return RET_SUCCESSFUL;
}