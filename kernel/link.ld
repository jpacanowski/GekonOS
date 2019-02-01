OUTPUT_FORMAT("binary")
SECTIONS
{
    .text 0x100000 :
    {
        /* KERNEL CODE SEGMENT */
        code = .; _code = .; __code = .;
        *(.text)
        . = ALIGN(4096);
    }
    .data :
    {
        /* KERNEL DATA SEGMENT */
        data = .; _data = .; __data = .;
        *(.data)
        . = ALIGN(4096);
    }
    .bss :
    {
        /* KERNEL BSS SEGMENT */
        bss = .; _bss = .; __bss = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4096);
    }
    end = .; _end = .; __end = .;
}