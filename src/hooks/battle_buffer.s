	.align 2
	.global repointed_battle_buffers
repointed_battle_buffers:
        .incbin "roms/BPRE0.gba", 0x0D78B0, (0x31 * 4)
        .word buffer_trainer2_class
        .word buffer_trainer2_name
        .word buffer_partner_class
        .word buffer_partner_name

        .thumb
        .align 2

        .global buffer_trainer2_class
buffer_trainer2_class:          @ Buffer 0x31
        bl multi_buffer_trainer2_class
        b buffer_copy

        .global buffer_trainer2_name
buffer_trainer2_name:           @ Buffer 0x32
        bl multi_buffer_trainer2_name
        b buffer_copy

        @@ Tag team battle buffers
        .global buffer_partner_class
buffer_partner_class:          @ Buffer 0x33
        bl partner_load_class
        b buffer_copy

        .global buffer_partner_name
buffer_partner_name:           @ Buffer 0x34
        bl partner_load_name

buffer_copy:
        mov r4, r0
        ldr r0, =0x080D8382|1
        bx r0
