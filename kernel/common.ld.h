/* BSP contained sections */
#define BSP_TEXT_SEGMENT    . = ALIGN(4); \
                            _bsp_text_start_ = .; \
                            *libbsp*.a:*(.text .text.*) \
                            . = ALIGN(4); \
                            _bsp_text_end_ = .;

#define BSP_RODATA_SEGMENT    . = ALIGN(4); \
                            _bsp_rodata_start_ = .; \
                            *libbsp*.a:*(.rodata .rodata.*) \
                            . = ALIGN(4); \
                            _bsp_rodata_end_ = .;

#define BSP_DATA_SEGMENT    . = ALIGN(4); \
                            _bsp_data_start_ = .; \
                            *libbsp*.a:*(.data .data.*) \
                            . = ALIGN(4); \
                            _bsp_data_end_ = .;

#define BSP_BSS_SEGMENT    . = ALIGN(4); \
                            _bsp_bss_start_ = .; \
                            *libbsp*.a:*(.bss .bss.*) \
                            . = ALIGN(4); \
                            _bsp_bss_end_ = .;

/* HAL contained sections */

#define HAL_TEXT_SEGMENT    . = ALIGN(4); \
                            _hal_text_start_ = .; \
                            *libhal*.a:*(.text .text.*) \
                            . = ALIGN(4); \
                            _hal_text_end_ = .;

#define HAL_RODATA_SEGMENT  . = ALIGN(4); \
                            _hal_rodata_start_ = .; \
                            *libhal*.a:*(.rodata .rodata.*) \
                            . = ALIGN(4); \
                            _hal_rodata_end_ = .;

#define HAL_DATA_SEGMENT    . = ALIGN(4); \
                            _hal_data_start_ = .; \
                            *libhal*.a:*(.data .data.*) \
                            . = ALIGN(4); \
                            _hal_data_end_ = .;

#define HAL_BSS_SEGMENT     . = ALIGN(4); \
                            _hal_bss_start_ = .; \
                            *libhal*.a:*(.bss .bss.*) \
                            . = ALIGN(4); \
                            _hal_bss_end_ = .;

/* FATFS contained sections */

#define FATFS_TEXT_SEGMENT      . = ALIGN(4); \
                                _fatfs_text_start_ = .; \
                                *libfatfs*.a:*(.text .text.*) \
                                . = ALIGN(4); \
                                _fatfs_text_end_ = .;

#define FATFS_RODATA_SEGMENT    . = ALIGN(4); \
                                _fatfs_rodata_start_ = .; \
                                *libfatfs*.a:*(.rodata .rodata.*) \
                                . = ALIGN(4); \
                                _fatfs_rodata_end_ = .;

#define FATFS_DATA_SEGMENT      . = ALIGN(4); \
                                _fatfs_data_start_ = .; \
                                *libfatfs*.a:*(.data .data.*) \
                                . = ALIGN(4); \
                                _fatfs_data_end_ = .;

#define FATFS_BSS_SEGMENT       . = ALIGN(4); \
                                _fatfs_bss_start_ = .; \
                                *libfatfs*.a:*(.bss .bss.*) \
                                . = ALIGN(4); \
                                _fatfs_bss_end_ = .;