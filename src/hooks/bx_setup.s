        .thumb
        .align 2
        .global bx_setup_hook

bx_setup_hook:
        ldr r0, =multi_bx_partner_init
        str r0, [r2, #8]
        mov r0, #2
        strb r0, [r1, #2]
        str r3, [r2, #0xC]
        ldr r0, =0x0800D470|1
        bx r0
