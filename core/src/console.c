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
#include "time_management.h"

#if defined(CONSOLE_UART)
#include "generic_hal.h"
#endif

#if defined(CONSOLE_FS)
#include "tolosat_fs.h"
#include "conf/fs_conf.h"
#endif

/***************************** Macros Definitions ****************************/

#if !defined(CONSOLE_NONE)

#if defined(CONSOLE_CIRCULAR_BUFFER)
#define CIRCULAR_BUFFER_SIZE (1024u) /**< Size of the circular buffer */
#endif

#if defined(CONSOLE_FS)
#define CONSOLE_TEMP_FILE g_files_conf[SD0][CONSOLE_FILE].temp_file /**< Name of console file */
#endif

#define INT_BUFFER_SIZE 12u /**< Buffer size for integer (absolute max value is 2147483648 which is 10 char + 1 sign char + we add 1 char of margin) */
#define HEX_BUFFER_SIZE 9u  /**< Buffer size for hexadecimal (max value is 0xFFFFFFFF which is 8 char + we add 1 char of margin) */

#endif

/*************************** Functions Declarations **************************/

#if !defined(CONSOLE_NONE)
static void ConsolePrintChar(char c);
static void ConsolePrintHeader(void);
static void ConsoleSync(void);
#endif

/*************************** Variables Definitions ***************************/

#if defined(CONSOLE_UART)
extern uartInst_t uart_print_inst;
#endif

#if defined(CONSOLE_CIRCULAR_BUFFER)
/**
 * @var     g_circular_buffer
 * @brief   Circular buffer for console printing
 */
uint8_t g_circular_buffer[CIRCULAR_BUFFER_SIZE] = {0};
#endif

/*************************** Functions Definitions ***************************/

/**
 * @fn          ConsolePrint(const char *msg)
 * @brief       Print message in console
 * @param[in]   msg Message we want to print
 * @return      nothing
 */
void ConsolePrint(const char *msg)
{
#if !defined(CONSOLE_NONE)
    // Variables Initialisation
    static uint32_t line_index = 0u;
    uint32_t i = 0u;

    // Function Core
    while (msg[i] != '\0')
    {
        // If first char of the line print the header first
        if (line_index == 0u)
        {
            ConsolePrintHeader();
        }

        // Print char
        ConsolePrintChar(msg[i]);

        // If the char was '\n' then we sync console and update line_index
        if (msg[i] == '\n')
        {
            ConsoleSync();
            line_index = 0u;
        }
        else
        {
            line_index++;
        }

        // Increment index of the message
        i++;
    }
#else
    (void)(msg);
#endif /* CONSOLE_NONE */
}

/**
 * @fn          ConsolePrintNumber(signed int number)
 * @brief       Function used to print an signed integer
 * @param[in]   number Number that will be printed
 * @return      nothing
 */
void ConsolePrintNumber(signed int number)
{
#if !defined(CONSOLE_NONE)
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
#else
    (void)(number);
#endif /* CONSOLE_NONE */
}

/**
 * @fn          ConsolePrintHex(unsigned int hex)
 * @brief       Function used to print an hexadecimal number
 * @param[in]   hex Number that will be printed
 * @return      nothing
 */
void ConsolePrintHex(unsigned int hex)
{
#if !defined(CONSOLE_NONE)
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
#else
    (void)(hex);
#endif /* CONSOLE_NONE */
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
#if !defined(CONSOLE_NONE)
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
#else
    (void)(number);
    (void)(precision);
#endif /* CONSOLE_NONE */
}

#if !defined(CONSOLE_NONE)
/**
 * @fn          ConsolePrintHeader
 * @brief       Function that prints the header of each line
 * @return      nothing
 * 
 * Currently the header is the CUC time
 */
static void ConsolePrintHeader(void)
{
    // Variable Initialisation
    char cuc_time_str[CUC_TIME_STR_SIZE] = {0}; // cppcheck-suppress misra-c2012-18.8; False positive because CUC_TIME_STR_SIZE is a constant

    // Function Core
    // First Get CUC time
    (void)GetStrCUCTime(cuc_time_str);
    
    // Then print header
    ConsolePrintChar('[');
    for (uint32_t i = 0u; i < CUC_TIME_STR_SIZE; i++)
    {
        ConsolePrintChar(cuc_time_str[i]);
    }
    ConsolePrintChar(']');
    ConsolePrintChar(':');
    ConsolePrintChar(' ');
}

/**
 * @fn          ConsolePrintChar(char c)
 * @brief       Function used to print a character
 * @param[in]   c Character that will be printed
 * @return      nothing
 */
static void ConsolePrintChar(char c)
{
#if defined(CONSOLE_UART)
    // Function Core
    (void)UartWrite(&uart_print_inst, (uartMsg_t *)&c, sizeof(char));
#elif defined(CONSOLE_FS)
    // Variable declaration
    static uint32_t last_position_in_file = 0u;
    uint32_t bytes_written = 0u;

    // Function Core
    f_lseek(CONSOLE_TEMP_FILE, last_position_in_file);
    f_write(CONSOLE_TEMP_FILE, &c, sizeof(char), (UINT *)&bytes_written);
    last_position_in_file++;
#elif defined(CONSOLE_CIRCULAR_BUFFER)
    // Variable declaration
    static uint32_t circular_buffer_index = 0u;

    // Function Core
    if (circular_buffer_index == CIRCULAR_BUFFER_SIZE)
    {
        circular_buffer_index = 0u;
    }
    g_circular_buffer[circular_buffer_index] = c;
    circular_buffer_index++;
#else
#error Please #define CONSOLE_NONE, CONSOLE_UART, CONSOLE_FS or CONSOLE_CIRCULAR_BUFFER
#endif
}

/**
 * @fn          ConsoleSync(void)
 * @brief       Allow to flush data onto the file system if CONSOLE_FS used 
 * @return      nothing
 */
static void ConsoleSync(void)
{
#if defined(CONSOLE_FS)
    f_sync(CONSOLE_TEMP_FILE);
#endif
}
#endif /* CONSOLE_NONE */