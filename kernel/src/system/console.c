/**
 * @file    console.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Console functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/console.h"
#include "core/time.h"
#include "core/tasks.h"
#include "fdir/fdir.h"
#include "core/fs.h"
#include "drv/peripherals.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

#if !defined(CONFIG_CONSOLE_NONE)
static void ConsoleLock(void);
static void ConsoleUnlock(void);
static void ConsolePrintNumber(signed int number);
static void ConsolePrintHex(unsigned int hex);
static void ConsolePrintFloat(float number, unsigned int precision);
static void ConsolePrintHeader(void);
static void ConsoleSpecificInit(void);
static void CheckConsoleSize(void);
static void ConsolePrintChar(char c);
static void ConsoleSync(void);
#endif

/*************************** Variables Definitions ***************************/

#if !defined(CONFIG_CONSOLE_NONE)
static consoleStatus_t console_status = CONSOLE_NOT_INITIALISED;
static mutexHandle_t console_mutex    = { 0 };
static bool console_mutex_initialised = false;
#endif /* !CONFIG_CONSOLE_NONE */

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitConsole(void)
 * @brief       Initialise the console
 * @return      Nothing
 */
void InitConsole(void)
{
#if !defined(CONFIG_CONSOLE_NONE)
    // Then do the specific init depending on the console mode
    ConsoleSpecificInit();

    // Finally declare the console initialised
    console_status = CONSOLE_INITIALISED;
#endif
}

/**
 * @fn      CreateConsoleMutexes(void)
 * @brief   Function that allows to postpone mutex initilisation when other mutexes will be initialised.
 */
