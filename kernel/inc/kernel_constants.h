/**
 * @file    kernel_constants.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for kernel
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 */

#ifndef KERNEL_CONSTANTS_H
#define KERNEL_CONSTANTS_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"
#include "conf/mutex_conf.h"
#include "conf/fs_conf.h"
#include "conf/peripherals_conf.h"
#include "conf/hk_conf.h"
#include "conf/timers_conf.h"

/*************************** Constants Definitions ***************************/

/************************************/
/********* GENERAL CONSTANTS ********/
/************************************/

#define NB_KERNEL_TASKS            4u /**< Number of kernel internal task (Idle, Tmr Svc, SYSWDG and SYSMON) */

#define NO_BUFFER                  (-1u) /**< Reference number to refer for no buffer */
#define NO_DEVICE                  (-1u) /**< Reference number for no device */
#define NO_TASK                    0u    /**< Reference number to refer for no task */
#define ANY_TASK                   (-1u) /**< Reference number to refer to any task */
#define HKID_ALL                   0u    /**< Reference for selecting every HKID */
#define NO_PERIOD                  0u    /**< Reference for period when a task is aperiodic */

#define SYSDEV_SYSTEM_INFO         0u /**< System device reference number for system information */
#define SYSDEV_SYSTEM_USAGE        1u /**< System device reference number for system usage */

/************************************/
/********** TIME CONSTANTS **********/
/************************************/

#define INVALID_TIME               (-1ull) /**< Invalid timestamp constant */

#define CUC_P_FIELD_CONSTANT       0x1fu                 /**< P-field for CUC time (equivalent of 0b00011111u)*/
#define CUC_P_FIELD_OFFSET         56                    /**< CUC P field offset */
#define CUC_P_FIELD_MASK           0xff00000000000000ull /**< CUC P field mask */
#define CUC_BASIC_TIME_OFFSET      24                    /**< CUC basic time field offset */
#define CUC_BASIC_TIME_MASK        0x00ffffffff000000ull /**< CUC basic time field mask */
#define CUC_FRACTIONAL_TIME_OFFSET 8                     /**< CUC fractional time field offset */
#define CUC_FRACTIONAL_TIME_MASK   0x0000000000ffffffull /**< CUC fractional time field mask */

#define SECONDS_IN_DAY             86400u      /**< Number of seconds in a day */
#define SECONDS_IN_HOUR            3600u       /**< Number of seconds in a hour */
#define SECONDS_IN_MINUTE          60u         /**< Number of seconds in a minute */
#define DAYS_IN_YEAR               365u        /**< Number of days in a year */
#define DAYS_IN_LEAP_YEAR          366u        /**< Number of days in a leap year (occures every 4 years execept some years) */
#define JANUARY_FIRST_2000         1325376000u /**< TAI timestamp for january 1rst 2000 (RTC cannot compute time before this date) */

/************************************/
/********** TASK PRIORITIES *********/
/************************************/

#define PRIORITY_LOW               8u  /**< Low priority tasks */
#define PRIORITY_BELOW_NORMAL      16u /**< Below normal priority tasks */
#define PRIORITY_NORMAL            24u /**< Normal priority tasks */
#define PRIORITY_ABOVE_NORMAL      32u /**< Above normal priority tasks */
#define PRIORITY_HIGH              40u /**< High priority tasks */
#define PRIORITY_EXTREME           48u /**< Extreme priority tasks */

/************************************/
/********** IOCTL COMMANDS **********/
/************************************/

#define IOCTL_SET_EXTRA_INFO       0u /**< Generic IOTCL command to set device extra info */

#define IOCTL_BUFFER_GET_COUNT     100u /**< BUFFER IOCTL get buffer count (how many message are in the buffer) */
#define IOCTL_BUFFER_GET_RECEIVER  101u /**< BUFFER IOCTL get buffer receiver */
#define IOCTL_BUFFER_GET_SENDER    102u /**< BUFFER IOCTL get buffer sender */

#define IOCTL_FS_GET_SIZE          200u /**< FS IOCTL get file size */
#define IOCTL_FS_SEEK              201u /**< FS IOCTL moves read/write pointer in the file */
#define IOCTL_FS_SYNC              202u /**< FS IOCTL synchronise file on the disk */
#define IOCTL_FS_TRANSFER_DATA     203u /**< FS IOCTL transfer data from the current file to another one */

#define IOCTL_PERIPHERAL_START_RX  300u /**< Peripheral IOCTL start reception */
#define IOCTL_PERIPHERAL_START_TX  301u /**< Peripheral IOCTL start transmission */
#define IOCTL_PERIPHERAL_CHECK_RX  302u /**< Peripheral IOCTL check reception */
#define IOCTL_PERIPHERAL_CHECK_TX  303u /**< Peripheral IOCTL check transmission */
#define IOCTL_GPIO_TOGGLE          310u /**< GPIO IOCTL toggle GPIO */
#define IOCTL_I2C_SET_SLAVE_ADDR   311u /**< I2C IO CTL set slave adress */
#define IOCTL_SPI_SET_TX_MSG       312u /**< SPI IO CTL set transmit message (when doing a read) */
#define IOCTL_OW_INIT_CONNECTION   313u /**< OW IO CTL init connection */

/************************************/
/************** SIGNALS *************/
/************************************/

