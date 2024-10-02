/* PUS contained sections */
#define PUS_TEXT_SEGMENT    . = ALIGN(4); \
                            _pus_text_start_ = .; \
                            *libpus*.a:*(.text .text.*) \
                            . = ALIGN(4); \
                            _pus_text_end_ = .;

#define PUS_DATA_SEGMENT    . = ALIGN(4); \
                            _pus_data_start_ = .; \
                            *libpus*.a:*(.data .data.*) \
                            . = ALIGN(4); \
                            _pus_data_end_ = .;

#define PUS_BSS_SEGMENT    . = ALIGN(4); \
                            _pus_bss_start_ = .; \
                            *libpus*.a:*(.bss .bss.*) \
                            . = ALIGN(4); \
                            _pus_bss_end_ = .;