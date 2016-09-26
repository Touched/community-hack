        .thumb
        .align 2

        .global overworld_register_trainer_hook
overworld_register_trainer_hook:
        mov r0, r5
        mov r1, r6
        mov r2, r7
        bl overworld_register_trainer
        ldr r0, =0x08081BDA|1
        bx r0

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global overworld_trainer_battle_flags_hook
overworld_trainer_battle_flags_hook:
        push {r4, lr}
        bl overworld_trainer_battle_flags
        ldr r1, =0x0808046C|1
        bx r1

@@@ --------------------------------------------------------------------------