// User Signals
#define USER_SIGNALS               0x0000ffffu /**< User signals mask */
#define SIGNAL_USER0               0x00000001u /**< User signal 0 that can be used for a specific application */
#define SIGNAL_USER1               0x00000002u /**< User signal 1 that can be used for a specific application */
#define SIGNAL_USER2               0x00000004u /**< User signal 2 that can be used for a specific application */
#define SIGNAL_USER3               0x00000008u /**< User signal 3 that can be used for a specific application */
#define SIGNAL_USER4               0x00000010u /**< User signal 4 that can be used for a specific application */
#define SIGNAL_USER5               0x00000020u /**< User signal 5 that can be used for a specific application */
#define SIGNAL_USER6               0x00000040u /**< User signal 6 that can be used for a specific application */
#define SIGNAL_USER7               0x00000080u /**< User signal 7 that can be used for a specific application */
#define SIGNAL_USER8               0x00000100u /**< User signal 8 that can be used for a specific application */
#define SIGNAL_USER9               0x00000200u /**< User signal 9 that can be used for a specific application */
#define SIGNAL_USER10              0x00000400u /**< User signal 10 that can be used for a specific application */
#define SIGNAL_USER11              0x00000800u /**< User signal 11 that can be used for a specific application */
#define SIGNAL_USER12              0x00001000u /**< User signal 12 that can be used for a specific application */
#define SIGNAL_USER13              0x00002000u /**< User signal 13 that can be used for a specific application */
#define SIGNAL_USER14              0x00004000u /**< User signal 14 that can be used for a specific application */
#define SIGNAL_USER15              0x00008000u /**< User signal 15 that can be used for a specific application */

// System Signals
#define SYSTEM_SIGNALS             0x00ff0000u /**< System signals mask */
#define SIGNAL_TIMER_ENDED         0x00010000u /**< System signal indicating a timer ended for a specific task */
#define SIGNAL_PERIPHERAL_RX_DONE  0x00020000u /**< System signal indicating a peripheral RX action ended for a specific task */
#define SIGNAL_PERIPHERAL_TX_DONE  0x00040000u /**< System signal indicating a peripheral TX action ended for a specific task */
#define SIGNAL_PERIPHERAL          0x00060000u /**< System signal indicating a peripheral action (RX or TX) ended for a specific task */
#define SIGNAL_EVENT               0x00080000u /**< System signal indicating the system a new event occured for the system */
#define SIGNAL_HOUSEKEEPING        0x00100000u /**< System signal indicating the system a new housekeeping has been produced for the system */

// Kernel Signals
#define KERNEL_SIGNALS             0xff000000u /**< Kernel signals mask */
#define SIGNAL_SUSPEND             0x01000000u /**< Kernel signal indicating a suspension for a specific task */
#define SIGNAL_MODE_CHANGE         0x02000000u /**< Kernel signal indicating a mode change for a specific task */

/************************************/
/************ SYSCALLS NO ***********/
/************************************/

#define SYSCALL_RESERVED           0u  /**< Syscall numero reserved, do not use */
#define SYSCALL_CHECK_ERROR        1u  /**< Syscall numero for CheckError */
#define SYSCALL_SLEEP              2u  /**< Syscall numero for Sleep */
#define SYSCALL_SLEEP_PERIODIC     3u  /**< Syscall numero for SleepPeriodic */
#define SYSCALL_GET_TICK           4u  /**< Syscall numero for GetTick */
#define SYSCALL_GET_TIME           5u  /**< Syscall numero for GetTime */
#define SYSCALL_SET_TIME           6u  /**< Syscall numero for SetTime */
#define SYSCALL_DEVICE_OPEN        7u  /**< Syscall numero for DeviceOpen */
#define SYSCALL_DEVICE_WRITE       8u  /**< Syscall numero for DeviceWrite */
#define SYSCALL_DEVICE_READ        9u  /**< Syscall numero for DeviceRead */
#define SYSCALL_DEVICE_IOCTL       10u /**< Syscall numero for DeviceIoctl */
#define SYSCALL_DEVICE_CLOSE       11u /**< Syscall numero for DeviceClose */
#define SYSCALL_GET_CURRENT_TASK   12u /**< Syscall numero for GetCurrentTask */
#define SYSCALL_SUSPEND_TASK       13u /**< Syscall numero for SuspendTask */
#define SYSCALL_RESUME_TASK        14u /**< Syscall numero for ResumeTask */
#define SYSCALL_GET_TASK_PRIORITY  15u /**< Syscall numero for GetTaskPriority */
#define SYSCALL_SET_TASK_PRIORITY  16u /**< Syscall numero for SetTaskPriority */
#define SYSCALL_ACQUIRE_MUTEX      17u /**< Syscall numero for AcquireMutex */
#define SYSCALL_RELEASE_MUTEX      18u /**< Syscall numero for ReleaseMutex */
#define SYSCALL_SEND_SIGNAL        19u /**< Syscall numero for SendSignal */
#define SYSCALL_WAIT_SIGNAL        20u /**< Syscall numero for WaitSignal */
#define SYSCALL_START_TIMER        21u /**< Syscall numero for StartTimer */
#define SYSCALL_PAUSE_TIMER        22u /**< Syscall numero for PauseTimer */
#define SYSCALL_RESUME_TIMER       23u /**< Syscall numero for ResumeTimer */
#define SYSCALL_SET_TIMER          24u /**< Syscall numero for SetTimer */
#define SYSCALL_CONSOLE_PRINT      25u /**< Syscall numero for ConsolePrint */
#define SYSCALL_ENABLE_HK          26u /**< Syscall numero for EnableHK */
#define SYSCALL_DISABLE_HK         27u /**< Syscall numero for DisableHK */
#define SYSCALL_EMIT_HK            28u /**< Syscall numero for EmitHK */
#define SYSCALL_COLLECT_HKS        29u /**< Syscall numero for CollectHKs */
#define NB_SYSCALLS                30u /**< Number of syscalls */

#define SYSCALL_EXIT               255u /**< Syscall numero for exiting a syscall */

#endif /* KERNEL_CONSTANTS_H */

/**
 * @}
 */