/**
 * @file    sd_spi_driver.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for SD card SPI driver
 * @date    02/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "sd_spi_driver.h"
#include "sd_crc.h"
#include "tolosat_hal.h"
#include "io_instances.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static halStatus_t sdSendBytes(uint8_t *data, uint32_t size);
static halStatus_t sdReceiveBytes(uint8_t *data, uint32_t size);

/*************************** Variables Definitions ***************************/

SDCardStatus_t g_sd_card_status = SD_CARD_OFF; /**< Indicates if SD card is ON/OFF */
SDCardType_t g_sd_card_type = NOT_SDCARD;      /**< SD card type */

/*************************** Functions Definitions ***************************/

/**
 * @fn      SD_Select(void)
 * @brief   Select SD card on SPI bus
 * @retval  #FS_ERROR if SPI or GPIO error occured
 * @retval  #FS_SUCCESSFUL else
 */
fsStatus_t SD_Select(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = THAL_SUCCESSFUL;

    // Select slave
    test_hal = GpioWrite(&sd_card_cs, GPIO_PIN_RESET);
    if (test_hal == THAL_SUCCESSFUL)
    {
        // Then send a fill char onto MOSI
        uint8_t fill_char = SPI_FILL_CHAR;
        test_hal = sdSendBytes(&fill_char, 1u);
        if (test_hal != THAL_SUCCESSFUL)
        {
            return_value = FS_ERROR;
        }
    }
    else
    {
        return_value = FS_ERROR;
    }

    return return_value;
}

/**
 * @fn      SD_Unselect(void)
 * @brief   Unselect SD card on SPI bus
 * @retval  #FS_ERROR if SPI or GPIO error occured
 * @retval  #FS_SUCCESSFUL else
 */
fsStatus_t SD_Unselect(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = THAL_SUCCESSFUL;

    // Send a fill char onto MOSI
    uint8_t fill_char = SPI_FILL_CHAR;
    test_hal = sdSendBytes(&fill_char, 1u);
    if (test_hal == THAL_SUCCESSFUL)
    {
        // Then unselect slave
        test_hal = GpioWrite(&sd_card_cs, GPIO_PIN_SET);
        if (test_hal != THAL_SUCCESSFUL)
        {
            return_value = FS_ERROR;
        }
    }
    else
    {
        return_value = FS_ERROR;
    }

    return return_value;
}

/**
 * @fn      SD_WaitUntilReady(void)
 * @brief   Wait until SD card is ready
 * @retval  SPI_FILL_CHAR if SD card is ready
 * @retval  #FS_SUCCESSFUL if SD card is ready (spi slave register is now empty)
 * @retval  #FS_TIMEOUT if function timeouted before clearing SD card being ready
 * @retval  #FS_ERROR if SPI has encountered an error
 */
fsStatus_t SD_WaitUntilReady(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = THAL_SUCCESSFUL;
    uint8_t answer = 0u;
    uint32_t counter = 0u;

    // Read SD card until it returns SPI_FILL_CHAR or timeouted
    while ((test_hal == THAL_SUCCESSFUL) && (answer != SPI_FILL_CHAR) && (counter < SD_CNT_TIMEOUT))
    {
        test_hal = sdReceiveBytes(&answer, 1u);
        counter++;
    }

    if (counter >= SD_CNT_TIMEOUT)
    {
        return_value = FS_TIMEOUT;
    }

    if (test_hal == THAL_ERROR)
    {
        return_value = FS_ERROR;
    }

    return return_value;
}

/**
 * @fn      SD_SwitchOn(void)
 * @brief   Wake up the SD card an start initialize SPI mode
 * @retval  #FS_ERROR if SPI has encountered an error
 * @retval  #FS_TIMEOUT if SD card never answered IDLE state
 * @retval  #FS_SUCCESSFUL else
 */
