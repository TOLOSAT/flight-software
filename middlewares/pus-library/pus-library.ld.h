/* PUS contained sections */
#define PUS_TEXT_SEGMENT    . = ALIGN(4); \
                            _text_pus_start_ = .; \
                            *libpus*.a:*(.text .text.*) \
                            . = ALIGN(4); \
                            _text_pus_end_ = .;

#define PUS_DATA_SEGMENT    . = ALIGN(4); \
                            _data_pus_start_ = .; \
                            *libpus*.a:*(.data .data.* .bss .bss.*) \
                            . = ALIGN(4); \
                            _data_pus_end_ = .;
