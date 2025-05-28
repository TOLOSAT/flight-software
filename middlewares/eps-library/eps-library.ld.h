/* CSP contained sections */
#define CSP_TEXT_SEGMENT    . = ALIGN(4); \
                            __csp_text_start__ = .; \
                            *libcsp*.a:*(.text .text.*) \
                            . = ALIGN(4); \
                            __csp_text_end__ = .;

#define CSP_RODATA_SEGMENT  . = ALIGN(4); \
                            __csp_rodata_start__ = .; \
                            *libcsp*.a:*(.rodata .rodata.*) \
                            . = ALIGN(4); \
                            __csp_rodata_end__ = .;

#define CSP_DATA_SEGMENT    . = ALIGN(4); \
                            __csp_data_start__ = .; \
                            *libcsp*.a:*(.data .data.*) \
                            . = ALIGN(4); \
                            __csp_data_end__ = .;

#define CSP_BSS_SEGMENT    . = ALIGN(4); \
                            __csp_bss_start__ = .; \
                            *libcsp*.a:*(.bss .bss.*) \
                            . = ALIGN(4); \
                            __csp_bss_end__ = .;