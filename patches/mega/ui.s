	// Allow button presses on the fight menu to be intercepted
        .org 0x0802EA2C
        bx r0
        .org 0x0802EA74
        .word mega_move_menu_controls_hook|1
