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
        ldrh r0, [r0]
        bl multi_money_calc_refactored
        mov r4, r0

        bl is_multi_battle
        cmp r0, #0
        beq 0f

        @@ Calculate money for the second opponent
        bl multi_second_opponent_id
        bl multi_money_calc_refactored
        add r0, r4

0:
        mov r4, r0
        ldr r1, =0x080259E8|1
        bx r1

multi_money_calc_refactored:
        @@ Pass in trainer ID in r0
        push {r4-r7, lr}
        mov r7, r8
        push {r7}
        ldr r2, =trainer_data
        lsl r1, r0, #2
        add r0, r1, r0
        ldr r3, =0x0802590C|1
        bx r3

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global multi_money_calc_return_hook
multi_money_calc_return_hook:
        lsl r0, r2, #1
        mov r4, r3
        mul r4, r0
        mov r0, r4
        pop {r3}
        mov r8, r3
        pop {r4-r7, pc}

@@@ --------------------------------------------------------------------------
