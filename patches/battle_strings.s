	// Trainer battle hook
        .org 0x080D73B6
        bx r7

        .org 0x080D73C0
        .word battle_string_intro_hook|1
