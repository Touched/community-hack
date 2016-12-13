	// Remove Hall of Fame, as it uses two additional sectors
	// outside of the normal saveblock storaage. This enables
	// use of all 16 sectors.
        .org 0x080F2230         // writing
        nop
        nop

        .org 0x080DA526         // reading
        nop

        // Modify the saveblock serialisation/deserialisation so that
	// the blocks are more tighly packed in flash memory to
	// minimise wasted space.
        .org 0x080D9870         // writing
        ldr r1, =saveblock_write_to_flash|1
        bx r1
        .pool

        .org 0x080D9E54         // reading
        ldr r1, =saveblock_read_from_flash|1
        bx r1
        .pool

        // Make save validation calculate the checksum on the entire
	// data portion of the sector. Since the sector buffer is
	// zeroed out, the checksum will always be correctly calculated.
        .org 0x080D9F4C
        mov r1, r3              // 4084
        .org 0x080D9FE8
        mov r1, r3              // 4084
