	// Initialise the order of Pokemon in a multi battle
        .org 0x0800D8A0
        ldr r0, =multi_init_pokemon_order_hook|1
        bx r0
        .pool

        // Load the parties for both trainers
        .org 0x0800FF8C
        ldr r1, =multi_init_parties_hook|1
        bx r1
        .pool

        // Stop the opponent being purged
        // TOOD: Cap the enemy's party at 3 (see 08038646 in Emerald)
        .org 0x08011318
        nop
        nop

        // Make the Pokeball readout (when enemy is switching pokemon)
	// split down the middle.
        .org 0x08049088
        ldr r1, =mutli_enemy_pokeball_readout_hook|1
        bx r1
        .pool