fsStatus_t SD_SwitchOn(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = THAL_SUCCESSFUL;
    uint8_t wakeup_message[SD_WAKEUP_MSG_SIZE];
    uint8_t answer = SPI_FILL_CHAR;

    // Function Core
    // Wakeup SD card by sending pad caracter without selecting it
    (void)SD_Unselect();
    (void)memset(&wakeup_message, SPI_FILL_CHAR, SD_WAKEUP_MSG_SIZE);
    test_hal = sdSendBytes((uint8_t *)&wakeup_message, SD_WAKEUP_MSG_SIZE);

    // Continue only if SPI has not encountered an error
    if (test_hal == THAL_SUCCESSFUL)
    {
        uint8_t reset_spi_mode_cmd[CMD_MSG_SIZE] = {CMD0, 0x00u, 0x00u, 0x00u, 0x00u, 0x95u};

        // Select SD card
        (void)SD_Select();

        // Send reset onto spi mode command
        test_hal = sdSendBytes((uint8_t *)reset_spi_mode_cmd, CMD_MSG_SIZE);

        // Continue only if SPI has not encountered an error
        if (test_hal == THAL_SUCCESSFUL)
        {
            // Wait until SD card
            uint32_t counter = 0u;
            while ((test_hal == THAL_SUCCESSFUL) && (answer != SD_IDLE_FLAG) && (counter < SD_CNT_TIMEOUT))
            {
                test_hal = sdReceiveBytes(&answer, 1u);
                counter++;
            }

            // Unselect SD card
            (void)SD_Unselect();

            // Test if procedure wents well
            if ((test_hal == THAL_SUCCESSFUL) && (counter < SD_CNT_TIMEOUT))
            {
                g_sd_card_status = SD_CARD_ON;
            }
            else
            {
                g_sd_card_status = SD_CARD_OFF;
                if (counter >= SD_CNT_TIMEOUT)
                {
                    return_value = FS_TIMEOUT;
                }
                else
                {
                    return_value = FS_ERROR;
                }
            }
        }
        else
        {
            return_value = FS_ERROR;
        }
    }
    else
    {
        return_value = FS_ERROR;
    }

    return return_value;
}

/**
 * @fn      SD_SwitchOff(void)
 * @brief   Switch off the SD card
 * @retval  #FS_SUCCESSFUL always
 */
fsStatus_t SD_SwitchOff(void)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;

    // Function Core
    g_sd_card_status = SD_CARD_OFF;

    return return_value;
}

