OUTPUT_FORMAT("coff-go32")

FORCE_COMMON_ALLOCATION

SECTIONS {
  .text : {
    *(.text)
    dlx_first_ctor = . ;
    *(.ctor)
    dlx_last_ctor = . ;
    dlx_first_dtor = . ;
    *(.dtor)
    dlx_last_dtor = . ;
    *(.gcc_exc*)
    ___EH_FRAME_BEGIN__ = . ;
    *(.eh_fram*)
    ___EH_FRAME_END__ = . ;
    *(.data)
    *(.bss)
    *(COMMON)
  }
}
