/**
 * @file    time_constants.h
 * @author  Merlin Kooshmanian
 * @brief   Header for time constants
 * @date    23/11/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TIME_CONSTANTS_H
#define TIME_CONSTANTS_H

/***************************** Macros Definitions ****************************/

/* CUC Constants */
#define CUC_HEAD_SIZE                   1u                                                  /**< CUC header size */
#define COARSE_TIME_SIZE                4u                                                  /**< Coarse time size */
#define FINE_TIME_SIZE                  1u                                                  /**< Fine time size */
#define CUC_TIME_SIZE                   (CUC_HEAD_SIZE + COARSE_TIME_SIZE + FINE_TIME_SIZE) /**< CUC time variables size */
#define TIME_HEAD_CONSTANT              0x1du                                               /**< P-field for CUC time (equivalent of 0b00011111u)*/
#define TAI_UNIX_OFFSET                 378691200u                                          /**< Number of seconds between TAI Ref (January 1rst 1958) and UNIX Ref (January 1rst 1970) */
#define CUC_TIME_STR_SIZE               (2*CUC_TIME_SIZE)                                   /**< Number of char needed to represent CUC time as a string (2 char are need to represent 1 uint8_t)*/

#endif /* TIME_CONSTANTS_H */
