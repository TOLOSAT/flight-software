/**
 * @file    console.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Console functions
 * @date    23/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "core.h"
#include "time.h"

#if defined(CONSOLE_MODE_UART)
#include "generic_hal.h"
#endif

#if defined(CONSOLE_MODE_FILE)
#include "fs.h"
#include "conf/fs_conf.h"
#endif

#if defined(CONSOLE_MODE_FILE) && defined(FS_MODE_NONE)
#error "Incompatible choice between FS_MODE_NONE and CONSOLE_MODE_FILE"
#endif

/***************************** Macros Definitions ****************************/

#if !defined(CONSOLE_MODE_NONE)

#if defined(CONSOLE_MODE_CIRCULAR_BUFFER)
#define CIRCULAR_BUFFER_SIZE (1024u) /**< Size of the circular buffer */
#endif

#if defined(CONSOLE_MODE_FILE)
#define CONSOLE_FILE_MAX_SIZE (512u * 1024u) /**< Maximum size of the console file */
#endif

#define INT_BUFFER_SIZE 12u /**< Buffer size for integer (absolute max value is 2147483648 which is 10 char + 1 sign char + we add 1 char of margin) */

#endif

/*************************** Functions Declarations **************************/

#if !defined(CONSOLE_MODE_NONE)
void CheckConsoleSize(void);
static void ConsolePrintChar(char c);
static void ConsolePrintHeader(void);
static void ConsoleSync(void);
#endif

/*************************** Variables Definitions ***************************/

#if defined(CONSOLE_MODE_UART)
extern uartInst_t uart_print_inst;
#endif

#if defined(CONSOLE_MODE_CIRCULAR_BUFFER)
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
void IN_CORE_TEXT_SECTION ConsolePrint(const char *msg)
{
#if !defined(CONSOLE_MODE_NONE)
    // First Acquire Mutex
    while (AcquireMutex(CONSOLE_MUTEX) != CORE_SUCCESSFUL)
    {
        // Yield the task until the mutex become available
        taskYIELD();
    }

    // Then Check the console size
    CheckConsoleSize();

    // Variables Initialisation
    static uint32_t IN_CORE_DATA_SECTION line_index = 0u;
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

    // Release Mutex Anyway
    (void)ReleaseMutex(CONSOLE_MUTEX);
#else
    (void)(msg);
#endif /* CONSOLE_MODE_NONE */
}

/**
 * @fn          ConsolePrintNumber(signed int number)
 * @brief       Function used to print an signed integer
 * @param[in]   number Number that will be printed
 * @return      nothing
 */
void IN_CORE_TEXT_SECTION ConsolePrintNumber(signed int number)
{
#if !defined(CONSOLE_MODE_NONE)
    // First Acquire Mutex
    while (AcquireMutex(CONSOLE_MUTEX) != CORE_SUCCESSFUL)
    {
        // Yield the task until the mutex become available
        taskYIELD();
    }

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

    // Release Mutex Anyway
    (void)ReleaseMutex(CONSOLE_MUTEX);
#else
    (void)(number);
#endif /* CONSOLE_MODE_NONE */
}

/**
 * @fn          ConsolePrintHex(unsigned int hex)
 * @brief       Function used to print an hexadecimal number
 * @param[in]   hex Number that will be printed
 * @return      nothing
 */
void IN_CORE_TEXT_SECTION ConsolePrintHex(unsigned int hex)
{
#if !defined(CONSOLE_MODE_NONE)
    // First Acquire Mutex
    while (AcquireMutex(CONSOLE_MUTEX) != CORE_SUCCESSFUL)
    {
        // Yield the task until the mutex become available
        taskYIELD();
    }

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
        char buffer[2u*sizeof(int)];
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

    // Release Mutex Anyway
    (void)ReleaseMutex(CONSOLE_MUTEX);
#else
    (void)(hex);
#endif /* CONSOLE_MODE_NONE */
}

