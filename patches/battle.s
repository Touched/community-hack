        // Hook after the battle clean up
        .org 0x0802E200
        ldr r1, =battle_extension_free_hook|1
        bx r1
        .pool
