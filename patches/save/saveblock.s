        // Ensure the pointer to the addtional saveblock set
        .org 0x080004EC
        ldr r1, =saveblock_init_pointers_hook|1
        bx r1
        .pool
