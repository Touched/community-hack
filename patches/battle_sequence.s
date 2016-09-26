	// Setup multi-battles (two enemy trainers)
        .org 0x080132F8
        ldr r1, =multi_battle_2_opponents_hook|1
        bx r1
        .pool

        // Sends command x2F (send out opponent pokemon) to bank 3 (opponent ally)
        // FIXME: Actually check for the multi/partner battle flag
	// instead of nopping the check out
        .org 0x080135D8
        nop

        // Allow the player to send out their pokemon after the enemy
	// has sent out theirs
        // FIXME: Don't nop out this entire function and figure out why it doesn't work
        // (see 0805F2F0 in Emerald)
        .org 0x08035C32
        ldr r0, =0x08035DB0|1
        bx r0
        .pool

        // Victory Sequence
        .org 0x08015724
        .word battle_script_victory

        // Money Calculation
        .org 0x08025904
        ldr r2, =multi_money_calc_hook|1
        bx r2
        .pool

        // Convert the middle section of the money calculator into its
	// own function so we can call it once per trainer
        .org 0x080259C4
        ldr r0, =multi_money_calc_return_hook|1
        bx r0
        .pool

        // Take an unused string from FireRed and replace it with our
	// multi-battle virtory message
        // The original string was: "You throw a BALL now, right? I... I`ll do my best!"
        // This appears to be a holdover from the Wally Battle from RS
        .org 0x083fe440
        .word string_multi_battle_won
