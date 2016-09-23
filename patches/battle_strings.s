	// Add multi battle trainer introduction message
        .org 0x080D73B6
        bx r7

        .org 0x080D73C0
        .word battle_string_intro_hook|1

        // Add multi battle trainer send out enemy Pokemon message
        .org 0x080D7474
        ldr r0, =battle_string_sendout_hook|1
        bx r0
        .pool

        // Additional battle buffers
        .org 0x080D789A
        cmp r0, #(0x30 + 2)

        // Repoint existing table
        .org 0x080D78AC
        .word repointed_battle_buffers
