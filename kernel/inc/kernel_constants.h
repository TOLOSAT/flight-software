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

/*************************** Constants Definitions ***************************/

/************************************/
/********* GENERAL CONSTANTS ********/
/************************************/

#define NO_BUFFER                   (-1u)   /**< Reference number to refer for no buffer */
#define NO_DEVICE                   (-1u)   /**< Device numero for no device */
#define ANY_TASK                    (-1u)   /**< Reference number to refer to any task */

#define DEVICE_NO_EXTRA_INFO        0u      /**< Constant to inform there is no extra data */

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

#define FS_IOCTL_GET_SIZE           10u     /**< Get file size */
#define FS_IOCTL_SEEK               11u     /**< Moves read/write pointer in the file */
#define FS_IOCTL_SYNC               12u     /**< Synchronise file on the disk */
#define FS_IOCTL_TRANSFER_DATA      13u     /**< Transfer data from the current file to another one */

#define GPIO_IOCTL_TOGGLE           20u      /**< GPIO IOCTL toggle GPIO */

#define UART_IOCTL_START_RX         30u      /**< UART IO CTL start DMA or IT tx */
#define UART_IOCTL_START_TX         31u      /**< UART IO CTL start DMA or IT rx */
#define UART_IOCTL_CHECK_RX_ENDED   32u      /**< UART IO CTL verify if DMA or IT rx ended */
#define UART_IOCTL_CHECK_TX_ENDED   33u      /**< UART IO CTL verify if DMA or IT tx ended */

#define OW_IOCTL_INIT_CONNECTION    40u      /**< OW IO CTL init connection */

#endif /* KERNEL_CONSTANTS_H */

/**
 * @}
 */