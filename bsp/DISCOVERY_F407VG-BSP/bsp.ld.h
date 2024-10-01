/* BSP contained sections */
#define BSP_TEXT_SEGMENT    . = ALIGN(4); \
                            _text_bsp_start_ = .; \
                            *libbsp*.a:*(.text .text.*) \
                            . = ALIGN(4); \
                            _text_bsp_end_ = .;

#define BSP_DATA_SEGMENT    . = ALIGN(4); \
                            _data_bsp_start_ = .; \
                            *libbsp*.a:*(.data .data.* .bss .bss.*) \
                            . = ALIGN(4); \
                            _data_bsp_end_ = .;

#define HAL_TEXT_SEGMENT    . = ALIGN(4); \
                            _text_hal_start_ = .; \
                            *libhal*.a:*(.text .text.*) \
                            . = ALIGN(4); \
                            _text_hal_end_ = .;

#define HAL_DATA_SEGMENT    . = ALIGN(4); \
                            _data_hal_start_ = .; \
                            *libhal*.a:*(.data .data.* .bss .bss.*) \
                            . = ALIGN(4); \
                            _data_hal_end_ = .;
