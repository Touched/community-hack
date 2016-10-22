	// Hook the palette load
        .org 0x0800F440
        bx r0
        .org 0x0800F45C
        .word battle_message_box_pal_swap_hook|1

        .org 0x0800F454
        .word battle_message_boxTiles
