	.thumb
        .align 2

        .global battle_message_box_pal_swap_hook
battle_message_box_pal_swap_hook:
        bl battle_message_box_load_palette
        ldr r1, =0x0800F448|1
        bx r1

@@@ --------------------------------------------------------------------------
