        .thumb
        .align 2

        .global battle_extension_free_hook
battle_extension_free_hook:
	bl battle_extension_free
        pop {r4-r6, pc}

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global battle_extension_malloc_hook
battle_extension_malloc_hook:
        bl battle_extension_malloc
        ldr r4, =battle_type_flags
        ldr r0, [r4]
        ldr r1, =0x0802E042|1
        bx r1

@@@ --------------------------------------------------------------------------
