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
