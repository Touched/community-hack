	// Adjust the X coordinate of the enemy trainer sprites for
	// multi battles
	.org 0x08037D80
        ldr r0, =opponent_cmd07_x_hook|1
        bx r0
        .pool

        // Load the trainer sprite for second enemy trainer
        .org 0x08037D2C
        ldr r1, =opponent_cmd07_sprite_hook|1
        bx r1
        .pool

        // TODO: Figure out this code
        // .org 0x08038DFC
        // nop

        .org 0x08037F00
        ldr r1, =opponent_cmd08_sprite_hook|1
        bx r1
        .pool

        // Fix battle script command 0x53 so it can now work on any bank
        // Emerald does't have this check anyway (see 0804DDB0)
        .org 0x080250E4
        nop
        nop
        nop
