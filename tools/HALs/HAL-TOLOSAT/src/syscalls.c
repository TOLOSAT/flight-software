/**
 * @file    syscalls.c
 * @author  Merlin Kooshmanian inspired by Carmine Noviello
 * @brief   Source file contening system calls using UART
 * @date    26/12/2022
 *
 * Inspired from mastering-stm32 by cnoviello on github.com
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

#define STDIN_FILENO    0   /**< File descriptor of STDIN */
#define STDOUT_FILENO   1   /**< File descriptor of STDOUT */
#define STDERR_FILENO   2   /**< File descriptor of STDERR */

/*************************** Functions Declarations **************************/

extern void InitConsole(uartInst_t *uart_inst);
extern int _isatty(int fd);
extern int _write(int fd, char *ptr, int len);
extern int _close(int fd);
extern int _lseek(int fd, int ptr, int dir);
extern int _read(int fd, char *ptr);
extern int _fstat(int fd, struct stat *st);

/*************************** Variables Definitions ***************************/

/**
 * @var   print_inst
 * @brief UART temporary istance before it has been affected in InitConsole function
 */
static uartInst_t *print_inst;

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitConsole(uartInst_t *uart_inst)
 * @brief   Initialise monitoring linking pirntf to uart
 * @param   uart_inst UART instance that will be linked to printf
 */
void InitConsole(uartInst_t *uart_inst)
{
    print_inst = uart_inst;

    /* Disable I/O buffering for STDOUT stream, so that
     * chars are sent out as soon as they are printed. */
    setvbuf(stdout, NULL, _IONBF, 0);
}

/**
 * @fn      _isatty(int fd)
 * @brief   Is a function that returns 1 if the file refers to a terminal
 * @param   fd File descriptor
 * @retval  0 if file descriptor is a terminal
 * @retval  1 if file descriptor is not a terminal
 */
int _isatty(int fd)
{
    if ((fd >= STDIN_FILENO) && (fd <= STDERR_FILENO))
        return 1;

    errno = EBADF;
    return 0;
}

/**
 * @fn      _write(int fd, char *ptr, int len)
 * @brief   Write a message in STDOUT or STDERR
 * @param   fd File descriptor
 * @param   ptr Message pointer
 * @param   len Message lenght
 * @retval  len if write is successful
 * @retval  EIO (Error IO) if writing fails
 * @retval  -1 if fd is not STDOUT_FILENO or STDERR_FILENO
 */
int _write(int fd, char *ptr, int len)
{
    if ((fd == STDOUT_FILENO) || (fd == STDERR_FILENO))
    {
        halStatus_t status = UartWrite(print_inst, (uint8_t *)ptr, len);
        if (status == THAL_SUCCESSFUL)
            return len;
        else
            return EIO;
    }
    errno = EBADF;
    return -1;
}

/**
 * @fn      _close(int fd)
 * @brief   Close syscall connection
 * @param   fd File descriptor
 * @retval  0 if fd is STDIN_FILENO, STDOUT_FILENO or STDERR_FILENO
 * @retval  -1 if fd is not STDIN_FILENO, STDOUT_FILENO or STDERR_FILENO
 */
int _close(int fd)
{
    if ((fd >= STDIN_FILENO) && (fd <= STDERR_FILENO))
        return 0;

    errno = EBADF;
    return -1;
}

/**
 * @fn      _lseek(int fd, int ptr, int dir)
 * @brief   Allows the file offset to be set beyond the end of the file
 * @param   fd File descriptor
 * @param   ptr Message pointer
 * @param   dir Message offset
 * @return  -1 always
 *
 * @warning This function looks to be unavaible
 */
int _lseek(int fd, int ptr, int dir)
{
    (void)fd;
    (void)ptr;
    (void)dir;

    errno = EBADF;
    return -1;
}

/**
 * @fn      _read(int fd, char *ptr)
 * @brief   Read a message from STDIN
 * @param   fd File descriptor
 * @param   ptr Message pointer
 * @retval  1 if read is successful
 * @retval  EIO (Error IO) if reading fails
 * @retval  -1 if fd is not STDIN_FILENO
 */
int _read(int fd, char *ptr)
{
    if (fd == STDIN_FILENO)
    {
        halStatus_t status = UartRead(print_inst, (uint8_t *)ptr, 1);
        if (status == THAL_SUCCESSFUL)
            return 1;
        else
            return EIO;
    }
    errno = EBADF;
    return -1;
}

/**
 * @fn      _fstat(int fd, struct stat *st)
 * @brief   Gets status information about the object specified by the open descriptor
 * @param   fd File descriptor
 * @param   st Status
 * @retval  0 always
 */
int _fstat(int fd, struct stat *st)
{
    if ((fd >= STDIN_FILENO) && (fd <= STDERR_FILENO))
    {
        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return 0;
}
