/**
 * @file    console.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Console functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "utils/console.h"
#include "core/time.h"
#include "core/tasks.h"
#include "fs/fs.h"
#include "drv/peripherals.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

#if !defined(CONFIG_CONSOLE_NONE)
static void ConsolePrintNumber(signed int number);
static void ConsolePrintHex(unsigned int hex);
static void ConsolePrintFloat(float number, unsigned int precision);
static void ConsoleSpecificInit(void);
static void CheckConsoleSize(void);
static void ConsolePrintChar(char c);
static void ConsolePrintHeader(void);
static void ConsoleSync(void);
#endif

/*************************** Variables Definitions ***************************/

#if !defined(CONFIG_CONSOLE_NONE)
static mutexHandle_t console_mutex = {0};
#endif

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitConsole(void)
 * @brief       Initialise the console
 * @return      Nothing
 */
void InitConsole(void)
{
#if !defined(CONFIG_CONSOLE_NONE)
    // First initialise console mutex
    static mutexQueue_t console_mutex_queue = {0};
    console_mutex = xSemaphoreCreateMutexStatic(&console_mutex_queue);
    portENABLE_INTERRUPTS(); // WORKAROUND : FreeRTOS API disable interrupts by default if scheduler has not been started.
    
    // Then do the specific init depending on the console mode
    ConsoleSpecificInit();
#endif
}

/**
 * @fn          ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision)
 * @brief       Print message in console
 * @param[in]   msg         Message we want to print
 * @param[in]   dnumber     Signed number, '%d' must be included in the message
 * @param[in]   hnumber     Hexadecimal number, '%x' must be included in the message
 * @param[in]   fnumber     Floating point number, '%f' must be included in the message
 * @param[in]   fprecision  Floating point number precision, it means how many digits will be printed after decimal seperator
 * @return      Nothing
 */
extern void ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision)
{
#if !defined(CONFIG_CONSOLE_NONE)
    // First Acquire Mutex
    (void)xSemaphoreTake(console_mutex, portMAX_DELAY);

    // Then Check the console size
    CheckConsoleSize();

    // Variables Initialisation
    uint32_t line_index = 0u;
    uint32_t i = 0u;

    // Function Core
    while (msg[i] != '\0')
    {
        // If first char of the line, print the header
        if (line_index == 0u)
        {
            ConsolePrintHeader();
        }

        // Check for format specifiers
        if (msg[i] == '%' && msg[i + 1] == 'd')
        {
            ConsolePrintNumber(dnumber);
            i++; // Skip the format specifier
        }
        else if (msg[i] == '%' && msg[i + 1] == 'x')
        {
            ConsolePrintHex(hnumber);
            i++; // Skip the format specifier
        }
        else if (msg[i] == '%' && msg[i + 1] == 'f')
        {
            ConsolePrintFloat(fnumber, fprecision);
            i++; // Skip the format specifier
        }
        else
        {
            // Print the character normally
            ConsolePrintChar(msg[i]);
        }

        // If the char was '\n' reset line_index
        if (msg[i] == '\n')
        {
            line_index = 0u;
        }
        else
        {
            line_index++;
        }

        // Increment index of the message
        i++;
    }

    // Check if the last character is not '\n'
    if (i > 0 && msg[i - 1] != '\n')
    {
        ConsolePrintChar('\n');  // Add a newline if not already present
    }

    // Synchronise console
    ConsoleSync();

    // Release Mutex
    (void)xSemaphoreGive(console_mutex);
#else
    (void)(msg);
    (void)(dnumber);
    (void)(hnumber);
    (void)(fnumber);
    (void)(fprecision);
#endif /* CONFIG_CONSOLE_NONE */
}

#if !defined(CONFIG_CONSOLE_NONE)
/**
 * @fn          ConsolePrintNumber(signed int number)
 * @brief       Function used to print an signed integer
 * @param[in]   number  Number that will be printed
 * @return      Nothing
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
        char buffer[12]; // 12 characters is sufficient to store a signed integer (absolute max value is 2147483648 which is 10 char + 1 sign char + we add 1 char of margin)
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
 * @param[in]   hex Number that will be printed
 * @return      Nothing
 */
