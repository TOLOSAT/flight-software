/**
 * @file    mpu.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for MPU functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "system/mpu.h"
#include "bsp.h"

#if defined(CONFIG_MPU)

/***************************** Macros Definitions ****************************/

#define DEFAULT_REGION_NO                    0u                      /**< Default region numero */
#define DEFAULT_REGION_BASE_ADDR             0x00000000u             /**< Default region base address */
#define DEFAULT_REGION_SIZE                  ARM_MPU_REGION_SIZE_4GB /**< Default region size */

#define KERNEL_TEXT_REGION_NO                1u                               /**< Kernel text region numero */
#define KERNEL_TEXT_REGION_BASE_ADDR         (uint32_t)&__kernel_text_start__ /**< Kernel text region base address */
#define KERNEL_TEXT_REGION_SIZE              ARM_MPU_REGION_SIZE_64KB         /**< Kernel text region size */

#define KERNEL_DATA_REGION_NO                2u                               /**< Kernel data region numero */
#define KERNEL_DATA_REGION_BASE_ADDR         (uint32_t)&__kernel_data_start__ /**< Kernel data region base address */
#define KERNEL_DATA_REGION_SIZE              ARM_MPU_REGION_SIZE_128KB        /**< Kernel data region size */

#define KERNEL_RODATA_REGION_NO              3u                                 /**< Kernel rodata region numero */
#define KERNEL_RODATA_REGION_BASE_ADDR       (uint32_t)&__kernel_rodata_start__ /**< Kernel rodata region base address */
#define KERNEL_RODATA_REGION_SIZE            ARM_MPU_REGION_SIZE_8KB            /**< Kernel rodata region size */

#define DMABUFF_REGION_NO                    4u                           /**< DMA buffer region numero */
#define DMABUFF_REGION_BASE_ADDR             (uint32_t)&__dmabuff_start__ /**< DMA buffer region base address */
#define DMABUFF_REGION_SIZE                  ARM_MPU_REGION_SIZE_32KB     /**< DMA buffer region size */

#define PERIPHERALS_REGION_NO                5u                        /**< Peripherals region numero */
#define PERIPHERALS_REGION_BASE_ADDR         0x40000000u               /**< Peripherals region base address */
#define PERIPHERALS_REGION_SIZE              ARM_MPU_REGION_SIZE_512MB /**< Peripherals region size */

#define PRIVATE_PERIPHERALS_REGION_NO        6u                      /**< Private peripherals region numero */
#define PRIVATE_PERIPHERALS_REGION_BASE_ADDR 0xE0000000u             /**< Private peripherals region base address */
#define PRIVATE_PERIPHERALS_REGION_SIZE      ARM_MPU_REGION_SIZE_1MB /**< Private peripherals region size */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

extern uint32_t __kernel_text_start__;
extern uint32_t __kernel_rodata_start__;
extern uint32_t __kernel_data_start__;
extern uint32_t __dmabuff_start__;

/*************************** Functions Definitions ***************************/

/**
 * @fn     InitMPU(void)
 * @brief  Function that initialises Memory Protection Unit
 *
 * The default cache and shareability strategies are :
 * - For normal memories : non-shareable and cacheable with write back and write and read allocate (i.e. TEX=001, C=1, B=1, S=0)
 * - For devices : shareable and not cacheable (i.e. TEX=000, C=0, B=1, S=1)
 * - For dma buffers : shareable and not cacheable (i.e. TEX=001, C=0, B=0, S=1)
 */
