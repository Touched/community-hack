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

        // Check if the opponent should send out another Pokemon when one faints.
        // This hook helps ensure the opponents don't share Pokemon
        .org 0x08019C40
        ldr r0, =multi_battle_opponent_should_send_out_hook|1
        bx r0
        .pool

        // TODO: Fix AI searching (see 08039E3C and 08039A80)
        .org 0x08038760
        nop
        mov r4, #6

        // Find the index of the next Pokemon to switch to if the AI to pick one.
        // This will search through the party, so we need to split it in half
        // so that the allies don't share their Pokemon
        .org 0x08038780
        ldr r1, =mb_opponent_faint_switch_lb_hook|1
        bx r1
        .pool

        .org 0x080387A4
        ldr r1, =mb_opponent_faint_switch_lb_hook2|1
        bx r1
        .pool

        .org 0x080387AE
        ldr r1, =mb_opponent_faint_switch_ub_hook|1
        bx r1
        .pool

        // TODO: Fix other switching cases (forced switching)
