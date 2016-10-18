        // Updates the big HP numbers for the player
        // single battles only, and during HP increase/reduction
        .org 0x08048440
        push {lr}
        //bl hp_nums_update
        pop {pc}
        .pool

        // Updates HP bar text and other gfx
        .org 0x08049DA4
        mov r8, r8
        //bl draw_hp_box_elements
        ldr r4, =0x08049FC6|1
        bx r4
        .pool