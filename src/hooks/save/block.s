        .align 2
        .global saveblock_init_pointers_hook

saveblock_init_pointers_hook:
        strb r4, [r0]

        @@ Initialise the extra saveblock pointer
        ldr r0, =saveblock4
        ldr r1, =saveblock4_data
        str r1, [r0]
        pop {r4, pc}
