        .thumb
        .align 2

        .global mega_ui_load_indicators_hook
mega_ui_load_indicators_hook:
        mov r0, r5
        bl mega_ui_load_indicators
        mov r0, r6
        pop {r4-r6, pc}

@@@ --------------------------------------------------------------------------