static void ConsolePrintHex(unsigned int hex)
{
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
}

/**
 * @fn          ConsolePrintFloat(float number, unsigned int precision)
 * @brief       Function used to print a floating point number with specified precision
 * @param[in]   number    Number that will be printed
 * @param[in]   precision Number of digits after the decimal point
 * @return      Nothing
 */
static void ConsolePrintFloat(float number, unsigned int precision)
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
    for (unsigned int i = 0; i < precision; i++)
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
 * @fn          ConsolePrintHeader
 * @brief       Function that prints the header of each line
 * @return      Nothing
 *
 * Currently the header is the CUC time
 */
static void ConsolePrintHeader(void)
{
    // Variable Initialisation
    time_t time = 0u;
    taskNo_t task = 0u;

    // First get time and task no
    (void)GetTime(&time);
    (void)GetCurrentTask(&task);

    // Print header start
    ConsolePrintChar('[');

    // Print timestamp
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

    // Print task no
    ConsolePrintChar(',');
    ConsolePrintChar('#');
    ConsolePrintNumber(task);

    // Print header end
    ConsolePrintChar(']');
    ConsolePrintChar(':');
    ConsolePrintChar(' ');
}
#endif /* CONFIG_CONSOLE_NONE */

/************************ File Based Console Functions ***********************/

#if defined(CONFIG_CONSOLE_FILE)

#if defined(CONFIG_FS_NONE)
#error "Incompatible choice between CONFIG_FS_NONE and CONFIG_CONSOLE_FILE"
#endif

/**
 * @fn          ConsoleSpecificInit
 * @brief       Initialisation specific to the console type choosed
 * @return      Nothing
 */
static void ConsoleSpecificInit(void)
{
    // Variable Initialisation
    length_t file_size = 0u;
    
    // Function Core
    (void)FsIoctl(CONSOLE_FILE, FS_IOCTL_GET_SIZE, &file_size, sizeof(file_size));
    (void)FsIoctl(CONSOLE_FILE, FS_IOCTL_SEEK, &file_size, sizeof(file_size));
}

/**
 * @fn          CheckConsoleSize
 * @brief       Check the console file size update the file if it reaches the maximum size
 * @return      Nothing
 *
 * If reach the maximum size, the content is saved in
 * the console_old.log and a new console.log is opened.
 */
static void CheckConsoleSize(void)
{
    // Variable initialisation
    uint32_t console_size = 0u;
    // Function Core

    (void)FsIoctl(CONSOLE_FILE, FS_IOCTL_GET_SIZE, &console_size, sizeof(console_size));
    if (console_size > ((uint32_t)(CONFIG_CONSOLE_FILE_SIZE) * 1024u))
    {
        fileNo_t old_console_no = CONSOLE_OLD_FILE;
        (void)FsIoctl(CONSOLE_FILE, FS_IOCTL_TRANSFER_DATA, &old_console_no, sizeof(fileNo_t));
    }
}

/**
 * @fn          ConsolePrintChar(char c)
 * @brief       Function used to print a character
 * @param[in]   c Character that will be printed
 * @return      Nothing
 */
static void ConsolePrintChar(char c)
{
    // Function Core
    (void)FsWrite(CONSOLE_FILE, (data_t)&c, sizeof(char));
}

/**
 * @fn          ConsoleSync(void)
 * @brief       Allow to flush data onto the file system if CONFIG_CONSOLE_FILE used
 * @return      Nothing
 */
static void ConsoleSync(void)
{
    (void)FsIoctl(CONSOLE_FILE, FS_IOCTL_SYNC, NULL, 0u);
}

#endif /* CONFIG_CONSOLE_FILE */

/************************ UART Based Console Functions ***********************/

#if defined(CONFIG_CONSOLE_UART)

#define CONSOLE_BAUDRATE    115200u

/**
 * @var     uart_print_inst
 * @brief   uart_print instance declaration
 */
static uartInst_t uart_print_inst = {
    .uart_ref = UART_PRINT_REF,
    .drive_type = UART_POLLING_DRIVE,
    .baudrate = CONSOLE_BAUDRATE,
    .irq_no = UART_PRINT_IRQ_NO,
};

