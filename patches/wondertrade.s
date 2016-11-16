        .org 0x08168C10
            .word to_trade|1

        .org 0x08050E28
            ldr r0, =name_buff_hook|1
            bx r0
        .pool

        // remove need for national dex for evolution
        .org 0x080CF560
            ldr r0, =(0x080CF598|1)
            bx r0
        .pool