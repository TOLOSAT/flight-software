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

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          cmsdk_UartInit(UART_HandleTypeDef *uart)
 * @brief       Initialize UART channel
 * @param[in]   uart UART handle struct
 * @retval      #HAL_OK always
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
    uart->gstate  = HAL_UART_STATE_READY;
    uart->rxstate = HAL_UART_STATE_READY;

    return HAL_OK;
}

/**
 * @fn          cmsdk_UartTx(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length, uint32_t timeout)
 * @brief       Send a message through UART in polling mode
 * @param[in]   uart UART handle struct
 * @param[in]   msg UART pointer to data to transmit
 * @param[in]   length  UART length of data transmitted
 * @param[in]   timeout UART transmission timeout value
 * @retval      #HAL_ERROR if UART encountered an error
 * @retval      #HAL_BUSY if UART is already busy transmitting
 * @retval      #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_UartTx(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length, uint32_t timeout)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t tickstart       = 0u;

    // First check if the uart is not used
    if (uart->gstate == HAL_UART_STATE_READY)
    {
        // Set UART TX state
        uart->gstate = HAL_UART_STATE_BUSY_TX;

        // Setup transmission data
        uart->p_tx_data     = msg;
        uart->tx_data_size  = length;
        uart->tx_data_count = 0u;

        // Get the start tick for timeout purposes
        tickstart = cmsdk_HalGetTick();

        // Transmit
        while ((uart->gstate != HAL_UART_STATE_ERROR) && (uart->tx_data_count < uart->tx_data_size) && (cmsdk_HalGetTick() < (tickstart + timeout)))
        {
            // Wait for transmitter buffer to be empty for sending new byte
            while (uart->instance->STATE & CMSDK_UART_STATE_TXBF_Msk)
            {
                __NOP();
            }

            // Send a byte
            uart->instance->DATA = (uint32_t)uart->p_tx_data[uart->tx_data_count];
            uart->tx_data_count++;
        }

        // Check Error
        if (uart->gstate == HAL_UART_STATE_ERROR)
        {
            status = HAL_ERROR;
        }
        else
        {
            // Check Timeout
            if (cmsdk_HalGetTick() > (tickstart + timeout))
            {
                status = HAL_TIMEOUT;
            }

            // Set state to ready
            uart->gstate = HAL_UART_STATE_READY;
        }
    }
    else
    {
        status = HAL_BUSY;
    }

    return status;
}

/**
 * @fn          cmsdk_UartTx_IT(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length)
 * @brief       Send a message through UART in polling mode
 * @param[in]   uart UART handle struct
 * @param[in]   msg UART pointer to data to transmit
 * @param[in]   length  UART length of data transmitted
 * @retval      #HAL_ERROR if UART encountered an error
 * @retval      #HAL_BUSY if UART is already busy transmitting
 * @retval      #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_UartTx_IT(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length)
{
    HAL_StatusTypeDef status = HAL_OK;

    // First check if the uart is not used
    if (uart->gstate == HAL_UART_STATE_READY)
    {
        // Setup transmission data
        uart->p_tx_data     = msg;
        uart->tx_data_size  = length;
        uart->tx_data_count = 0u;

        // Change UART status
        uart->gstate = HAL_UART_STATE_BUSY_TX;

        // Start Interrupt
        uart->instance->CTRL     |= CMSDK_UART_CTRL_TXIRQEN_Msk;
        uart->instance->INTCLEAR  = CMSDK_UART_CTRL_TXIRQ_Msk;

        // Send first byte to trigger the interrupt mecanism
        uart->instance->DATA = (uint32_t)uart->p_tx_data[uart->tx_data_count];
        uart->tx_data_count++;
    }
    else
    {
        status = HAL_BUSY;
    }

    return status;
}

/**
 * @fn          cmsdk_UartRx(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length, uint32_t timeout)
 * @brief       Receive a message through UART in polling mode
 * @param[in]   uart UART handle struct
 * @param[in]   msg UART pointer to data to receive
 * @param[in]   length  UART maximum length for data to receive
 * @param[in]   timeout UART reception timeout value
 * @retval      #HAL_ERROR if UART encountered an error
 * @retval      #HAL_BUSY if UART is already busy receiving
 * @retval      #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_UartRx(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length, uint32_t timeout)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t tickstart       = 0u;

    // First check if the uart is not used
    if (uart->rxstate == HAL_UART_STATE_READY)
    {
        // Lock
        uart->rxstate = HAL_UART_STATE_BUSY_RX;

        // Setup reception data
        uart->p_rx_data     = msg;
        uart->rx_data_size  = length;
        uart->rx_data_count = 0u;

        // Get the start tick for timeout purposes
        tickstart = cmsdk_HalGetTick();

        // Receive
        while ((uart->gstate != HAL_UART_STATE_ERROR) && (uart->rx_data_count < uart->rx_data_size) && (cmsdk_HalGetTick() < (tickstart + timeout)))
        {
            // Got the new byte
            if (uart->instance->STATE & CMSDK_UART_STATE_RXBF_Msk)
            {
                uart->p_tx_data[uart->tx_data_count] = (uint8_t)uart->instance->DATA;
                uart->tx_data_count++;
            }
        }

        // Check error
        if (uart->gstate != HAL_UART_STATE_ERROR)
        {
            status = HAL_ERROR;
        }
        else
        {
            // Check Timeout
            if (cmsdk_HalGetTick() >= (tickstart + timeout))
            {
                status = HAL_TIMEOUT;
            }

            // Set state to ready
            uart->rxstate = HAL_UART_STATE_READY;
        }
    }
    else
    {
        status = HAL_BUSY;
    }

    return status;
}

/**
 * @fn          cmsdk_UartRx_IT(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length)
 * @brief       Receive a message through UART in interrupt mode
 * @param[in]   uart UART handle struct
 * @param[in]   msg UART pointer to data to receive
 * @param[in]   length  UART maximum length for data to receive
 * @retval      #HAL_ERROR if UART encountered an error
 * @retval      #HAL_BUSY if UART is already busy receiving
 * @retval      #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_UartRx_IT(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length)
{
    HAL_StatusTypeDef status = HAL_OK;

    // First check if the uart is not used
    if (uart->rxstate == HAL_UART_STATE_READY)
    {
        // Setup transmission data
        uart->p_rx_data     = msg;
        uart->rx_data_size  = length;
        uart->rx_data_count = 0u;

        // Change UART status
        uart->rxstate = HAL_UART_STATE_BUSY_RX;

        // Start Interrupt
        uart->instance->CTRL     |= CMSDK_UART_CTRL_RXIRQEN_Msk;
        uart->instance->INTCLEAR  = CMSDK_UART_CTRL_RXIRQ_Msk;
    }
    else
    {
        status = HAL_BUSY;
    }

    return status;
}

/**
 * @fn          cmsdk_UartDeInit(UART_HandleTypeDef *uart)
 * @brief       De-Initialize UART
 * @param[in]   uart UART handle struct
 * @retval      #HAL_OK always
 */