/**
 * @fn          ConsoleSpecificInit
 * @brief       Initialisation specific to the console type choosed
 * @return      Nothing
 */
static void ConsoleSpecificInit(void)
{
    (void)UartOpen(&uart_print_inst);
}

/**
 * @fn          CheckConsoleSize
 * @brief       Check the console file size update the file if it reaches the maximum size
 * @return      Nothing
 *
 * Does nothing for this console type
 */
static void CheckConsoleSize(void)
{
    // Nothing to do 
}

/**
 * @fn          ConsolePrintChar(char c)
 * @brief       Function used to print a character
 * @param[in]   c Character that will be printed
 * @return      Nothing
 */
static void ConsolePrintChar(char c)
{
    // Function Core
    (void)UartWrite(&uart_print_inst, (data_t)&c, sizeof(char));
}

/**
 * @fn          ConsoleSync(void)
 * @brief       Allow to flush data onto the file system if CONFIG_CONSOLE_FILE used
 * @return      Nothing
 * 
 * Does nothing for this console type
 */
static void ConsoleSync(void)
{
    // Nothing to do 
}

#endif /* CONFIG_CONSOLE_UART */

/************************ UART Based Console Functions ***********************/

#if defined(CONFIG_CONSOLE_ITM)

/**
 * @fn          ConsoleSpecificInit
 * @brief       Initialisation specific to the console type choosed
 * @return      Nothing
 */
static void ConsoleSpecificInit(void)
{
    // Nothing to do
}

/**
 * @fn          CheckConsoleSize
 * @brief       Check the console file size update the file if it reaches the maximum size
 * @return      Nothing
 *
 * Does nothing for this console type
 */
static void CheckConsoleSize(void)
{
    // Nothing to do 
}

/**
 * @fn          ConsolePrintChar(char c)
 * @brief       Function used to print a character
 * @param[in]   c Character that will be printed
 * @return      Nothing
 */
static void ConsolePrintChar(char c)
{
    // Function Core
    (void)ITM_SendChar(c);
}

/**
 * @fn          ConsoleSync(void)
 * @brief       Allow to flush data onto the file system if CONFIG_CONSOLE_FILE used
 * @return      Nothing
 * 
 * Does nothing for this console type
 */
static void ConsoleSync(void)
{
    // Nothing to do 
}

#endif /* CONFIG_CONSOLE_ITM */

/****************** Circular Buffer Based Console Functions ******************/

#if defined(CONFIG_CONSOLE_CIRCULAR_BUFFER)

/**
 * @var     g_circular_buffer
 * @brief   Circular buffer for console printing
 */
uint8_t g_circular_buffer[CONFIG_CIRCULAR_BUFFER_SIZE*1024u] __attribute__((aligned(32))) = {0};

/**
 * @fn          ConsoleSpecificInit
 * @brief       Initialisation specific to the console type choosed
 * @return      Nothing
 * 
 * Does nothing for this console type
 */
static void ConsoleSpecificInit(void)
{
    // Nothing to do 
}

/**
 * @fn          CheckConsoleSize
 * @brief       Check the console file size update the file if it reaches the maximum size
 * @return      Nothing
 *
 * Does nothing for this console type
 */
static void CheckConsoleSize(void)
{
    // Nothing to do 
}

/**
 * @fn          ConsolePrintChar(char c)
 * @brief       Function used to print a character
 * @param[in]   c Character that will be printed
 * @return      Nothing
 */
static void ConsolePrintChar(char c)
{
    // Variable declaration
    static uint32_t circular_buffer_index = 0u;

    // Function Core
    if (circular_buffer_index == ((uint32_t)CONFIG_CIRCULAR_BUFFER_SIZE * 1024u))
    {
        circular_buffer_index = 0u;
    }
    g_circular_buffer[circular_buffer_index] = c;
    circular_buffer_index++;
}

/**
 * @fn          ConsoleSync(void)
 * @brief       Allow to flush data onto the file system if CONFIG_CONSOLE_FILE used
 * @return      Nothing
 * 
 * Does nothing for this console type
 */
static void ConsoleSync(void)
{
    // Nothing to do 
}

#endif /* CONFIG_CONSOLE_CIRCULAR_BUFFER */