void InitMPU(void)
{
    uint32_t rbar = 0u;
    uint32_t rasr = 0u;

    // First disable MPU (in case it has been enabled before)
    ARM_MPU_Disable();

    // Set background region
    rbar = ARM_MPU_RBAR(DEFAULT_REGION_NO, DEFAULT_REGION_BASE_ADDR);
    rasr = ARM_MPU_RASR_EX(0,               // DisableExec: 0 (executable)
                           ARM_MPU_AP_FULL, // AccessPermission: full access (read/write for privileged and non-privileged)
                           ARM_MPU_ACCESS_( // AccessAttribute: normal memory, cacheable write-back and write and read allocate, non-shareable
                               1,           // (tex:0b001)
                               0,           // (non-shareable)
                               1,           // (cacheable)
                               1            // (bufferable)
                               ),
                           0,                  // SubRegionDisable: 0 (no sub-region disabled)
                           DEFAULT_REGION_SIZE // Region size
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect kernel text region
    rbar = ARM_MPU_RBAR(KERNEL_TEXT_REGION_NO, KERNEL_TEXT_REGION_BASE_ADDR); // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to be addressed
    rasr = ARM_MPU_RASR_EX(0,                                                 // DisableExec: 0 (executable)
                           ARM_MPU_AP_PRO,                                    // AccessPermission: read-only access for privileged only
                           ARM_MPU_ACCESS_( // AccessAttribute: normal memory, cacheable write-back and write and read allocate, non-shareable
                               1,           // (tex:0b001)
                               0,           // (non-shareable)
                               1,           // (cacheable)
                               1            // (bufferable)
                               ),
                           0,                      // SubRegionDisable: 0 (no sub-region disabled)
                           KERNEL_TEXT_REGION_SIZE // Region size
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect kernel data region
    rbar = ARM_MPU_RBAR(KERNEL_DATA_REGION_NO, KERNEL_DATA_REGION_BASE_ADDR); // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to be addressed
    rasr = ARM_MPU_RASR_EX(1,                                                 // DisableExec: 1 (not executable)
                           ARM_MPU_AP_PRIV,                                   // AccessPermission: read-write access for privileged only
                           ARM_MPU_ACCESS_( // AccessAttribute: normal memory, cacheable write-back and write and read allocate, non-shareable
                               1,           // (tex:0b001)
                               0,           // (non-shareable)
                               1,           // (cacheable)
                               1            // (bufferable)
                               ),
                           0,                      // SubRegionDisable: 0 (no sub-region disabled)
                           KERNEL_DATA_REGION_SIZE // Region size
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect kernel rodata region
    rbar = ARM_MPU_RBAR(KERNEL_RODATA_REGION_NO, KERNEL_RODATA_REGION_BASE_ADDR); // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to be addressed
    rasr = ARM_MPU_RASR_EX(1,                                                     // DisableExec: 1 (not executable)
                           ARM_MPU_AP_PRO,                                        // AccessPermission: read-only access for privileged only
                           ARM_MPU_ACCESS_( // AccessAttribute: normal memory, cacheable write-back and write and read allocate, non-shareable
                               1,           // (tex:0b001)
                               0,           // (non-shareable)
                               1,           // (cacheable)
                               1            // (bufferable)
                               ),
                           0,                        // SubRegionDisable: 0 (no sub-region disabled)
                           KERNEL_RODATA_REGION_SIZE // Region size
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect DMABUFF region
    rbar = ARM_MPU_RBAR(DMABUFF_REGION_NO, DMABUFF_REGION_BASE_ADDR); // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to be addressed
    rasr = ARM_MPU_RASR_EX(1,                                         // DisableExec: 1 (not executable)
                           ARM_MPU_AP_FULL,                           // AccessPermission: full access (read/write for privileged and non-privileged)
                           ARM_MPU_ACCESS_(                           // AccessAttribute: normal memory, noncacheable, shareable
                               1,                                     // (tex:0b001)
                               1,                                     // (shareable)
                               0,                                     // (not cacheable)
                               0                                      // (not bufferable)
                               ),
                           0,                  // SubRegionDisable: 0 (no sub-region disabled)
                           DMABUFF_REGION_SIZE // Region size
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect peripherals region
    rbar = ARM_MPU_RBAR(PERIPHERALS_REGION_NO, PERIPHERALS_REGION_BASE_ADDR);
    rasr = ARM_MPU_RASR_EX(1,               // DisableExec: 1 (not executable)
                           ARM_MPU_AP_PRIV, // AccessPermission: read/write access for privileged only
                           ARM_MPU_ACCESS_( // AccessAttribute: device memory, noncacheable, shareable
                               0,           // (tex:0b000)
                               1,           // (shareable)
                               0,           // (not cacheable)
                               1            // (bufferable)
                               ),
                           0,                      // SubRegionDisable: 0 (no sub-region disabled)
                           PERIPHERALS_REGION_SIZE // Region size
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect private peripherals region
    rbar = ARM_MPU_RBAR(PRIVATE_PERIPHERALS_REGION_NO, PRIVATE_PERIPHERALS_REGION_BASE_ADDR);
    rasr = ARM_MPU_RASR_EX(1,               // DisableExec: 1 (not executable)
                           ARM_MPU_AP_PRIV, // AccessPermission: read/write access for privileged only
                           ARM_MPU_ACCESS_( // AccessAttribute: strongly ordered memory, noncacheable, shareable
                               0,           // (tex:0b000)
                               1,           // (shareable)
                               0,           // (not cacheable)
                               0            // (bufferable)
                               ),
                           0,                              // SubRegionDisable: 0 (no sub-region disabled)
                           PRIVATE_PERIPHERALS_REGION_SIZE // Region size
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Finally enable the MPU
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);
}

#else
/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn     InitMPU(void)
 * @brief  Function that initialises Memory Protection Unit
 */
void InitMPU(void)
{
    // Nothing to do
}

#endif