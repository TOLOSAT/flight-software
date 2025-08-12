/**
 * @file    drv_spi.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for SPI functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/peripherals/drv_spi.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              SpiOpen(spiDesc_t *spi_desc, const spiConf_t *const spi_conf)
 * @brief           Function that initialise a SPI connection
 * @param[in,out]   spi_desc   Descriptor that contains SPI handlers
 * @param[in]       spi_conf   Configuration that contains SPI parameters
 * @retval          #RET_SUCCESSFUL always
 *
 * @warning         SPI is not available on CMSDK
 */
returnCode_t SpiOpen(spiDesc_t *spi_desc, const spiConf_t *const spi_conf)
{
    (void)(spi_desc);
    (void)(spi_conf);
    return RET_SUCCESSFUL;
}

/**
 * @fn          SpiWrite(spiDesc_t *spi_desc, data_t data, length_t length)
 * @brief       Function that write over a SPI connection
 * @param[in]   spi_desc    Instance that contains SPI parameters and SPI Handler
 * @param[in]   data        Message we want to send
 * @param[in]   length      Size of the message we want to sent
 * @retval      #RET_SUCCESSFUL always
 *
 * @warning     SPI is not available on CMSDK
 */
returnCode_t SpiWrite(spiDesc_t *spi_desc, data_t data, length_t length)
{
    (void)(spi_desc);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @fn          SpiRead(spiDesc_t *spi_desc, data_t data, length_t length)
 * @brief       Function that read over SPI connection
 * @param[in]   spi_desc        Instance that contains SPI parameters and SPI Handler
 * @param[in]   slave_addr      Adress of the slave to which the message will be requested
 * @param[out]  data            Message we want to receive
 * @param[in]   length          Size of the message we want to receive
 * @retval      #RET_SUCCESSFUL always
 *
 * @warning     SPI is not available on CMSDK
 */
returnCode_t SpiRead(spiDesc_t *spi_desc, data_t data, length_t length)
{
    (void)(spi_desc);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @fn              SpiIoctl(spiDesc_t *spi_desc, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   spi_desc    Instance that contains SPI parameters and SPI Handler
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_SUCCESSFUL always
 *
 * @warning         SPI is not available on CMSDK
 */
returnCode_t SpiIoctl(spiDesc_t *spi_desc, uint32_t cmd, void *data, uint32_t data_size)
{
    (void)(spi_desc);
    (void)(cmd);
    (void)(data);
    (void)(data_size);
    return RET_SUCCESSFUL;
}

/**
 * @fn              SpiClose(spiDesc_t *spi_desc)
 * @brief           Function that desinit the SPI connection and puts defaults parameters
 * @param[in,out]   spi_desc    Instance that contains SPI parameters and SPI Handler
 * @retval          #RET_SUCCESSFUL always
 *
 * @warning         SPI is not available on CMSDK
 */
returnCode_t SpiClose(spiDesc_t *spi_desc)
{
    (void)(spi_desc);
    return RET_SUCCESSFUL;
}
