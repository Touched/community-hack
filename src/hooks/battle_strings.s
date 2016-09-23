        .thumb
        .align 2

        .global battle_string_intro_hook
battle_string_intro_hook:
        bl is_multi_battle
        cmp r0, #0
        beq not_multi
        adr r7, string_multi_battle_intro
        b return

not_multi:
        ldr r7, =0x083FD366

return:
        ldr r1, =0x080D77DC|1
        bx r1

        .align 2
string_multi_battle_intro:
        .string "{STRING 28} {STRING 29} and\n{STRING 28} {STRING 29}\lwant to battle!\p$"
