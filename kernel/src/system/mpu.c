/**
 * @file    mpu.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for MPU functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/mpu.h"
#include "bsp.h"

#if defined(CONFIG_MPU)

/***************************** Macros Definitions ****************************/

/* -------------------------------------------------------------------------- */
/* Default Region                                                             */
/* Full access, Normal memory, cacheable (write-back, write & read allocate), */
/* non-shareable, executable                                                  */
/* -------------------------------------------------------------------------- */
#define DEFAULT_REGION_NO                    0u                      /**< Default region numero */
#define DEFAULT_REGION_BASE_ADDR             0x00000000u             /**< Default region base address */
#define DEFAULT_REGION_SIZE                  ARM_MPU_REGION_SIZE_4GB /**< Default region size : 4GB */
#define DEFAULT_REGION_XN                    0                       /**< Default region access permission : Execute */
#define DEFAULT_REGION_AP                    ARM_MPU_AP_FULL         /**< Default region access permission : full access */
#define DEFAULT_REGION_TEX                   0b001                   /**< Default region TEX : 0b001  */
#define DEFAULT_REGION_S                     0                       /**< Default region shareability : non shareable */
#define DEFAULT_REGION_C                     1                       /**< Default region cacheability : cacheable */
#define DEFAULT_REGION_B                     1                       /**< Default region bufferability : bufferable */

/* -------------------------------------------------------------------------- */
/* Kernel Text Region                                                         */
/* Privileged read-only, Normal memory, cacheable (write-back, write & read   */
/* allocate), non-shareable, executable                                       */
/* -------------------------------------------------------------------------- */
#define KERNEL_TEXT_REGION_NO                1u                               /**< Kernel text region numero */
#define KERNEL_TEXT_REGION_BASE_ADDR         (uint32_t)&__kernel_text_start__ /**< Kernel text region base address */
#define KERNEL_TEXT_REGION_SIZE              ARM_MPU_REGION_SIZE_64KB         /**< Kernel text region size */
#define KERNEL_TEXT_REGION_XN                0                                /**< Kernel text region execute permission : executable */
#define KERNEL_TEXT_REGION_AP                ARM_MPU_AP_PRO                   /**< Kernel text region access permission : privileged read-only */
#define KERNEL_TEXT_REGION_TEX               0b001                            /**< Kernel text region TEX : 0b001 (Normal, WBWA) */
#define KERNEL_TEXT_REGION_S                 0                                /**< Kernel text region shareability : non shareable */
#define KERNEL_TEXT_REGION_C                 1                                /**< Kernel text region cacheability : cacheable */
#define KERNEL_TEXT_REGION_B                 1                                /**< Kernel text region bufferability : bufferable */

/* -------------------------------------------------------------------------- */
/* Kernel Data Region                                                         */
/* Privileged read/write, Normal memory, cacheable (write-back, write & read  */
/* allocate), non-shareable, execute-never                                    */
/* -------------------------------------------------------------------------- */
#define KERNEL_DATA_REGION_NO                2u                               /**< Kernel data region numero */
#define KERNEL_DATA_REGION_BASE_ADDR         (uint32_t)&__kernel_data_start__ /**< Kernel data region base address */
#define KERNEL_DATA_REGION_SIZE              ARM_MPU_REGION_SIZE_128KB        /**< Kernel data region size */
#define KERNEL_DATA_REGION_XN                1                                /**< Kernel data region execute permission : execute never */
#define KERNEL_DATA_REGION_AP                ARM_MPU_AP_PRIV                  /**< Kernel data region access permission : privileged read/write */
#define KERNEL_DATA_REGION_TEX               0b001                            /**< Kernel data region TEX : 0b001 (Normal, WBWA) */
#define KERNEL_DATA_REGION_S                 0                                /**< Kernel data region shareability : non shareable */
#define KERNEL_DATA_REGION_C                 1                                /**< Kernel data region cacheability : cacheable */
#define KERNEL_DATA_REGION_B                 1                                /**< Kernel data region bufferability : bufferable */

