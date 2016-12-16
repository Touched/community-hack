        // The original games use 14 out of the 16 available save sectors
	// (the remaining 16 are used for the save backup). This file
	// makes all the necessary changes to make the game use all 16
	// sectors.
        FLASH_SAVE_SECTORS equ 16

        .org 0x080D9816
        mov r1, #FLASH_SAVE_SECTORS

        .org 0x080D9834
        cmp r4, #(FLASH_SAVE_SECTORS - 1)

        .org 0x080D9A22
        mov r1, #FLASH_SAVE_SECTORS

        .org 0x080D9B6C
        mov r1, #FLASH_SAVE_SECTORS

        .org 0x080D9B7E
        nop
        nop
        lsl r0, #4              // multiply by 16

        .org 0x080D9D04
        mov r1, #FLASH_SAVE_SECTORS

        .org 0x080D9D16
        nop
        nop
        lsl r0, #4              // multiply by 16

        .org 0x080D9D9C
        mov r1, #FLASH_SAVE_SECTORS

        .org 0x080D9DAE
        nop
        nop
        lsl r0, #4              // multiply by 16

        .org 0x080D9F7E
        cmp r4, #(FLASH_SAVE_SECTORS - 1)

        .org 0x080D9FB0
        .word 0xFFFF            // all 16 (instead of 14) bits out of the word set

        .org 0x080D9FC2
        add r0, #FLASH_SAVE_SECTORS

        .org 0x080DA01A
        cmp r4, #(FLASH_SAVE_SECTORS - 1)

        .org 0x080DA048
        .word 0xFFFF            // all 16 (instead of 14) bits out of the word set

        .org 0x080DA3DC
        mov r0, #FLASH_SAVE_SECTORS

        .org 0x080DA410
        mov r0, #FLASH_SAVE_SECTORS

        .org 0x080DA438
        mov r0, #FLASH_SAVE_SECTORS
