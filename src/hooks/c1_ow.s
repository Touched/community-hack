        .thumb
        .align 2

        .global c1_overworld_hook
c1_overworld_hook:
        push {lr}
        bl c1_overworld_plus
        pop {pc}


@@@ --------------------------------------------------------------------------
