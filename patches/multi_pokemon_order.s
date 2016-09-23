	// Initialise the order of Pokemon in a multi battle
        .org 0x0800D8A0
        ldr r0, =multi_init_pokemon_order_hook|1
        bx r0
        .pool
