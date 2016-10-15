        .thumb
        .align 2

        .global mega_cmd_player_select_move_hook
mega_cmd_player_select_move_hook:
        bl mega_cmd_player_select_move
        ldr r1, =0x0802EB70|1
        bx r1

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global mega_cmd_player_select_move_doubles_hook
mega_cmd_player_select_move_doubles_hook:
        bl mega_cmd_player_select_move
        ldr r1, =0x0802E6FA|1
        bx r1

@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global mega_cmd_sync_trigger_from_buffer_hook
mega_cmd_sync_trigger_from_buffer_hook:
        bl mega_cmd_sync_trigger_from_buffer
        lsl r0, r3, #9
        add r1, #2
        add r0, r1
        ldrb r0, [r0]
        ldr r1, =0x080175AC|1
        bx r1

@@@ --------------------------------------------------------------------------