void CreateConsoleMutexes(void)
{
#if !defined(CONFIG_CONSOLE_NONE)
    static mutexQueue_t console_mutex_queue = { 0 };

    // Initialise mutex for the filesystem
    console_mutex = xSemaphoreCreateMutexStatic(&console_mutex_queue);
    if (console_mutex == NULL)
    {
        KernelPanic();
    }
    else
    {
        console_mutex_initialised = true;
    }
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
    // Print only if the console is initialised
    if (console_status == CONSOLE_INITIALISED)
    {
        uint32_t line_index = 0u;
        uint32_t i          = 0u;

        // First lock the console
        ConsoleLock();

        // Then Check the console size
        CheckConsoleSize();

        // While there are still characters in the string
        while (msg[i] != '\0')
        {
            // If first char of the line, print the header
            if (line_index == 0u)
            {
                ConsolePrintHeader();
            }

            // Check for format specifiers
            if ((msg[i] == '%') && (msg[i + 1u] == 'd'))
            {
                ConsolePrintNumber(dnumber);
                i++; // Skip the format specifier
            }
            else if ((msg[i] == '%') && (msg[i + 1u] == 'x'))
            {
                ConsolePrintHex(hnumber);
                i++; // Skip the format specifier
            }
            else if ((msg[i] == '%') && (msg[i + 1u] == 'f'))
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
        if ((i > 0u) && (msg[i - 1u] != '\n'))
        {
            ConsolePrintChar('\n'); // Add a newline if not already present
        }

        // Synchronise console
        ConsoleSync();

        // Finally unlock the console
        ConsoleUnlock();
    }
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
 * @fn      ConsoleLock(void)
 * @brief   Lock the Console with a mutex
 * @return  Nothing
 */
static void ConsoleLock(void)
{
    BaseType_t mutex_status;

    // Lock the mutex if it has been initialised
    if (console_mutex_initialised)
    {
        mutex_status = xSemaphoreTake(console_mutex, portMAX_DELAY);
        if (mutex_status != pdTRUE)
        {
            KernelPanic();
        }
    }
}

/**
 * @fn      ConsoleUnlock(void)
 * @brief   Unlock the Console (which has been locked with a mutex)
 * @return  Nothing
 */
static void ConsoleUnlock(void)
{
    BaseType_t mutex_status;

    // Unlock the mutex if it has been initialised
    if (console_mutex_initialised)
    {
        mutex_status = xSemaphoreGive(console_mutex);
        if (mutex_status != pdTRUE)
        {
            KernelPanic();
        }
    }
}

/**
 * @fn          ConsolePrintNumber(signed int number)
 * @brief       Function used to print an signed integer
 * @param[in]   number  Number that will be printed
 * @return      Nothing
 */
void ConsolePrintNumber(signed int number)
{
    int remaining_number = number;

    // If number is zero print 0
    if (remaining_number == 0)
    {
        ConsolePrintChar('0');
    }
    else
    {
        // Init string buffer
        char buffer[12]; // 12 characters is sufficient to store a signed integer (absolute max value is 2147483648 which is 10 char + 1 sign char +
                         // we add 1 char of margin)
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
            buffer[i]         = (remaining_number % 10) + '0';
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
    int integerPart      = 0;
    float fractionalPart = 0.0f;

    // Check the sign
    if (number < 0.0f)
    {
        // Number is negative
        ConsolePrintChar('-');
        integerPart    = (int)(-number);
        fractionalPart = (-number) - (float)integerPart;
    }
    else
    {
        // Number is positive
        integerPart    = (int)number;
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
        int digit       = (int)fractionalPart;

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
    // First get time and task no
    time_t time   = GetTime();
    taskNo_t task = GetCurrentTask();

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

#define LOG_DIRECTORY_PATH           "logs"                                /**< Log directory path */
#define CONSOLE_FILE_PATH            "logs/console.log"                    /**< Console file path */
#define CONSOLE_FILE_ACCESS_MODE     (FA_OPEN_ALWAYS | FA_WRITE | FA_READ) /**< Console file acess mode */
#define OLD_CONSOLE_FILE_PATH        "logs/old_console.log"                /**< Old console file path */
#define OLD_CONSOLE_FILE_ACCESS_MODE (FA_OPEN_ALWAYS | FA_WRITE | FA_READ) /**< Old console file acess mode */

/**
 * @var     console_file
 * @brief   Console file
 */
static FIL console_file = { 0 };

/**
 * @var     old_console_file
 * @brief   Old console file
 */
static FIL old_console_file = { 0 };

/**
 * @fn          ConsoleSpecificInit
 * @brief       Initialisation specific to the console type choosed
 * @return      Nothing
 */
static void ConsoleSpecificInit(void)
{
    // First create "logs" directory
    FRESULT res = f_mkdir(LOG_DIRECTORY_PATH);
    if ((res != FR_OK) && (res != FR_EXIST))
    {
        KernelPanic();
    }

    // Then open files
    if (f_open(&console_file, CONSOLE_FILE_PATH, CONSOLE_FILE_ACCESS_MODE) != FR_OK)
    {
        KernelPanic();
    }
    if (f_open(&old_console_file, OLD_CONSOLE_FILE_PATH, OLD_CONSOLE_FILE_ACCESS_MODE) != FR_OK)
    {
        KernelPanic();
    }

    // Put file pointer at the end of the console file
    if (f_lseek(&console_file, f_size(&console_file)) != FR_OK)
    {
        KernelPanic();
    }
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
    // If the maximum size is reached, then transfer the logs to console_old.log
    if (f_size(&console_file) > ((uint32_t)(CONFIG_CONSOLE_FILE_SIZE) * 1024u))
    {
        // First close the files in order to avoid issues when renaming and deleting files
        if (f_close(&console_file) != FR_OK)
        {
            KernelPanic();
        }
        if (f_close(&old_console_file) != FR_OK)
        {
            KernelPanic();
        }

        // Remove the old console file (we keep only one old file)
        if (f_unlink(OLD_CONSOLE_FILE_PATH) != FR_OK)
        {
            KernelPanic();
        }

        // Then rename the file
        if (f_rename(CONSOLE_FILE_PATH, OLD_CONSOLE_FILE_PATH) != FR_OK)
        {
            KernelPanic();
        }

        // Then we can open the console files again
        if (f_open(&console_file, CONSOLE_FILE_PATH, CONSOLE_FILE_ACCESS_MODE) != FR_OK)
        {
            KernelPanic();
        }
        if (f_open(&old_console_file, OLD_CONSOLE_FILE_PATH, OLD_CONSOLE_FILE_ACCESS_MODE) != FR_OK)
        {
            KernelPanic();
        }
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
    UINT nb_writen;
    if ((f_write(&console_file, &c, 1u, &nb_writen) != FR_OK) || (nb_writen != 1u))
    {
        KernelPanic();
    }
}

/**
 * @fn          ConsoleSync(void)
 * @brief       Allow to flush data onto the file system if CONFIG_CONSOLE_FILE used
 * @return      Nothing
 */
static void ConsoleSync(void)
{
    if (f_sync(&console_file) != FR_OK)
    {
        KernelPanic();
    }
}

#endif /* CONFIG_CONSOLE_FILE */

/************************ UART Based Console Functions ***********************/

#if defined(CONFIG_CONSOLE_UART)

#define CONSOLE_BAUDRATE 115200u

/**
 * @var     uart_print_conf
 * @brief   uart_print configuration declaration
 */
static const uartConf_t uart_print_conf = {
    .uart_ref     = UART_PRINT_REF,
    .default_mode = POLLING_MODE,
    .baudrate     = CONSOLE_BAUDRATE,
    .irq_no       = UART_PRINT_IRQ_NO,
};

/**
 * @var     uart_print_desc
 * @brief   uart_print descriptor declaration
 */
static uartInst_t uart_print_desc = { 0 };

/**
 * @fn          ConsoleSpecificInit
 * @brief       Initialisation specific to the console type choosed
 * @return      Nothing
 */
static void ConsoleSpecificInit(void)
{
    if (UartOpen(&uart_print_desc, &uart_print_conf) != RET_SUCCESSFUL)
    {
        KernelPanic();
    }
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
    (void)UartWrite(&uart_print_desc, (data_t)&c, sizeof(char));
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
uint8_t g_circular_buffer[CONFIG_CIRCULAR_BUFFER_SIZE * 1024u] __attribute__((aligned(32))) = { 0 };

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

    // Check if the pointer reach the end of the circular buffer
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
