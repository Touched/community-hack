        .thumb
        .align 2

        .global battle_string_intro_hook
battle_string_intro_hook:
        bl is_multi_battle
        cmp r0, #0
        beq 0f
        adr r7, string_multi_battle_intro
        b 1f

0:
        @@ Load standard battle intro string
        ldr r7, =0x083FD366

1:
        @@ Render the string
        ldr r1, =0x080D77DC|1
        bx r1

        .align 2
string_multi_battle_intro:
        .string "{STRING 28} {STRING 29} and\n{STRING 49} {STRING 50}\lwant to battle!\p$"

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global battle_string_sendout_hook
battle_string_sendout_hook:
        adr r7, string_multi_battle_sendout
        b 0f

        mov r0, #0x40
        and r0, r1
        cmp r0, #0
        beq 1f
        ldr r7, =0x083FD41E

0:
	@@ Return branch 1
        ldr r0, =0x080D77DC|1
        bx r0

1:
        @@ Return branch 2
        ldr r0, =0x080D7488|1
        bx r0

        .align 2
string_multi_battle_sendout:
        .string "{STRING 28} {STRING 29} sent\nout {STRING 6}!\p"
        .string "{STRING 49} {STRING 50} sent\nout {STRING 8}!$"

@@@ --------------------------------------------------------------------------

        .global string_multi_battle_won
string_multi_battle_won:
        .string "{STRING 28} {STRING 29} and\n{STRING 49} {STRING 50}\lwere defeated!\p$"

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global battle_string_switchin_hook
battle_string_switchin_hook:
        bl is_multi_battle
        cmp r0, #0
        beq 1f

0:
        ldr r7, =battle_script_active_side
        ldrb r7, [r7]
        cmp r7, #1
        beq 1f
        adr r7, string_multi_battle_switchin_trainerB
        b 2f

1:
        ldr r7, =0x083FD3E4

2:
        ldr r0, =0x80D77DC|1
        bx r0

        .align 2
string_multi_battle_switchin_trainerB:
        .string "{STRING 49} {STRING 50} sent\nout {STRING 8}!$"

@@@ --------------------------------------------------------------------------
