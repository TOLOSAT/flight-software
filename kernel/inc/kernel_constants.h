/**
 * @file    kernel_constants.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for kernel
 *
 * @copyright Copyright (c) TOLOSAT 2024
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

/*************************** Constants Definitions ***************************/

/************************************/
/********* GENERAL CONSTANTS ********/
/************************************/

#define NB_KERNEL_TASKS             3u      /**< Number of kernel internal task (Idle, Tmr Svc and Watchdog) */

#define NO_BUFFER                   (-1u)   /**< Reference number to refer for no buffer */
#define NO_DEVICE                   (-1u)   /**< Device numero for no device */
#define ANY_TASK                    (-1u)   /**< Reference number to refer to any task */
#define HKID_ALL                    0u      /**< Reference for selecting every HKID */

#define DEVICE_NO_EXTRA_INFO        0u      /**< Constant to inform there is no extra data */

#define SYSDEV_SYSTEM_INFO          0u      /**< System device reference number for system information */
#define SYSDEV_SYSTEM_USAGE         1u      /**< System device reference number for system usage */

/************************************/
/********** TASK PRIORITIES *********/
/************************************/

#define PRIORITY_LOW                8u      /**< Low priority tasks */
#define PRIORITY_BELOW_NORMAL       16u     /**< Below normal priority tasks */
#define PRIORITY_NORMAL             24u     /**< Normal priority tasks */
#define PRIORITY_ABOVE_NORMAL       32u     /**< Above normal priority tasks */
#define PRIORITY_HIGH               40u     /**< High priority tasks */
#define PRIORITY_EXTREME            48u     /**< Extreme priority tasks */
#define TASK_NB_CONFIG_REGIONS      11u     /**< Number of regions which can be configured for a task */

/************************************/
/********** IOCTL COMMANDS **********/
/************************************/

#define IOCTL_LOCK_DEVICE           0u      /**< Generic IOTCL command to lock the device linked file or peripheral */
#define IOCTL_UNLOCK_DEVICE         1u      /**< Generic IOTCL command to unlock the device linked file or peripheral */
#define IOCTL_SET_EXTRA_INFO        2u      /**< Generic IOTCL command to set device extra info */

#define BUFFER_IOCTL_GET_COUNT      20u     /**< BUFFER IOCTL get buffer count (how many message are in the buffer) */

#define FS_IOCTL_GET_SIZE           50u     /**< FS IOCTL get file size */
#define FS_IOCTL_SEEK               51u     /**< FS IOCTL moves read/write pointer in the file */
#define FS_IOCTL_SYNC               52u     /**< FS IOCTL synchronise file on the disk */
#define FS_IOCTL_TRANSFER_DATA      53u     /**< FS IOCTL transfer data from the current file to another one */

#define GPIO_IOCTL_TOGGLE           100u    /**< GPIO IOCTL toggle GPIO */

#define UART_IOCTL_START_RX         110u    /**< UART IO CTL start DMA or IT tx */
#define UART_IOCTL_START_TX         111u    /**< UART IO CTL start DMA or IT rx */
#define UART_IOCTL_CHECK_RX_ENDED   112u    /**< UART IO CTL verify if DMA or IT rx ended */
#define UART_IOCTL_CHECK_TX_ENDED   113u    /**< UART IO CTL verify if DMA or IT tx ended */

#define OW_IOCTL_INIT_CONNECTION    120u    /**< OW IO CTL init connection */

/************************************/
/************ SYSCALLS NO ***********/
/************************************/

#define SYSCALL_CHECK_ERROR         1u      /**< Syscall no for CheckError */
#define SYSCALL_SLEEP               2u      /**< Syscall no for Sleep */
#define SYSCALL_SLEEP_PERIODIC      3u      /**< Syscall no for SleepPeriodic */
#define SYSCALL_GET_TICK            4u      /**< Syscall no for GetTick */
#define SYSCALL_GET_TIME            5u      /**< Syscall no for GetTime */
#define SYSCALL_SET_TIME            6u      /**< Syscall no for SetTime */
#define SYSCALL_DEVICE_OPEN         7u      /**< Syscall no for DeviceOpen */
#define SYSCALL_DEVICE_WRITE        8u      /**< Syscall no for DeviceWrite */
#define SYSCALL_DEVICE_READ         9u      /**< Syscall no for DeviceRead */
#define SYSCALL_DEVICE_IOCTL        10u     /**< Syscall no for DeviceIoctl */
#define SYSCALL_DEVICE_CLOSE        11u     /**< Syscall no for DeviceClose */
#define SYSCALL_GET_CURRENT_TASK    12u     /**< Syscall no for GetCurrentTask */
#define SYSCALL_SUSPEND_TASK        13u     /**< Syscall no for SuspendTask */
#define SYSCALL_RESUME_TASK         14u     /**< Syscall no for ResumeTask */
#define SYSCALL_GET_TASK_PRIORITY   15u     /**< Syscall no for GetTaskPriority */
#define SYSCALL_SET_TASK_PRIORITY   16u     /**< Syscall no for SetTaskPriority */
#define SYSCALL_ACQUIRE_MUTEX       17u     /**< Syscall no for AcquireMutex */
#define SYSCALL_RELEASE_MUTEX       18u     /**< Syscall no for ReleaseMutex */
#define SYSCALL_CONSOLE_PRINT       19u     /**< Syscall no for ConsolePrint */
#define SYSCALL_ENABLE_HK           20u     /**< Syscall no for EnableHK */
#define SYSCALL_DISABLE_HK          21u     /**< Syscall no for DisableHK */
#define SYSCALL_EMIT_HK             22u     /**< Syscall no for EmitHK */
#define SYSCALL_COLLECT_HKS         23u     /**< Syscall no for CollectHKs */

#endif /* KERNEL_CONSTANTS_H */

/**
 * @}
 */