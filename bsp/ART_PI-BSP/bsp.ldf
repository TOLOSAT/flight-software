/* Put section macro */
#ifndef PUT_SECTION
#define PUT_SECTION(section)    . = ALIGN(4); \
                                _##section##_start_ = .; \
                                *(.section) \
                                . = ALIGN(4); \
                                _##section##_end_ = .;
#endif

/* PUS contained sections */
#define BSP_TEXT_SEGMENT        PUT_SECTION(text_bsp)
#define BSP_DATA_SEGMENT        PUT_SECTION(data_bsp)

#define HAL_TEXT_SEGMENT        . = ALIGN(4); \
                                _text_hal_start_ = .; \
                                *libhal*.a:*(.text .text.*) \
                                . = ALIGN(4); \
                                _text_hal_end_ = .;

#define HAL_DATA_SEGMENT        . = ALIGN(4); \
                                _data_hal_start_ = .; \
                                *libhal*.a:*(.data .data.* .bss .bss.*) \
                                . = ALIGN(4); \
                                _data_hal_end_ = .;