.global _start

_start:
    // NOPs are needed, houston would jump to the header
    // as it expects payloads to be loaded in the RX base
    // but we can't just remove the header, since EL3_MON
    // freaks out for whatever reason.
    nop
    nop
    nop
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
    bl      main

hang:
    b hang
