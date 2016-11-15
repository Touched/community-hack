        .thumb
        .align 2
        
        .global name_buff_hook
name_buff_hook:
    bl trade_name_buffering
    ldr r0, =0x08050EF0|1
    bx r0
