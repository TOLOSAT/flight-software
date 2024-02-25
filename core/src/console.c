/**
 * @file    console.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Console functions
 * @date    23/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core_basics.h"
#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

#define INT_BUFFER_SIZE             12u /**< Buffer size for integer (absolute max value is 2147483648 which is 10 char + 1 sign char + we add 1 char of margin) */
#define HEX_BUFFER_SIZE             9u  /**< Buffer size for hexadecimal (max value is 0xFFFFFFFF which is 8 char + we add 1 char of margin) */

/*************************** Functions Declarations **************************/

static void ConsolePrintChar(char c);

/*************************** Variables Definitions ***************************/

extern uartInst_t uart_print_inst;

/*************************** Functions Definitions ***************************/

/**
 * @fn          ConsolePrint(const char *msg)
 * @brief       Print message in console
 * @param[in]   msg Message we want to print
 * @return      nothing
 */
void ConsolePrint(const char *msg)
{
    // Variables Initialisation
    int i = 0;

    // Function Core
    while (msg[i] != '\0')
    {
        ConsolePrintChar(msg[i]);
        i++;
    }
}

/**
 * @fn          ConsolePrintNumber(signed int number)
 * @brief       Function used to print an signed integer
 * @param[in]   number Number that will be printed
 * @return      nothing
 */
void ConsolePrintNumber(signed int number)
{
    // Variable Initialisation
    int remaining_number = number;

    // Function Core
    if (remaining_number == 0)
    {
        ConsolePrintChar('0');
    }
    else
    {
        // Init string buffer
        char buffer[INT_BUFFER_SIZE];
        int i = 0;

        // Handle negative numbers
        if (remaining_number < 0)
        {
            ConsolePrintChar('-');
            remaining_number = -remaining_number;
        }

        // Convert the number to a string in reverse order
        while (remaining_number > 0)
        {
            buffer[i] = (remaining_number % 10) + '0';
            remaining_number /= 10;
            i++;
        }

        // Print the number in the correct order
        while (i > 0)
        {
            i--;
            ConsolePrintChar(buffer[i]);
        }
    }
}

/**
 * @fn          ConsolePrintHex(unsigned int hex)
 * @brief       Function used to print an hexadecimal number
 * @param[in]   number Number that will be printed
 * @return      nothing
 */
void ConsolePrintHex(unsigned int hex)
{
    // Variable Initialisation
    unsigned int remaining_number = hex;

    // Function Core
    if (remaining_number == 0u)
    {
        ConsolePrintChar('0');
    }
    else
    {
        // Init string buffer
        char buffer[HEX_BUFFER_SIZE];
        int i = 0;

        // Convert the number to a string in reverse order
        while (remaining_number > 0u)
        {
            int temp = remaining_number % 16u;
            if (temp < 10)
            {
                buffer[i] = temp + '0';
                i++;
            }
            else
            {
                buffer[i] = (temp - 10) + 'a';
                i++;
            }
            remaining_number /= 16;
        }

        // Print the number in the correct order
        while (i > 0)
        {
            i--;
            ConsolePrintChar(buffer[i]);
        }
    }
}

/**
 * @fn          ConsolePrintFloat(float number, int precision)
 * @brief       Function used to print a floating point number with specified precision
 * @param[in]   number    Number that will be printed
 * @param[in]   precision Number of digits after the decimal point
 * @return      nothing
 */
void ConsolePrintFloat(float number, int precision)
{
    // Variables initialisation
    int integerPart = 0;
    float fractionalPart = 0.0f;

    // Function core
    if (number < 0.0f)
    {
        // Number is negative
        ConsolePrintChar('-');
        integerPart = (int)(-number);
        fractionalPart = (-number) - (float)integerPart;
    }
    else
    {
        // Number is positive
        integerPart = (int)number;
        fractionalPart = number - (float)integerPart;
    }

    // Print the integer part
    ConsolePrintNumber(integerPart);

    // Print the decimal point
    ConsolePrintChar('.');

    // Print the fractional part
    for (int i = 0; i < precision; i++)
    {
        // Move the next digit to the integer part
        fractionalPart *= 10.0f;
        int digit = (int)fractionalPart;
        
        // Print the digit
        ConsolePrintChar('0' + digit);

        // Remove the printed digit from the fractional part
        fractionalPart -= (float)digit;
    }
}

/**
 * @fn          ConsolePrintChar(char c)
 * @brief       Function used to print a character
 * @param[in]   c Character that will be printed
 * @return      nothing
 */
static void ConsolePrintChar(char c)
{
    // Function Core
    (void)UartWrite(&uart_print_inst, (uartMsg_t *)&c, sizeof(char));
}