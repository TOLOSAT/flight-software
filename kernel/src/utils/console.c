/**
 * @file    console.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Console functions
 * @date    23/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "utils/console.h"
#include "fs/fs.h"
#include "conf/fs_conf.h"
#include "drv/peripherals.h"
#include "conf/peripherals_conf.h"
#include "core/time.h"
#include "core/mutex.h" // TO DO : remove
#include "conf/mutex_conf.h" // TO DO : remove

#if defined(CONFIG_CONSOLE_FILE) && defined(CONFIG_FS_NONE)
#error "Incompatible choice between CONFIG_FS_NONE and CONFIG_CONSOLE_FILE"
#endif

/***************************** Macros Definitions ****************************/

#if !defined(CONFIG_CONSOLE_NONE)

#if defined(CONFIG_CONSOLE_CIRCULAR_BUFFER)
#define CIRCULAR_BUFFER_SIZE (1024u) /**< Size of the circular buffer */
#endif

#if defined(CONFIG_CONSOLE_FILE)
#define CONSOLE_FILE_MAX_SIZE (512u * 1024u) /**< Maximum size of the console file */
#endif

#define INT_BUFFER_SIZE 12u /**< Buffer size for integer (absolute max value is 2147483648 which is 10 char + 1 sign char + we add 1 char of margin) */

#endif

/*************************** Functions Declarations **************************/

#if !defined(CONFIG_CONSOLE_NONE)
void CheckConsoleSize(void);
static void ConsolePrintChar(char c);
static void ConsolePrintHeader(void);
static void ConsoleSync(void);
#endif

/*************************** Variables Definitions ***************************/

#if defined(CONFIG_CONSOLE_CIRCULAR_BUFFER)
/**
 * @var     g_circular_buffer
 * @brief   Circular buffer for console printing
 */
uint8_t IN_KERNEL_DATA_SECTION g_circular_buffer[CIRCULAR_BUFFER_SIZE] __attribute__((aligned(32))) = {0};
#endif

/*************************** Functions Definitions ***************************/

/**
 * @fn          ConsolePrint(const char *msg)
 * @brief       Print message in console
 * @param[in]   msg Message we want to print
 * @return      nothing
 */
void IN_KERNEL_TEXT_SECTION ConsolePrint(const char *msg)
{
#if !defined(CONFIG_CONSOLE_NONE)
    // First Acquire Mutex
    while (AcquireMutex(CONSOLE_MUTEX) != KERNEL_SUCCESSFUL)
    {
        // Yield the task until the mutex become available
        taskYIELD();
    }

    // Then Check the console size
    CheckConsoleSize();

    // Variables Initialisation
    static uint32_t IN_KERNEL_DATA_SECTION line_index = 0u;
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
#endif /* CONFIG_CONSOLE_NONE */
}

/**
 * @fn          ConsolePrintNumber(signed int number)
 * @brief       Function used to print an signed integer
 * @param[in]   number Number that will be printed
 * @return      nothing
 */
void IN_KERNEL_TEXT_SECTION ConsolePrintNumber(signed int number)
{
#if !defined(CONFIG_CONSOLE_NONE)
    // First Acquire Mutex
    while (AcquireMutex(CONSOLE_MUTEX) != KERNEL_SUCCESSFUL)
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
#endif /* CONFIG_CONSOLE_NONE */
}

/**
 * @fn          ConsolePrintHex(unsigned int hex)
 * @brief       Function used to print an hexadecimal number
 * @param[in]   hex Number that will be printed
 * @return      nothing
 */
void IN_KERNEL_TEXT_SECTION ConsolePrintHex(unsigned int hex)
{
#if !defined(CONFIG_CONSOLE_NONE)
    // First Acquire Mutex
    while (AcquireMutex(CONSOLE_MUTEX) != KERNEL_SUCCESSFUL)
    {
        // Yield the task until the mutex become available
        taskYIELD();
    }

    // Print hex start
    ConsolePrintChar('0');
    ConsolePrintChar('x');

    // Print each hexadecimal digit
    for (uint32_t i = 1u; i <= (2u * sizeof(unsigned int)); i++)
    {
        // Compute position of the 4 bits that will be printed
        uint32_t shift = 4u * ((2u * sizeof(unsigned int)) - i);

        // Extract the current hex digit by shifting and masking
        uint8_t hex_digit = (uint8_t)((hex >> shift) & 0x000000000000000Fllu);

        // Convert to character and print
        if (hex_digit < 10u)
        {
            ConsolePrintChar('0' + hex_digit);
        }
        else
        {
            ConsolePrintChar('a' + (hex_digit - 10u));
        }
    }

    // Release Mutex Anyway
    (void)ReleaseMutex(CONSOLE_MUTEX);
#else
    (void)(hex);
#endif /* CONFIG_CONSOLE_NONE */
}

