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

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global tag_team_partner_slide_in_xpos_hook
tag_team_partner_slide_in_xpos_hook:
        push {r7}
        bl is_partner_battle
        cmp r0, #0
        beq 0f
        ldr r2, =0x0803228E|1
        bx r2

0:
        ldr r0, =battle_type_flags
        ldr r0, [r0]
        mov r1, #0x40
        ldr r2, =0x08032288|1
        bx r2

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global tag_team_sendout_string_hook
tag_team_sendout_string_hook:
        and r1, r0
        cmp r1, #0
        bne 0f
        bl is_partner_battle
        cmp r0, #0
        beq 2f
        adr r7, string_partner_battle_sendout
        b 1f

0:
        @@ Link multi battle string
        ldr r7, =0x083FD466

1:
        @@ Render string
        ldr r1, =0x080D77DC|1
        bx r1

2:
        @@ Single battle send out
        ldr r1, =0x080D744E|1
        bx r1

        .align 2
string_partner_battle_sendout:
        .string "{STRING 51} {STRING 52} sent out {STRING 7}!\nGo {STRING 5}!$"

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global tag_team_backsprite_hook
tag_team_backsprite_hook:
        bl is_partner_battle
        cmp r0, #0
        beq 0f
        ldr r0, =b_active_side
        ldrb r0, [r0]
        cmp r0, #2
        bne 0f
        bl partner_load_backsprite_index
        mov r4, r0
        b 1f

0:
        ldr r0, =saveblock2
        ldr r0, [r0]
        ldrb r4, [r0, #8]

1:
        ldr r5, =0x08032342|1
        bx r5

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global tag_team_backsprite_pal_hook
tag_team_backsprite_pal_hook:
        bl is_partner_battle
        cmp r0, #0
        beq 0f
        ldr r0, =b_active_side
        ldrb r0, [r0]
        cmp r0, #2
        bne 0f
        bl partner_load_backsprite_index
        b 1f

0:
        ldr r0, =saveblock2
        ldr r0, [r0]
        ldrb r0, [r0, #8]

1:
        ldr r2, =0x08033528|1
        bx r2


@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global tag_team_tbl1_exec_complete_hook
tag_team_tbl1_exec_complete_hook:
        push {r0}
        @@ Ensure this is a tag team battle
        bl is_partner_battle
        cmp r0, #0
        beq 0f

        @@ Ensure the active bank is the player's ally
        ldrb r0, [r4]
        cmp r0, #2
        bne 0f

        @@ We should use the partner's thing
        ldr r1, =multi_bx_partner
        b 1f

0:
        @@ The standard player command executer
        ldr r1, =0x0802E3B4|1

1:
        pop {r0}
        str r1, [r0]
        ldr r0, =0x0802E34E|1
        bx r0

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global tag_team_obedience_hook
tag_team_obedience_hook:
        lsl r0, #0x18
        cmp r0, #0
        beq 1f
	bl is_partner_attacker
        cmp r0, #0
        beq 0f
        b 1f

0:
        ldr r0, =0x827
        ldr r1, =0x0801D488|1
        bx r1

1:
        ldr r1, =0x0801D4F4|1
        bx r1
