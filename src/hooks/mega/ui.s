        .thumb
        .align 2

        .global mega_move_menu_controls_hook
mega_move_menu_controls_hook:
        bl mega_ui_menu_button_hijack
        cmp r0, #0
        beq 0f

        @@ Make function return without handling button presses
        ldr r1, =0x0802EDBA|1
        bx r1

0:
        @@ Handle button presses normally
        ldr r0, =super
        ldrh r1, [r0, #0x2E]
        ldr r0, =0x0802EA2E|1
        bx r0

@@@ --------------------------------------------------------------------------
