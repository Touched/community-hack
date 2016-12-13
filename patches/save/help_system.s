	// Disable full screen saving
        .org 0x08000512
        nop
        nop
        nop
        nop

        .org 0x080F5100
        bx lr

        .org 0x080F50F4
        bx lr

        // Disable help system
        .org 0x0800051A
        nop
        nop
        nop
        nop
        nop
