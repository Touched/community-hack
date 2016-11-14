        // nop old c1_overworld for c1_overworld_plus.
        // see src/c1_overworld.c
        .org 0x08056534
        ldr r0, =c1_overworld_hook|1;
        bx r0
        .pool

		