/**
 * @fn          ConsolePrintFloat(float number, int precision)
 * @brief       Function used to print a floating point number with specified precision
 * @param[in]   number    Number that will be printed
 * @param[in]   precision Number of digits after the decimal point
 * @return      nothing
 */
void IN_KERNEL_TEXT_SECTION ConsolePrintFloat(float number, int precision)
{
#if !defined(CONFIG_CONSOLE_NONE)
    // First Acquire Mutex
    while (AcquireMutex(CONSOLE_MUTEX) != KERNEL_SUCCESSFUL)
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
#endif /* CONFIG_CONSOLE_NONE */
}

#if !defined(CONFIG_CONSOLE_NONE)
/**
 * @fn          ConsolePrintHeader
 * @brief       Function that prints the header of each line
 * @return      nothing
 *
 * Currently the header is the CUC time
 */
static void IN_KERNEL_TEXT_SECTION ConsolePrintHeader(void)
{
    // Variable Initialisation
    time_t time = 0u;

    // First get time
    (void)GetTime(&time);

    // Print header start
    ConsolePrintChar('[');

    // Print each hexadecimal digit
    for (uint32_t i = 1u; i <= (2u * sizeof(time_t)); i++)
    {
        // Compute position of the 4 bits that will be printed
        uint32_t shift = 4u * ((2u * sizeof(time_t)) - i);

        // Extract the current hex digit by shifting and masking
        uint8_t hex_digit = (uint8_t)((time >> shift) & 0x000000000000000Fllu);

        // Convert to character and print
        if (hex_digit < 10u)
        {
            ConsolePrintChar('0' + hex_digit);
        }
        else
        {
            ConsolePrintChar('a' + (hex_digit - 10u));
        }
    }

    // Print header end
    ConsolePrintChar(']');
    ConsolePrintChar(':');
    ConsolePrintChar(' ');
}

/**
 * @fn          CheckConsoleSize
 * @brief       Check the console file size update the file if it reaches the maximum size
 * @return      nothing
 *
 * If reach the maximum size, the content is saved in
 * the console_old.log and a new console.log is opened.
 */
void IN_KERNEL_TEXT_SECTION CheckConsoleSize(void)
{
#if defined(CONFIG_CONSOLE_FILE)
    // First check the size of the console
    uint32_t console_size = f_size(g_file_desc_table[CONSOLE_FILE].temp_file);
    if (console_size > CONSOLE_FILE_MAX_SIZE)
    {
        fileNo_t old_console_no = CONSOLE_OLD_FILE;
        (void)FsIoctl(CONSOLE_FILE, FS_IOCTL_TRANSFER_DATA, &old_console_no, sizeof(fileNo_t));
    }
#endif
}

/**
 * @fn          ConsolePrintChar(char c)
 * @brief       Function used to print a character
 * @param[in]   c Character that will be printed
 * @return      nothing
 */
static void IN_KERNEL_TEXT_SECTION ConsolePrintChar(char c)
{
#if defined(CONFIG_CONSOLE_UART)
    // Function Core
    (void)PeripheralWrite(UART_PRINT, (data_t)&c, sizeof(char), 0u);
#elif defined(CONFIG_CONSOLE_FILE)
    // Variable declaration
    length_t console_size = 0u;

    // Function Core
    (void)FsIoctl(CONSOLE_FILE, FS_IOCTL_GET_SIZE, &console_size, sizeof(length_t));
    (void)FsWrite(CONSOLE_FILE, console_size, (data_t)&c, sizeof(char));
#elif defined(CONFIG_CONSOLE_CIRCULAR_BUFFER)
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
#error Please #define CONFIG_CONSOLE_NONE, CONFIG_CONSOLE_UART, CONFIG_CONSOLE_FILE or CONFIG_CONSOLE_CIRCULAR_BUFFER
#endif
}

/**
 * @fn          ConsoleSync(void)
 * @brief       Allow to flush data onto the file system if CONFIG_CONSOLE_FILE used
 * @return      nothing
 */
static void IN_KERNEL_TEXT_SECTION ConsoleSync(void)
{ 
#if defined(CONFIG_CONSOLE_FILE)
    (void)FsIoctl(CONSOLE_FILE, FS_IOCTL_SYNC, NULL, 0u);
#endif
}
#endif /* CONFIG_CONSOLE_NONE */