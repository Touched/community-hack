        // Hook after the battle clean up
        .org 0x0802E200
        ldr r1, =battle_extension_free_hook|1
        bx r1
        .pool

        // Hook the battle malloc
        .org 0x0802E040
        bx r4
        .org 0x0802E118
        .word battle_extension_malloc_hook|1
        