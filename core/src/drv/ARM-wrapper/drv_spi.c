/**
 * @file    drv_spi.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for SPI functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core.h"
#include "drv/drv_types.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              SpiOpen(spiInst_t *spi_inst)
 * @brief           Function that initialise a SPI connection
 * @param[in,out]   spi_inst Instance that contains SPI parameters and SPI Handler
 * @retval          #CORE_SUCCESSFUL always
 * 
 * @warning         SPI is not available on CMSDK
 */
coreStatus_t IN_CORE_TEXT_SECTION SpiOpen(spiInst_t *spi_inst)
{
    (void)(spi_inst);
    return CORE_SUCCESSFUL;
}

/**
 * @fn          SpiWrite(spiInst_t *spi_inst, spiMsg_t *msg, spiMsgLength_t length)
 * @brief       Function that write over a SPI connection
 * @param[in]   spi_inst Instance that contains SPI parameters and SPI Handler
 * @param[in]   msg Message we want to send
 * @param[in]   length Size of the message we want to sent
 * @retval      #CORE_SUCCESSFUL always
 * 
 * @warning     SPI is not available on CMSDK
 */
coreStatus_t IN_CORE_TEXT_SECTION SpiWrite(spiInst_t *spi_inst, spiMsg_t *msg, spiMsgLength_t length)
{
    (void)(spi_inst);
    (void)(msg);
    (void)(length);
    return CORE_SUCCESSFUL;
}

/**
 * @fn          SpiRead(spiInst_t *spi_inst, spiMsg_t *received_msg, spiMsg_t *transmit_msg, spiMsgLength_t length)
 * @brief       Function that read over SPI connection
 * @param[in]   spi_inst Instance that contains SPI parameters and SPI Handler
 * @param[in]   slave_addr Adress of the slave to which the message will be requested
 * @param[out]  received_msg Message we want to receive
 * @param[in]   transmit_msg Message we will transmit while we receive (if NULL then 0 will be send instead)
 * @param[in]   length Size of the message we want to receive
 * @retval      #CORE_SUCCESSFUL always
 * 
 * @warning     SPI is not available on CMSDK
 */
coreStatus_t IN_CORE_TEXT_SECTION SpiRead(spiInst_t *spi_inst, spiMsg_t *received_msg, spiMsg_t *transmit_msg, spiMsgLength_t length)
{
    (void)(spi_inst);
    (void)(received_msg);
    (void)(transmit_msg);
    (void)(length);
    return CORE_SUCCESSFUL;
}

/**
 * @fn              SpiIoctl(spiInst_t *spi_inst, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   spi_inst Instance that contains SPI parameters and SPI Handler
 * @param[in]       cmd IO Control command
 * @param[in,out]   data IO Control command
 * @param[in]       data_size IO Control data size
 * @retval          #CORE_SUCCESSFUL always
 * 
 * @warning         SPI is not available on CMSDK
 */
coreStatus_t IN_CORE_TEXT_SECTION SpiIoctl(spiInst_t *spi_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    (void)(spi_inst);
    (void)(cmd);
    (void)(data);
    (void)(data_size);
    return CORE_SUCCESSFUL;
}

/**
 * @fn              SpiClose(spiInst_t *spi_inst)
 * @brief           Function that desinit the SPI connection and puts defaults parameters
 * @param[in,out]   spi_inst Instance that contains SPI parameters and SPI Handler
 * @retval          #CORE_SUCCESSFUL always
 * 
 * @warning         SPI is not available on CMSDK
 */
coreStatus_t IN_CORE_TEXT_SECTION SpiClose(spiInst_t *spi_inst)
{
    (void)(spi_inst);
    return CORE_SUCCESSFUL;
}
