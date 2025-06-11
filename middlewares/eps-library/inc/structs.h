/**
 * @file    eps.h
 * @author  Matteo Planchet
 * @brief   Header file for EPS structs
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup eps-library EPS
 * @brief Structs for EPS communication via CSP, coming from nanopower P31 data sheet.
 * @{
 */

#ifndef EPS_STRUCTS_H
#define EPS_STRUCTS_H

/******************************* Include Files *******************************/
#include <stdint.h>
/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

typedef struct
{
    uint16_t pv[3];         /**< @brief Photo voltaic input voltage [mV] */
    uint16_t pc;            /**< @brief Total photo current [mA] */
    uint16_t bv;            /**< @brief Battery voltage [mV] */
    uint16_t sc;            /**< @brief Total system current [mA] */
    int16_t temp[4];        /**< @brief Temp. of boost converters (1,2,3) and onboard battery [degC] */
    int16_t batt_temp[2];   /**< @brief External board battery temperatures [degC] */
    uint16_t latchup[6];    /**< @brief Number of latch-ups on each output 5V and +3V3 channel
                                        Order[5V1 5V2 5V3 3.3V1 3.3V2 3.3V3]
                                        Transmit as 5V1 first and 3.3V3 last */
    uint8_t reset;          /**< @brief Cause of last EPS reset */
    uint16_t bootcount;     /**< @brief Number of EPS reboots */
    uint16_t sw_errors;     /**< @brief Number of errors in the eps software */
    uint8_t ppt_mode;       /**< @brief PPT (Peak Power Tracking) mode: 0 = Hardware, 1 = MPPT, 2 = Fixed SW PPT. */
    uint8_t channel_status; /**< @brief Mask of output channel status (1 = on, 0 = off).
                                        MSB - [QH QS 3.3V3 3.3V2 3.3V1 5V3 5V2 5V1] - LSB
                                        QH = Quadbat heater, QS = Quadbat switch */
} hkparam_t;

typedef struct ATTR_PACKED
{
    uint16_t vboost[3];            /**< @brief Voltage of boost converters [mV] [PV1, PV2, PV3] */
    uint16_t vbatt;                /**< @brief Voltage of battery [mV] */
    uint16_t curin[3];             /**< @brief Current in [mA] */
    uint16_t cursun;               /**< @brief Current from boost converters */
    uint16_t cursys;               /**< @brief Current out of battery */
    uint16_t reserved1;            /**< @brief Reserved for future use */
    uint16_t curout[6];            /**< @brief Current out [mA] */
    uint8_t output[8];             /**< @brief Status of outputs */
    uint16_t output_on_delta[8];   /**< @brief Time till power on */
    uint16_t output_off_delta[8];  /**< @brief Time till power off */
    uint16_t latchup[6];           /**< @brief Number of latch-ups */
    uint32_t wdt_i2c_time_left;    /**< @brief Time left on I2C wdt */
    uint32_t wdt_gnd_time_left;    /**< @brief Time left on GND wdt */
    uint8_t wdt_csp_pings_left[2]; /**< @brief Pings left on CSP wdt */
    uint32_t counter_wdt_i2c;      /**< @brief Number of WDT I2C reboots */
    uint32_t counter_wdt_gnd;      /**< @brief Number of WDT GND reboots */
    uint32_t counter_wdt_csp[2];   /**< @brief Number of WDT CSP reboots */
    uint32_t counter_boot;         /**< @brief Number of EPS reboots */
    int16_t temp[6];               /**< @brief Temperature sensors [0 = TEMP1, TEMP2, TEMP3, TEMP4, BATT0, BATT1] */
    uint8_t bootcause;             /**< @brief Cause of last EPS reset */
    uint8_t battmode;              /**< @brief Mode for battery [0 = normal, 1 = undervoltage, 2 = overvoltage] */
    uint8_t pptmode;               /**< @brief Mode of PPT tracker */
    uint16_t reserved2;
} eps_hk_t;

typedef struct ATTR_PACKED
{
    uint16_t vboost[3]; /**< @brief Voltage of boost converters [mV] [PV1, PV2, PV3] */
    uint16_t vbatt;     /**< @brief Voltage of battery [mV] */
    uint16_t curin[3];  /**< @brief Current in [mA] */
    uint16_t cursun;    /**< @brief Current from boost converters */
    uint16_t cursys;    /**< @brief Current out of battery */
    uint16_t reserved1; /**< @brief Reserved for future use */
} eps_hk_vi_t;

typedef struct ATTR_PACKED
{
    uint16_t curout[6];           /**< @brief Current out [mA] */
    uint8_t output[8];            /**< @brief Status of outputs */
    uint16_t output_on_delta[8];  /**< @brief Time till power on */
    uint16_t output_off_delta[8]; /**< @brief Time till power off */
    uint16_t latchup[6];          /**< @brief Number of latch-ups */
} eps_hk_out_t;

typedef struct ATTR_PACKED
{
    uint32_t wdt_i2c_time_left;    /**< @brief Time left on I2C wdt */
    uint32_t wdt_gnd_time_left;    /**< @brief Time left on GND wdt */
    uint8_t wdt_csp_pings_left[2]; /**< @brief Pings left on CSP wdt */
    uint32_t counter_wdt_i2c;      /**< @brief Number of WDT I2C reboots */
    uint32_t counter_wdt_gnd;      /**< @brief Number of WDT GND reboots */
    uint32_t counter_wdt_csp[2];   /**< @brief Number of WDT CSP reboots */
} eps_hk_wdt_t;

typedef struct ATTR_PACKED
{
    uint32_t counter_boot; /**< @brief Number of EPS reboots */
    int16_t temp[6];       /**< @brief Temperature sensors [0 = TEMP1, TEMP2, TEMP3, TEMP4, BATT0, BATT1] */
    uint8_t bootcause;     /**< @brief Cause of last EPS reset */
    uint8_t battmode;      /**< @brief Mode for battery [0 = normal, 1 = undervoltage, 2 = overvoltage] */
    uint8_t pptmode;       /**< @brief Mode of PPT tracker */
    uint16_t reserved2;
} eps_hk_basic_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

#endif /* EPS_STRUCTS_H */

/**
 * @}
 * @}
 */