/* -------------------------------------------------------------------------- */
/* Kernel Rodata Region                                                       */
/* Privileged read-only, Normal memory, cacheable (write-back, write & read   */
/* allocate), non-shareable, execute-never                                    */
/* -------------------------------------------------------------------------- */
#define KERNEL_RODATA_REGION_NO              3u                                 /**< Kernel rodata region numero */
#define KERNEL_RODATA_REGION_BASE_ADDR       (uint32_t)&__kernel_rodata_start__ /**< Kernel rodata region base address */
#define KERNEL_RODATA_REGION_SIZE            ARM_MPU_REGION_SIZE_8KB            /**< Kernel rodata region size */
#define KERNEL_RODATA_REGION_XN              1                                  /**< Kernel rodata region execute permission : execute never */
#define KERNEL_RODATA_REGION_AP              ARM_MPU_AP_PRO                     /**< Kernel rodata region access permission : privileged read-only */
#define KERNEL_RODATA_REGION_TEX             0b001                              /**< Kernel rodata region TEX : 0b001 (Normal, WBWA) */
#define KERNEL_RODATA_REGION_S               0                                  /**< Kernel rodata region shareability : non shareable */
#define KERNEL_RODATA_REGION_C               1                                  /**< Kernel rodata region cacheability : cacheable */
#define KERNEL_RODATA_REGION_B               1                                  /**< Kernel rodata region bufferability : bufferable */

/* -------------------------------------------------------------------------- */
/* DMA Buffers Region                                                         */
/* Full access, Normal memory, non-cacheable, shareable, execute-never        */
/* -------------------------------------------------------------------------- */
#define DMABUFF_REGION_NO                    4u                           /**< DMA buffer region numero */
#define DMABUFF_REGION_BASE_ADDR             (uint32_t)&__dmabuff_start__ /**< DMA buffer region base address */
#define DMABUFF_REGION_SIZE                  ARM_MPU_REGION_SIZE_32KB     /**< DMA buffer region size */
#define DMABUFF_REGION_XN                    1                            /**< DMA buffer region execute permission : execute never */
#define DMABUFF_REGION_AP                    ARM_MPU_AP_FULL              /**< DMA buffer region access permission : full access */
#define DMABUFF_REGION_TEX                   0b001                        /**< DMA buffer region TEX : 0b001 (Normal memory) */
#define DMABUFF_REGION_S                     1                            /**< DMA buffer region shareability : shareable */
#define DMABUFF_REGION_C                     0                            /**< DMA buffer region cacheability : non cacheable */
#define DMABUFF_REGION_B                     0                            /**< DMA buffer region bufferability : non bufferable */

/* -------------------------------------------------------------------------- */
/* Peripherals Region                                                         */
/* Privileged read/write, Device memory, shareable, non-cacheable,            */
/* bufferable, execute-never                                                   */
/* -------------------------------------------------------------------------- */
#define PERIPHERALS_REGION_NO                5u                        /**< Peripherals region numero */
#define PERIPHERALS_REGION_BASE_ADDR         0x40000000u               /**< Peripherals region base address */
#define PERIPHERALS_REGION_SIZE              ARM_MPU_REGION_SIZE_512MB /**< Peripherals region size */
#define PERIPHERALS_REGION_XN                1                         /**< Peripherals region execute permission : execute never */
#define PERIPHERALS_REGION_AP                ARM_MPU_AP_PRIV           /**< Peripherals region access permission : privileged read/write */
#define PERIPHERALS_REGION_TEX               0b000                     /**< Peripherals region TEX : 0b000 (Device memory) */
#define PERIPHERALS_REGION_S                 1                         /**< Peripherals region shareability : shareable */
#define PERIPHERALS_REGION_C                 0                         /**< Peripherals region cacheability : non cacheable */
#define PERIPHERALS_REGION_B                 1                         /**< Peripherals region bufferability : bufferable */

