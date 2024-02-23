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

/*************************** Functions Declarations **************************/

static void uart_send(char c);
static void print_string(const char *str);
static void print_number(int number);
static void print_unsigned(unsigned int number);

/*************************** Variables Definitions ***************************/

extern uartInst_t uart_print_inst;

/*************************** Functions Definitions ***************************/

void ConsoleOpen(void)
{

}

void ConsolePrint(const char *format, ...)
{
    // Variable Initialisation
    va_list args;
    va_start(args, format);

    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
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
                uart_send(c);
                break;
            }
            default:
                uart_send(*format);
                break;
            }
        }
        else
        {
            uart_send(*format);
        }
        format++;
    }

    va_end(args);
}

static void uart_send(char c)
{
    uartMsg_t msg = c;
    (void)UartWrite(&uart_print_inst, &msg, sizeof(char));
}

static void print_string(const char *str)
{
    while (*str)
    {
        uart_send(*str++);
    }
}

static void print_number(int number)
{
    if (number < 0)
    {
        uart_send('-');
        number = -number;
    }
    if (number / 10)
    {
        print_number(number / 10);
    }
    uart_send((number % 10) + '0');
}

static void print_unsigned(unsigned int number)
{
    if (number / 10)
    {
        print_unsigned(number / 10);
    }
    uart_send((number % 10) + '0');
}
