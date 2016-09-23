	.thumb
        .align 2

        .global opponent_cmd07_x_hook
        @@ Adjust the x coordinate of the opponent trainer sprites
opponent_cmd07_x_hook:
        lsl r3, #0x18
        lsr r3, #0x18

        push {r1-r3}
        @@ Not a multi battle, so put the trainer in the middle of the
        @@ battle platform
        bl is_multi_battle
        cmp r0, #0
        beq opponent_cmd07_x_hook_opponent_single

        @@ Put both trainers side by side (coordintes coppied from
        @@ Emerald)
        mov r1, r8
        ldrb r0, [r1]
        cmp r0, #1
        beq opponent_cmd07_x_hook_opponent_a
        mov r0, #0xc8
        b opponent_cmd07_x_hook_return

opponent_cmd07_x_hook_opponent_a:
        mov r0, #0x98
        b opponent_cmd07_x_hook_return

opponent_cmd07_x_hook_opponent_single:
        mov r0, #0xB0

opponent_cmd07_x_hook_return:
        pop {r1-r3}
        mov r1, r0
        mov r0, r5
        ldr r2, =0x08037D88|1
        bx r2

        .thumb
        .align 2

        .global opponent_cmd07_sprite_hook
opponent_cmd07_sprite_hook:
        bl multi_load_trainer_sprite
        ldr r1, =0x08037D3A|1
        bx r1
