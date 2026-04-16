.section .header, "a"
.align 4
.space 0x10

.section .text.startup, "ax"
.global _start

_start:
    adrp x3, __bss_start
    add  x3, x3, :lo12:__bss_start

    adrp x1, __bss_end
    add  x1, x1, :lo12:__bss_end
    mov     w2, #0

bss_clear_loop:
    cmp     x3, x1
    b.eq    bss_done
    str     w2, [x3], #4
    b       bss_clear_loop

bss_done:
    adrp x0, __bss_start
    add  x0, x0, :lo12:__bss_start
    bl      main

hang:
    b hang
