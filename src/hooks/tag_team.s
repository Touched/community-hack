        .thumb
        .align 2

        .global tag_team_partner_name_in_switch_hook
tag_team_partner_name_in_switch_hook:
        bl is_partner_battle
        cmp r0, #0
        beq 0f
        bl partner_load_name
        b 1f

0:
        ldr r2, =0x080444C4|1
        bl call_via_r2

1:
	mov r1, r0
        mov r0, r4
        ldr r2, =0x08127AF0|1
        bx r2

call_via_r2:
        bx r2
