        .thumb
        .align 2

        .global battle_extension_free_hook
battle_extension_free_hook:
	bl battle_extension_free
        pop {r4-r6, pc}

@@@ --------------------------------------------------------------------------
