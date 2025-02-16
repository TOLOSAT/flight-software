/**
 * @file    cmsdk_uart.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for UART CMSDK functions
 * @date    09/06/2024
 *
 * Largely inspired by the Zephyr driver and STM32 HAL style.
 */

/******************************* Include Files *******************************/

#include "cmsdk_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static HAL_StatusTypeDef cmsdk_UartTxChar(UART_HandleTypeDef *uart, unsigned char c);
static HAL_StatusTypeDef cmsdk_UartRxChar(UART_HandleTypeDef *uart, unsigned char *c);

/*************************** Variables Definitions ***************************/

extern uint32_t SystemCoreClock;

/*************************** Functions Definitions ***************************/

/**
 * @brief Initialize UART channel
 */
HAL_StatusTypeDef cmsdk_UartInit(UART_HandleTypeDef *uart)
{
    // Set baud rate
    if (uart->baud_rate != 0u)
    {
        uart->instance->BAUDDIV = SystemCoreClock / uart->baud_rate;
    }

    // Enable receiver and transmitter
    uart->instance->CTRL = CMSDK_UART_CTRL_TXEN_Msk | CMSDK_UART_CTRL_RXEN_Msk;

    // Setup the states to ready
    uart->gstate = HAL_UART_STATE_READY;
    uart->rxstate = HAL_UART_STATE_READY;

    return HAL_OK;
}

/**
 * @brief Send a message through UART in polling mode
 */
HAL_StatusTypeDef cmsdk_UartTx(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length, uint32_t timeout)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t tickstart       = 0u;
    uint32_t i               = 0u;

    // First check if the uart is not used
    if (uart->gstate == HAL_UART_STATE_READY)
    {
        // Lock
        uart->gstate = HAL_UART_STATE_BUSY_TX;

        // Get the start tick for timeout purposes
        tickstart = cmsdk_HalGetTick();

        // Transmit
        while ((status != HAL_ERROR) && (i < length) && (cmsdk_HalGetTick() < (tickstart + timeout)))
        {
            status = cmsdk_UartTxChar(uart, msg[i]);
            i++;
        }

        // Check Timeout
        if (cmsdk_HalGetTick() > (tickstart + timeout))
        {
            status = HAL_TIMEOUT;
        }

        // Unlock
        uart->gstate = HAL_UART_STATE_READY;
    }
    else
    {
        status = HAL_BUSY;
    }

    return status;
}

/**
 * @brief Send a message through UART in interruption mode
 */
HAL_StatusTypeDef cmsdk_UartTx_IT(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length)
{
    HAL_StatusTypeDef status = HAL_OK;

    // First check if the uart is not used
    if (uart->gstate == HAL_UART_STATE_READY)
    {
        // TO DO
    }
    else
    {
        status = HAL_BUSY;
    }

    return status;
}

/**
 * @brief Receive a message through uart
 */
HAL_StatusTypeDef cmsdk_UartRx(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length, uint32_t timeout)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t tickstart       = 0u;
    uint32_t i               = 0u;

    // First check if the uart is not used
    if (uart->rxstate == HAL_UART_STATE_READY)
    {
        // Lock
        uart->rxstate = HAL_UART_STATE_BUSY_RX;

        // Get the start tick for timeout purposes
        tickstart = cmsdk_HalGetTick();

        // Receive
        while ((status != HAL_ERROR) && (i < length) && (cmsdk_HalGetTick() < (tickstart + timeout)))
        {
            status = cmsdk_UartRxChar(uart, &msg[i]);
            if (status == HAL_OK)
            {
                i++;
            }
        }

        // Check Timeout
        if (cmsdk_HalGetTick() >= (tickstart + timeout))
        {
            status = HAL_TIMEOUT;
        }

        // Unlock
        uart->rxstate = HAL_UART_STATE_READY;
    }
    else
    {
        status = HAL_BUSY;
    }

    return status;
}

/**
 * @brief Send a message through UART in interruption mode
 */
HAL_StatusTypeDef cmsdk_UartRx_IT(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length)
{
    HAL_StatusTypeDef status = HAL_OK;

    // First check if the uart is not used
    if (uart->rxstate == HAL_UART_STATE_READY)
    {
        // TO DO
    }
    else
    {
        status = HAL_BUSY;
    }

    return status;
}

/**
 * @brief Poll the device for transmitting a char.
 */
static HAL_StatusTypeDef cmsdk_UartTxChar(UART_HandleTypeDef *uart, unsigned char c)
{
    /* Wait for transmitter to be ready */
    while (uart->instance->STATE & CMSDK_UART_STATE_TXBF_Msk)
    {
        __NOP();
    }

    /* Send a character */
    uart->instance->DATA = (uint32_t)c;

    return HAL_OK;
}

/**
 * @brief Poll the device for receiving a char.
 */
static HAL_StatusTypeDef cmsdk_UartRxChar(UART_HandleTypeDef *uart, unsigned char *c)
{
    // If the receiver is not ready returns HAL_BUSY
    if (!(uart->instance->STATE & CMSDK_UART_STATE_RXBF_Msk))
    {
        return HAL_BUSY;
    }

    // Got a character
    *c = (unsigned char)uart->instance->DATA;

    return HAL_OK;
}

/**
 * @brief De-Initialize UART channel
 */
HAL_StatusTypeDef cmsdk_UartDeInit(UART_HandleTypeDef *uart)
{
    // Setup the states to reset
    uart->gstate = HAL_UART_STATE_RESET;
    uart->rxstate = HAL_UART_STATE_RESET;

    // Disable receiver and transmitter
    uart->instance->CTRL = 0x00u;

    return HAL_OK;
}

/**
 * @brief UART Receiver Interrupt Handler
 */
void cmsdk_UartRxIRQHandler(UART_HandleTypeDef *huart)
{
    // TO DO
}

/**
 * @brief UART Transmitter Interrupt Handler
 */
void cmsdk_UartTxIRQHandler(UART_HandleTypeDef *huart)
{
    // TO DO
}