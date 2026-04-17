ENTRY(_start)

SECTIONS
{
    . = 0x02022000;

    /DISCARD/ : {
        *(.interp)
        *(.note*)
        *(.comment)
        *(.dynamic)
        *(.dynsym)
        *(.dynstr)
        *(.rela*)
        *(.rela.*)
        *(.gnu*)
    }

    .text : {
        *(.text.startup*)
        *(.text*)
    }

    .rodata : { *(.rodata*) }
    .data   : { *(.data*) }

    .bss (NOLOAD) : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }
}
