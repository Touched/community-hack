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

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global mutli_enemy_pokeball_readout_hook
mutli_enemy_pokeball_readout_hook:
        bl is_multi_battle
        cmp r0, #0
        bne 1f
        mov r1, r10
        ldr r0, [r1]
        mov r1, #0x40
        and r0, r1
        cmp r0, #0
        bne 1f

0:
        @@ Render normally
        ldr r0, =0x080490C8|1
        bx r0

1:
        @@ Render split
        ldr r0, =0x08049094|1
        bx r0

@@@ --------------------------------------------------------------------------
