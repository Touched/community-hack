        .thumb
        .align 2

        .global multi_battle_2_opponents_hook
multi_battle_2_opponents_hook:
        @@ Add a second trainer
        bl multi_battle_add_second_opponent

        @@ Increment battle bank
        mov r2, r9
        ldrb r0, [r2]
        add r0, #1
        strb r0, [r2]
        ldr r1, =0x08013300|1
        bx r1

call_via_r2:
        bx r2

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global multi_money_calc_hook

multi_money_calc_hook:
        bl multi_money_calc
        mov r4, r0
        ldr r1, =saveblock1
        mov r8, r1
        ldr r1, =0x080259E8|1
        bx r1

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global multi_trainer_flags_set_hook
multi_trainer_flags_set_hook:
        bl multi_trainer_flags_set
        pop {pc}

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global multi_trainer_sendout_pokemon_hook
multi_trainer_sendout_pokemon_hook:
        bl is_link_or_multi_battle
        ldr r1, =0x080135D6|1
        bx r1

@@@ --------------------------------------------------------------------------
