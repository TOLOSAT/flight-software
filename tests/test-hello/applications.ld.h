/* Application contained sections */
#define APP_TEXT_SEGMENT    . = ALIGN(4); \
                            _application_text_start_ = .; \
                            *libapplication*.a:*(.text .text.*) \
                            . = ALIGN(4); \
                            _application_text_end_ = .;

#define APP_RODATA_SEGMENT  . = ALIGN(4); \
                            _application_rodata_start_ = .; \
                            *libapplication*.a:*(.rodata .rodata.*) \
                            . = ALIGN(4); \
                            _application_rodata_end_ = .;

#define APP_DATA_SEGMENT    . = ALIGN(4); \
                            _application_data_start_ = .; \
                            *libapplication*.a:*(.data .data.*) \
                            . = ALIGN(4); \
                            _application_data_end_ = .;

#define APP_BSS_SEGMENT     . = ALIGN(4); \
                            _application_bss_start_ = .; \
                            *libapplication*.a:*(.bss .bss.*) \
                            . = ALIGN(4); \
                            _application_bss_end_ = .;
