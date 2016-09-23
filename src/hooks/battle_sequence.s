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
