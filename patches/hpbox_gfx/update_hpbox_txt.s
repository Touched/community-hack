        // Updates the big HP numbers for the player
        // single battles only, and during HP increase/reduction
        .org 0x08048440
        ldr r3, =hp_nums_update_hook|1;
        bx r3
        .pool

        // Updates HP bar text and other gfx
        .org 0x08049DA4
        ldr r3, =draw_hp_box_elements_hook|1;
        bx r3
        .pool
		