/**
 * @fn          SD_RxDataBlock(uint8_t *buff, uint32_t len)
 * @brief       Receives a block from SD card
 * @param[out]  buff Buffer containing the block received
 * @param[in]   len Length of the block
 * @retval      #FS_INVALID_PARAM if buff is null pointer or len is null
 * @retval      #FS_ERROR if SPI has encountered an error
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t SD_RxDataBlock(uint8_t *buff, uint32_t len)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    halStatus_t test_hal = THAL_SUCCESSFUL;
    uint8_t token = SPI_FILL_CHAR;

    // Function Core
    if ((buff != NULL) && (len != 0u))
    {
        // Loop until receive a response or timeout
        uint32_t counter = 0u;
        while ((test_hal == THAL_SUCCESSFUL) && (token == SPI_FILL_CHAR) && (counter < SD_CNT_TIMEOUT))
        {
            test_hal = sdReceiveBytes(&token, 1u);
            counter++;
        }

        // Check if read was successful and gets a start block token
        if ((token == SD_START_BLOCK_TOKEN) && (test_hal == THAL_SUCCESSFUL) && (counter < SD_CNT_TIMEOUT))
        {
            // Receive block
            test_hal = sdReceiveBytes(buff, len);

            // Check if block has corretly been read
            if (test_hal == THAL_SUCCESSFUL)
            {
                // Receive (and discard CRC)
                uint8_t crc[2] = {0};
                test_hal = sdReceiveBytes((uint8_t *)&crc, 2u);
                // Check if crc has corretly been read
                if (test_hal != THAL_SUCCESSFUL)
                {
                    return_value = FS_ERROR;
                }
            }
            else
            {
                return_value = FS_ERROR;
            }
        }
        else
        {
            return_value = FS_ERROR;
        }
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SD_TxDataBlock(const uint8_t *buff, uint32_t len, uint8_t token)
 * @brief       Sends a block from SD card
 * @param[in]   buff Buffer containing the block to send
 * @param[in]   len Length of the block
 * @param[in]   token Token indicating type of transmission
 * @retval      #FS_INVALID_PARAM if buff is null pointer or len is null except if token is SD_STOP_TOKEN
 * @retval      #FS_ERROR if SPI has encountered an error
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t SD_TxDataBlock(const uint8_t *buff, uint32_t len, uint8_t token)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;

    // Function Core
    if ((len != 0u) && (buff == NULL) && ((token == SD_STOP_TOKEN)))
    {
        return_value = FS_INVALID_PARAM;
    }
    else
    {
        // Wait until SD card is ready
        fsStatus_t test_wait = FS_SUCCESSFUL;
        test_wait = SD_WaitUntilReady();
        if (test_wait == FS_SUCCESSFUL)
        {
            // Send token
            halStatus_t test_hal = THAL_SUCCESSFUL;
            test_hal = sdSendBytes(&token, 1u);
            if (test_hal == THAL_SUCCESSFUL)
            {
                // if it's not STOP token, transmit data
                if (token != SD_STOP_TOKEN)
                {
                    test_hal = sdSendBytes((uint8_t *)buff, len); // cppcheck-suppress misra-c2012-11.8; Low-level drivers don't use the const argument so it has to disappear somewhere 
                    if (test_hal == THAL_SUCCESSFUL)
                    {
                        // Read and discard CRC
                        uint8_t crc[2] = {0};
                        test_hal = sdReceiveBytes((uint8_t *)&crc, 2u);
                        if (test_hal == THAL_SUCCESSFUL)
                        {
                            uint8_t answer = SPI_FILL_CHAR;
                            uint32_t counter = 0u;
                            while ((test_hal == THAL_SUCCESSFUL) && (answer == SPI_FILL_CHAR) && (counter < SD_CNT_TIMEOUT))
                            {
                                test_hal = sdReceiveBytes(&answer, 1u);
                                counter++;
                            }
                            // Check if we get the answer
                            if ((test_hal == THAL_SUCCESSFUL) && (answer != SPI_FILL_CHAR) && (counter < SD_CNT_TIMEOUT))
                            {
                                // Clear receive buffer until fill char is received
                                test_wait = SD_WaitUntilReady();
                                if (test_wait == FS_SUCCESSFUL)
                                {
                                    // Check if data has been accepted
                                    if ((answer & SD_DATA_RESPONSE_MASK) != SD_DATA_ACCEPTED)
                                    {
                                        return_value = FS_ERROR;
                                    }
                                }
                            }
                        }
                        else
                        {
                            return_value = FS_ERROR;
                        }
                    }
                    else
                    {
                        return_value = FS_ERROR;
                    }
                }
            }
            else
            {
                return_value = FS_ERROR;
            }
        }
        else
        {
            return_value = FS_ERROR;
        }
    }

    return return_value;
}

/**
 * @fn          SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size)
 * @brief       Sends a command to the SD card
 * @param[in]   cmd Command to send
 * @param[in]   arg Command argument
 * @param[out]  answer Command answer
 * @param[in]   answer_size Command answer size
 * @retval      #FS_INVALID_PARAM if command is invalid, or answer is null pointer but answer_size non null
 * @retval      #FS_TIMEOUT if SD card was not ready or CMD12 still busy
 * @retval      #FS_ERROR if an error occured
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    fsStatus_t test_wait;
    halStatus_t test_hal;

    // Function Core
    if ((answer_size != 0u) && (answer == NULL))
    {
        return_value = FS_INVALID_PARAM;
    }
    else
    {
        if ((cmd >= 0x40u) && (cmd <= 0x7fu))
        {
            // Wait until transfer complete
            test_wait = SD_WaitUntilReady();
            if (test_wait == FS_SUCCESSFUL)
            {
                uint8_t cmd_msg[CMD_MSG_SIZE] = {0};
                // Build command message with function arguments
                cmd_msg[0] = (uint8_t)(cmd);
                cmd_msg[1] = (uint8_t)((0xff000000u & arg) >> 24u);
                cmd_msg[2] = (uint8_t)((0x00ff0000u & arg) >> 16u);
                cmd_msg[3] = (uint8_t)((0x0000ff00u & arg) >> 8u);
                cmd_msg[4] = (uint8_t)(0x000000ffu & arg);
                cmd_msg[5] = ComputeCommandCRC7((uint8_t *)&cmd_msg);

                // Send Command
                test_hal = sdSendBytes((uint8_t *)&cmd_msg, CMD_MSG_SIZE);
                if (test_hal == THAL_SUCCESSFUL)
                {
                    uint32_t counter = 0u;
                    uint8_t command_status = SPI_FILL_CHAR;
                    while ((command_status == SPI_FILL_CHAR) && (test_hal == THAL_SUCCESSFUL) && (counter < SD_CNT_TIMEOUT))
                    {
                        test_hal = sdReceiveBytes(&command_status, 1u);
                        counter++;
                    }

                    // Check Result
                    if ((test_hal == THAL_SUCCESSFUL) && (command_status <= SD_IDLE_FLAG))
                    {
                        if ((cmd == CMD41) && (command_status != 0u))
                        {
                            return_value = FS_BUSY;
                        }
                        else
                        {
                            // If command is CMD12 (STOP_TRANSMISSION) wait until ready
                            if (cmd == CMD12)
                            {
                                test_wait = SD_WaitUntilReady();
                                if (test_wait != FS_SUCCESSFUL)
                                {
                                    return_value = FS_TIMEOUT;
                                }
                            }
                            else
                            {
                                if (answer_size != 0u)
                                {
                                    // Receive answer
                                    test_hal = sdReceiveBytes(answer, answer_size);

                                    // Check if everything wents well
                                    if (test_hal != THAL_SUCCESSFUL)
                                    {
                                        return_value = FS_ERROR;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        return_value = FS_ERROR;
                    }
                }
                else
                {
                    return_value = FS_ERROR;
                }
            }
            else
            {
                return_value = FS_TIMEOUT;
            }
        }
        else
        {
            return_value = FS_INVALID_PARAM;
        }
    }

    return return_value;
}

/**
 * @fn          sdSendBytes(uint8_t *data, uint32_t size)
 * @brief       Send bytes to SD card
 * @param[in]   data Data sended to SD card
 * @param[in]   size Data size in bytes
 * @return      Status of SpiWrite function
 */
static halStatus_t sdSendBytes(uint8_t *data, uint32_t size)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;
    uint32_t i = 0u;

    // Function Core
    while ((return_value == THAL_SUCCESSFUL) && (i < size))
    {
        return_value = SpiWrite(&spi_sdcard_inst, &data[i], 1u);
        i++;
    }

    return return_value;
}

/**
 * @fn          sdReceiveBytes(uint8_t *data, uint32_t size)
 * @brief       Receive bytes to SD card
 * @param[out]  data Data received from SD card
 * @param[in]   size Data size in bytes
 * @return      Status of SpiRead function
 */
static halStatus_t sdReceiveBytes(uint8_t *data, uint32_t size)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;
    uint8_t fill_char = SPI_FILL_CHAR;
    uint32_t i = 0u;

    // Function Core
    while ((return_value == THAL_SUCCESSFUL) && (i < size))
    {
        return_value = SpiRead(&spi_sdcard_inst, &data[i], &fill_char, 1u);
        i++;
    }

    return return_value;
}
