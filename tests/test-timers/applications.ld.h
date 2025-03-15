/* Application sections */
#define APP_TEXT_SEGMENT    . = ALIGN(4); \
                            __application_text_start__ = .; \
                            *libapplication*.a:*(.text .text.*) \
                            . = ALIGN(4); \
                            __application_text_end__ = .;

#define APP_RODATA_SEGMENT  . = ALIGN(4); \
                            __application_rodata_start__ = .; \
                            *libapplication*.a:*(.rodata .rodata.*) \
                            . = ALIGN(4); \
                            __application_rodata_end__ = .;

#define APP_DATA_SEGMENT    . = ALIGN(4); \
                            __application_data_start__ = .; \
                            *libapplication*.a:*(.data .data.*) \
                            . = ALIGN(4); \
                            __application_data_end__ = .;

#define APP_BSS_SEGMENT     . = ALIGN(4); \
                            __application_bss_start__ = .; \
                            *libapplication*.a:*(.bss .bss.*) \
                            . = ALIGN(4); \
                            __application_bss_end__ = .;
