        .thumb
        .align 2

        .global mega_ui_load_indicators_hook
mega_ui_load_indicators_hook:
	bl mega_ui_load_indicators
        pop {r4-r6, pc}

@@@ --------------------------------------------------------------------------