/* -------------------------------------------------------------------------- */
/* External Memories Region (NAND via FMC)                                    */
/* Privileged read/write, Device memory, non-shareable, non-cacheable,        */
/* non-bufferable, execute-never                                               */
/* -------------------------------------------------------------------------- */
#define NAND_REGION_NO                       6u                        /**< External memories region numero */
#define NAND_REGION_BASE_ADDR                0x80000000u               /**< External memories region base address */
#define NAND_REGION_SIZE                     ARM_MPU_REGION_SIZE_256MB /**< External memories region size */
#define NAND_REGION_XN                       1                         /**< External memories region execute permission : execute never */
#define NAND_REGION_AP                       ARM_MPU_AP_PRIV           /**< External memories region access permission : privileged read/write */
#define NAND_REGION_TEX                      0b010                     /**< External memories region TEX : 0b010 (Device, non shareable) */
#define NAND_REGION_S                        0                         /**< External memories region shareability : non shareable */
#define NAND_REGION_C                        0                         /**< External memories region cacheability : non cacheable */
#define NAND_REGION_B                        0                         /**< External memories region bufferability : non bufferable */

/* -------------------------------------------------------------------------- */
/* Private Peripherals Region (System Control Space)                          */
/* Privileged read/write, Strongly ordered, shareable, non-cacheable,         */
/* non-bufferable, execute-never                                               */
/* -------------------------------------------------------------------------- */
#define PRIVATE_PERIPHERALS_REGION_NO        7u                      /**< Private peripherals region numero */
#define PRIVATE_PERIPHERALS_REGION_BASE_ADDR 0xE0000000u             /**< Private peripherals region base address */
#define PRIVATE_PERIPHERALS_REGION_SIZE      ARM_MPU_REGION_SIZE_1MB /**< Private peripherals region size */
#define PRIVATE_PERIPHERALS_REGION_XN        1                       /**< Private peripherals region execute permission : execute never */
#define PRIVATE_PERIPHERALS_REGION_AP        ARM_MPU_AP_PRIV         /**< Private peripherals region access permission : privileged read/write */
#define PRIVATE_PERIPHERALS_REGION_TEX       0b000                   /**< Private peripherals region TEX : 0b000 (Strongly ordered) */
#define PRIVATE_PERIPHERALS_REGION_S         1                       /**< Private peripherals region shareability : shareable */
#define PRIVATE_PERIPHERALS_REGION_C         0                       /**< Private peripherals region cacheability : non cacheable */
#define PRIVATE_PERIPHERALS_REGION_B         0                       /**< Private peripherals region bufferability : non bufferable */

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
    rasr = ARM_MPU_RASR_EX(DEFAULT_REGION_XN,      //
                           DEFAULT_REGION_AP,      //
                           ARM_MPU_ACCESS_(        //
                               DEFAULT_REGION_TEX, //
                               DEFAULT_REGION_S,   //
                               DEFAULT_REGION_C,   //
                               DEFAULT_REGION_B    //
                               ),                  //
                           0,                      //
                           DEFAULT_REGION_SIZE     //
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect kernel text region
    rbar = ARM_MPU_RBAR(KERNEL_TEXT_REGION_NO, KERNEL_TEXT_REGION_BASE_ADDR); // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to be
                                                                              // addressed
    rasr = ARM_MPU_RASR_EX(KERNEL_TEXT_REGION_XN,                             //
                           KERNEL_TEXT_REGION_AP,                             //
                           ARM_MPU_ACCESS_(                                   //
                               KERNEL_TEXT_REGION_TEX,                        //
                               KERNEL_TEXT_REGION_S,                          //
                               KERNEL_TEXT_REGION_C,                          //
                               KERNEL_TEXT_REGION_B                           //
                               ),                                             //
                           0,                                                 //
                           KERNEL_TEXT_REGION_SIZE                            //
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect kernel data region
    rbar = ARM_MPU_RBAR(KERNEL_DATA_REGION_NO, KERNEL_DATA_REGION_BASE_ADDR); // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to be
                                                                              // addressed
    rasr = ARM_MPU_RASR_EX(KERNEL_DATA_REGION_XN,                             //
                           KERNEL_DATA_REGION_AP,                             //
                           ARM_MPU_ACCESS_(                                   //
                               KERNEL_DATA_REGION_TEX,                        //
                               KERNEL_DATA_REGION_S,                          //
                               KERNEL_DATA_REGION_C,                          //
                               KERNEL_DATA_REGION_B                           //
                               ),                                             //
                           0,                                                 //
                           KERNEL_DATA_REGION_SIZE                            //
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect kernel rodata region
    rbar = ARM_MPU_RBAR(KERNEL_RODATA_REGION_NO, KERNEL_RODATA_REGION_BASE_ADDR); // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to be
                                                                                  // addressed
    rasr = ARM_MPU_RASR_EX(KERNEL_RODATA_REGION_XN,                               //
                           KERNEL_RODATA_REGION_AP,                               //
                           ARM_MPU_ACCESS_(                                       //
                               KERNEL_RODATA_REGION_TEX,                          //
                               KERNEL_RODATA_REGION_S,                            //
                               KERNEL_RODATA_REGION_C,                            //
                               KERNEL_RODATA_REGION_B                             //
                               ),                                                 //
                           0,                                                     //
                           KERNEL_RODATA_REGION_SIZE                              //
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect DMABUFF region
    rbar = ARM_MPU_RBAR(DMABUFF_REGION_NO, DMABUFF_REGION_BASE_ADDR); // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to be addressed
    rasr = ARM_MPU_RASR_EX(DMABUFF_REGION_XN,                         //
                           DMABUFF_REGION_AP,                         //
                           ARM_MPU_ACCESS_(                           //
                               DMABUFF_REGION_TEX,                    //
                               DMABUFF_REGION_S,                      //
                               DMABUFF_REGION_C,                      //
                               DMABUFF_REGION_B                       //
                               ),                                     //
                           0,                                         //
                           DMABUFF_REGION_SIZE                        //
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect peripherals region
    rbar = ARM_MPU_RBAR(PERIPHERALS_REGION_NO, PERIPHERALS_REGION_BASE_ADDR);
    rasr = ARM_MPU_RASR_EX(PERIPHERALS_REGION_XN,      //
                           PERIPHERALS_REGION_AP,      //
                           ARM_MPU_ACCESS_(            //
                               PERIPHERALS_REGION_TEX, //
                               PERIPHERALS_REGION_S,   //
                               PERIPHERALS_REGION_C,   //
                               PERIPHERALS_REGION_B    //
                               ),                      //
                           0,                          //
                           PERIPHERALS_REGION_SIZE     //
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // External memories region
    rbar = ARM_MPU_RBAR(NAND_REGION_NO, NAND_REGION_BASE_ADDR);
    rasr = ARM_MPU_RASR_EX(NAND_REGION_XN,      //
                           NAND_REGION_AP,      //
                           ARM_MPU_ACCESS_(     //
                               NAND_REGION_TEX, //
                               NAND_REGION_S,   //
                               NAND_REGION_C,   //
                               NAND_REGION_B    //
                               ),               //
                           0,                   //
                           NAND_REGION_SIZE     //
    );
    ARM_MPU_SetRegion(rbar, rasr);

    // Protect private peripherals region
    rbar = ARM_MPU_RBAR(PRIVATE_PERIPHERALS_REGION_NO, PRIVATE_PERIPHERALS_REGION_BASE_ADDR);
    rasr = ARM_MPU_RASR_EX(PRIVATE_PERIPHERALS_REGION_XN,      //
                           PRIVATE_PERIPHERALS_REGION_AP,      //
                           ARM_MPU_ACCESS_(                    //
                               PRIVATE_PERIPHERALS_REGION_TEX, //
                               PRIVATE_PERIPHERALS_REGION_S,   //
                               PRIVATE_PERIPHERALS_REGION_C,   //
                               PRIVATE_PERIPHERALS_REGION_B    //
                               ),                              //
                           0,                                  //
                           PRIVATE_PERIPHERALS_REGION_SIZE     //
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