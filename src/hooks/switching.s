        .thumb
        .align 2

        .global multi_battle_opponent_should_send_out_hook
multi_battle_opponent_should_send_out_hook:
        bl is_multi_battle
        cmp r0, #0
        beq 0f
        mov r0, r4
        bl multi_opponent_should_send_out
        ldr r1, =0x08019D90|1
        bx r1

0:
        ldr r0, =battle_type_flags
        ldr r1, [r0]
        mov r0, #0x40
        and r1, r0
        cmp r1, #0
        beq 1f
        ldr r0, =0x08019C48|1
        bx r0

1:
        ldr r0, =0x08019CCC|1
        bx r0

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global mb_opponent_faint_switch_lb_hook
mb_opponent_faint_switch_lb_hook:
        lsr r5, r0, #0x18
        mov r6, r5
        bl multi_get_party_index_lower_bound
        mov r4, r0
        ldr r1, =0x080387AE|1
        bx r1

        .thumb
        .align 2

        .global mb_opponent_faint_switch_lb_hook2
mb_opponent_faint_switch_lb_hook2:
        lsr r0, r0, #0x18
        lsr r5, r0, #0x18
        bl multi_get_party_index_lower_bound
        mov r4, r0
        ldr r1, =0x080387AE|1
        bx r1

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global mb_opponent_faint_switch_ub_hook
mb_opponent_faint_switch_ub_hook:
        bl multi_get_party_index_upper_bound
        cmp r4, r0
        blo 0f
        ldr r1, =0x0803880A|1
        bx r1

0:
        mov r0, #0x64
        mul r0, r4
        ldr r1, =party_opponent
        ldr r2, =0x080387B8|1
        bx r2

@@@ --------------------------------------------------------------------------
