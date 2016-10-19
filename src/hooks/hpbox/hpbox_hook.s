        .thumb
        .align 2

        .global hp_nums_update_hook
hp_nums_update_hook:
        push {lr}
        bl hp_nums_update
        pop {pc}


@@@ --------------------------------------------------------------------------

        .thumb
        .align 2

        .global draw_hp_box_elements_hook
draw_hp_box_elements_hook:
        mov r8, r8
        bl draw_hp_box_elements
        ldr r4, =0x08049FC6|1
        bx r4

@@@ --------------------------------------------------------------------------