HAL_StatusTypeDef cmsdk_UartDeInit(UART_HandleTypeDef *uart)
{
    // Setup the states to reset
    uart->gstate  = HAL_UART_STATE_RESET;
    uart->rxstate = HAL_UART_STATE_RESET;

    // Disable receiver and transmitter
    uart->instance->CTRL = 0x00u;

    return HAL_OK;
}

/**
 * @brief UART Receiver Interrupt Handler
 */
void cmsdk_UartRxIRQHandler(UART_HandleTypeDef *uart)
{
    // Clear Interrupt
    uart->instance->INTCLEAR = CMSDK_UART_CTRL_RXIRQ_Msk;

    // Check if data need to be received
    if (uart->rx_data_count < uart->rx_data_size)
    {
        // Receive bytes
        while ((uart->instance->STATE & CMSDK_UART_STATE_RXBF_Msk) && (uart->rx_data_count < uart->rx_data_size))
        {
            uart->p_rx_data[uart->rx_data_count] = (uint8_t)uart->instance->DATA;
            uart->rx_data_count++;
        }

        // All bytes have been received
        uart->rxstate = HAL_UART_STATE_READY;
    }
    else
    {
        // All data has been received
        uart->rxstate = HAL_UART_STATE_READY;
    }
}

/**
 * @brief UART Transmitter Interrupt Handler
 */
void cmsdk_UartTxIRQHandler(UART_HandleTypeDef *uart)
{
    // Clear Interrupt
    uart->instance->INTCLEAR = CMSDK_UART_CTRL_TXIRQ_Msk;

    // Check if data need to be sent
    if (uart->tx_data_count < uart->tx_data_size)
    {
        if (!(uart->instance->STATE & CMSDK_UART_STATE_TXBF_Msk))
        {
            // Send a byte
            uart->instance->DATA = (uint32_t)uart->p_tx_data[uart->tx_data_count];
            uart->tx_data_count++;
        }
    }
    else
    {
        // All data has been sent
        uart->gstate = HAL_UART_STATE_READY;
    }
}