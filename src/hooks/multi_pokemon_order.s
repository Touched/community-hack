        .thumb
        .align 2

        .global multi_init_pokemon_order_hook
multi_init_pokemon_order_hook:
	bl multi_init_pokemon_order
        mov r10, r5
        pop {r4-r7}
        pop {r0}
        bx r0


        .global multi_init_parties_hook
multi_init_parties_hook:
        bl multi_init_parties
        ldr r0, =0x0800FF94|1
        bx r0
