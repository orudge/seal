OUTPUT_FORMAT("coff-go32")

FORCE_COMMON_ALLOCATION

SECTIONS {
  .text : {
    *(.text)
    *(.data)
    *(.bss)
    *(COMMON)
  }
}
