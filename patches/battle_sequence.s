	// Setup multi-battles (two enemy trainers)
        .org 0x080132F8
        ldr r1, =multi_battle_2_opponents_hook|1
        bx r1
        .pool
