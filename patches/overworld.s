	// Rewrite the check for players loop
        .org 0x08081B30
        ldr r1, =overworld_handle_trainer_spot|1
        bx r1
        .pool

        // Register trainers that spot the player
        .org 0x08081BC0
        ldr r1, =overworld_register_trainer_hook|1
        bx r1
        .pool

        // Patch the trainer battle start to add other battle flags
        .org 0x08080464
        ldr r0, =overworld_trainer_battle_flags_hook|1
        bx r0
        .pool
        
        
    // disable help system
    .org 0x0800051A
    .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
