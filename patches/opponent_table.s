	.org 0x08037D80
        ldr r0, =opponent_cmd07_x_hook|1
        bx r0
        .pool

        // Finish
        .org 0x08038DFC
        nop
