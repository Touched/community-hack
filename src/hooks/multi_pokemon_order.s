        .thumb
        .align 2

        .global multi_init_pokemon_order_hook
multi_init_pokemon_order_hook:
	bl multi_init_pokemon_order
        mov r10, r5
        pop {r4-r7}
        pop {r0}
        bx r0
