RAM_SIZE = 0x400000;      
__heap_size = 0x50000;
/*STACK_SIZE = 0x1000; /*DEFINED(__stack_size__) ? __stack_size__ : */
MEMORY
{
    MEM (rwx)   : ORIGIN = 0x00000000, LENGTH = RAM_SIZE
}
PROVIDE(__ram_size = RAM_SIZE);
SECTIONS
{
    .text : ALIGN(4)
    {
        . = ALIGN(4);
        *boot*.o*(.text)
        *boot*.o*(.rodata)

        . = 0x50;
        KEEP(*(.irqen))
        . = 0x100;
        KEEP(*(.irqhandler))
        _text = .;
        *(.text .text.* .gnu.linkonce.t.*)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        /**(.text*)*/
        
        _sfixed = .;

        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
          KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        . = ALIGN(4);
        KEEP(*(.fini))

        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

        /* C++ static constructors */
        . = ALIGN(4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        /* C++ static destructors */
        . = ALIGN(4);
        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))

        . = ALIGN(4);
        PROVIDE (__etext = .);
        PROVIDE (_etext = .);
        PROVIDE (etext = .);
    } > MEM
    .rodata __etext :
    {
        /* constant data like strings */
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        *(.rodata1)

        /* finish section on WORD boundary */
        . = ALIGN(4);
        __RODATA_END__ = .;
    } > MEM
    .data : ALIGN(4)
    {
        __DATA_BEGIN__ = .;
        __SDATA_BEGIN__ = .;
        *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
        *(.data1)
        *(.data .data.* .gnu.linkonce.d.*)
        SORT(CONSTRUCTORS)

        *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) *(.data.rel.ro .data.rel.ro.* .gnu.linkonce.d.rel.ro.*)
        *(.dynamic)

        /* We want the small data sections together, so single-instruction offsets
        can access them all, and initialized data all before uninitialized, so
        we can shorten the on-disk segment size.  */

        *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata .srodata.*)
        *(.sdata .sdata.* .gnu.linkonce.s.*)

        PROVIDE_HIDDEN (__tdata_start = .);
        *(.tdata .tdata.* .gnu.linkonce.td.*)

        /* finish section on WORD boundary */
        . = ALIGN(4);
        _edata = .;
        PROVIDE (edata = .);
        __DATA_END__ = .;
        _global = __DATA_END__ + 0x800;
    } > MEM

    .bss (NOLOAD): ALIGN(4)
    {
        __BSS_START__ = .;
        *(.dynsbss)
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
        *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon)
        *(.scommon)
        *(.dynbss)
        *(.bss .bss.* .gnu.linkonce.b.*)

        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        *(COMMON)
        /* Align here to ensure that the .bss section occupies space up to
        _end.  Align after .bss to ensure correct alignment even if the
        .bss section disappears because there are no input sections.
        FIXME: Why do we need it? When there is no .bss section, we do not
        pad the .data section.  */
        . = ALIGN(. != 0 ? 32 / 8 : 1);

        /* finish section on WORD boundary */
        . = ALIGN(4);
        __BSS_END__ = .;
        _end = .; PROVIDE (end = .);
    } > MEM
    /*
    .stack :
    {
        . = ALIGN(4);
        _sstack = .;
        . = . + STACK_SIZE;
        . = ALIGN(4);
        _estack = .;
    } > MEM
    PROVIDE ( _stack = _sstack );
    */
    PROVIDE(stack_begin = (ORIGIN(MEM) + LENGTH(MEM)) - 0x1004);

    .heap : ALIGN(8)
    {
        PROVIDE(__heap_start = .);
        . = . + __heap_size;
        /*. = ALIGN(4);*/
        PROVIDE(__heap_end = .);
    } > MEM


}