/**
 * @fn          ConsolePrintFloat(float number, int precision)
 * @brief       Function used to print a floating point number with specified precision
 * @param[in]   number    Number that will be printed
 * @param[in]   precision Number of digits after the decimal point
 * @return      nothing
 */
void IN_CORE_TEXT_SECTION ConsolePrintFloat(float number, int precision)
{
#if !defined(CONSOLE_MODE_NONE)
    // First Acquire Mutex
    while (AcquireMutex(CONSOLE_MUTEX) != CORE_SUCCESSFUL)
    {
        // Yield the task until the mutex become available
        taskYIELD();
    }

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

    // Release Mutex Anyway
    (void)ReleaseMutex(CONSOLE_MUTEX);
#else
    (void)(number);
    (void)(precision);
#endif /* CONSOLE_MODE_NONE */
}

#if !defined(CONSOLE_MODE_NONE)
/**
 * @fn          CheckConsoleSize
 * @brief       Check the console file size update the file if it reaches the maximum size
 * @return      nothing
 *
 * If reach the maximum size, the content is saved in
 * the console_old.log and a new console.log is opened.
 */
void IN_CORE_TEXT_SECTION CheckConsoleSize(void)
{
#if defined(CONSOLE_MODE_FILE)
    // First check the size of the console
    uint32_t console_size = f_size(g_file_desc_table[CONSOLE_FILE].temp_file);
    if (console_size > CONSOLE_FILE_MAX_SIZE)
    {
        fsFileno_t old_console_no = CONSOLE_OLD_FILE;
        (void)FsIoctl(CONSOLE_FILE, FS_IOCTL_TRANSFER_DATA, &old_console_no, sizeof(fsFileno_t));
    }
#endif
}

/**
 * @fn          ConsolePrintHeader
 * @brief       Function that prints the header of each line
 * @return      nothing
 *
 * Currently the header is the CUC time
 */
static void IN_CORE_TEXT_SECTION ConsolePrintHeader(void)
{
    // Variable Initialisation
    time_t time = 0u;

    // Function Core
    // First Get CUC time
    (void)GetTime(&time);

    // Then print header
    ConsolePrintChar('[');

    // Init string buffer
    char time_char_buff[2u*sizeof(time_t)];
    int i = 0;

    // Convert the time to a string in reverse order
    while (time > 0u)
    {
        long int temp = time % 16u;
        if (temp < 10)
        {
            time_char_buff[i] = temp + '0';
            i++;
        }
        else
        {
            time_char_buff[i] = (temp - 10) + 'a';
            i++;
        }
        time /= 16;
    }

    // Print the time in the correct order
    while (i > 0)
    {
        i--;
        ConsolePrintChar(time_char_buff[i]);
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
static void IN_CORE_TEXT_SECTION ConsolePrintChar(char c)
{
#if defined(CONSOLE_MODE_UART)
    // Function Core
    (void)UartWrite(&uart_print_inst, (uartMsg_t *)&c, sizeof(char));
#elif defined(CONSOLE_MODE_FILE)
    // Variable declaration
    fsSize_t console_size = 0u;

    // Function Core
    (void)FsIoctl(CONSOLE_FILE, FS_IOCTL_GET_SIZE, &console_size, sizeof(fsSize_t));
    (void)FsWrite(CONSOLE_FILE, console_size, (fsData_t *)&c, sizeof(char));
#elif defined(CONSOLE_MODE_CIRCULAR_BUFFER)
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
#error Please #define CONSOLE_MODE_NONE, CONSOLE_MODE_UART, CONSOLE_MODE_FILE or CONSOLE_MODE_CIRCULAR_BUFFER
#endif
}

/**
 * @fn          ConsoleSync(void)
 * @brief       Allow to flush data onto the file system if CONSOLE_MODE_FILE used
 * @return      nothing
 */
static void IN_CORE_TEXT_SECTION ConsoleSync(void)
{ 
#if defined(CONSOLE_MODE_FILE)
    (void)FsIoctl(CONSOLE_FILE, FS_IOCTL_SYNC, NULL, 0u);
#endif
}
#endif /* CONSOLE_MODE_NONE */