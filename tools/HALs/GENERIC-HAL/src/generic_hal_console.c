/**
 * @file    generic_hal_console.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC HAL Console functions
 * @date    23/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <stdarg.h>

#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

#define INT_BUFFER_SIZE             12u /**< Buffer size for integer (absolute max value is 2147483648 which is 10 char + 1 sign char + we add 1 char of margin) */
#define UNSIGNED_INT_BUFFER_SIZE    11u /**< Buffer size for unsigned integer (max value is 4294967295 which is 10 char + we add 1 char of margin) */
#define HEX_BUFFER_SIZE             9u  /**< Buffer size for hexadecimal (max value is 0xFFFFFFFF which is 8 char + we add 1 char of margin) */

/*************************** Functions Declarations **************************/

static void print_char(char c);
static void print_string(const char *str);
static void print_number(int number);
static void print_unsigned(unsigned int number);
static void print_hex(unsigned int number);

/*************************** Variables Definitions ***************************/

static uartInst_t *print_inst_pointer;

/*************************** Functions Definitions ***************************/

/**
 * @fn              ConsoleOpen(uartInst_t *uart_inst)
 * @brief           Initialise console by linking the uart instance used for printing
 * @param[in,out]   uart_inst uart instance used for print
 * @retval          #GEN_HAL_INVALID_PARAM if pointer is null
 * @retval          #GEN_HAL_SUCCESSFUL else
 */
halStatus_t ConsoleOpen(uartInst_t *uart_inst)
{
    // Variables Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (uart_inst != NULL)
    {
        print_inst_pointer = uart_inst;
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ConsolePrint(const char *format, ...)
 * @brief       Printf like function but lighter and compatible with embedded systems
 * @param[in]   format "Formatted" message we want to print
 * @param[in]   ... Other arguments (used when you utilise %d, %u)
 * @return      nothing
 * 
 * @warning     Only %s, %d, %i, %u, %c and %x are available
 */
void ConsolePrint(const char *format, ...)
{
    // Variables Initialisation
    int i = 0;
    va_list args;
    va_start(args, format);

    // Function Core
    while (format[i] != '\0')
    {
        if (format[i] == '%')
        {
            i++; // increment index because we want the next char
            switch (format[i])
            {
            case 's':
            {
                char *str = va_arg(args, char *);
                print_string(str);
                break;
            }
            case 'd':
            case 'i':
            {
                int num = va_arg(args, int);
                print_number(num);
                break;
            }
            case 'u':
            {
                unsigned int num = va_arg(args, unsigned int);
                print_unsigned(num);
                break;
            }
            case 'c':
            {
                char c = (char)va_arg(args, int); // char est promu en int lorsqu'il est passé via ...
                print_char(c);
                break;
            }
            case 'x':
            {
                unsigned int num = va_arg(args, unsigned int);
                print_hex(num);
                break;
            }
            default:
                print_char(format[i]);
                break;
            }
        }
        else
        {
            print_char(format[i]);
        }
        i++;
    }

    va_end(args);
}

/**
 * @fn          print_string(const char *str)
 * @brief       Function used to print strings
 * @param[in]   str Pointer to the string that will be printed
 */
static void print_string(const char *str)
{
    // Variable initialisation
    int i = 0;

    // Function Core
    while (str[i] != '\0')
    {
        print_char(str[i]);
        i++;
    }
}

/**
 * @fn          print_number(int number)
 * @brief       Function used to print an signed integer
 * @param[in]   number Number that will be printed
 */
static void print_number(int number)
{
    // Variable Initialisation
    int remaining_number = number;

    // Function Core
    if (remaining_number == 0)
    {
        print_char('0');
    }
    else
    {
        char buffer[INT_BUFFER_SIZE];
        int i = 0;
        if (remaining_number < 0)
        {
            print_char('-');
            remaining_number = -remaining_number;
        }

        while (remaining_number > 0)
        {
            buffer[i] = (remaining_number % 10) + '0';
            remaining_number /= 10;
            i++;
        }

        while (i > 0)
        {
            i--;
            print_char(buffer[i]);
        }
    }
}

/**
 * @fn          print_unsigned(unsigned int number)
 * @brief       Function used to print an unsigned integer
 * @param[in]   number Number that will be printed
 */
static void print_unsigned(unsigned int number)
{
    // Variable Initialisation
    unsigned int remaining_number = number;

    // Function Core
    if (remaining_number == 0u)
    {
        print_char('0');
    }
    else
    {
        char buffer[UNSIGNED_INT_BUFFER_SIZE]; // Assez grand pour un unsigned int
        int i = 0;
        while (remaining_number > 0u)
        {
            buffer[i] = (remaining_number % 10u) + '0';
            remaining_number /= 10;
            i++;
        }

        while (i > 0)
        {
            i--;
            print_char(buffer[i]);
        }
    }
}

/**
 * @fn          print_hex(unsigned int number)
 * @brief       Function used to print an hexadecimal number
 * @param[in]   number Number that will be printed
 */
static void print_hex(unsigned int number)
{
    // Variable Initialisation
    unsigned int remaining_number = number;

    // Function Core
    if (remaining_number == 0u)
    {
        print_char('0');
    }
    else
    {
        char buffer[HEX_BUFFER_SIZE];
        int i = 0;
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

        while (i > 0)
        {
            i--;
            print_char(buffer[i]);
        }
    }
}

/**
 * @fn          print_char(char c)
 * @brief       Function used to print a character
 * @param[in]   c Character that will be printed
 */
static void print_char(char c)
{
    // Function Core
    (void)UartWrite(print_inst_pointer, (uartMsg_t *)&c, sizeof(char));
}