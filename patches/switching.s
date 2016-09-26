        // Check if the opponent should send out another Pokemon when one faints.
        // This hook helps ensure the opponents don't share Pokemon
        .org 0x08019C40
        ldr r0, =multi_battle_opponent_should_send_out_hook|1
        bx r0
        .pool

        // Fix AI searching for suitable Pokemon
        // TODO: 08039A80
        .org 0x08039F18         // Lower bound
        ldr r0, =mb_ai_suitable_pokemon_lb1_hook|1
        bx r0
        .pool

        .org 0x0803A000         // Upper bound
        ldr r0, =mb_ai_suitable_pokemon_ub1_hook|1
        bx r0
        .pool

        .org 0x0803A094         // Lower bound
        ldr r0, =mb_ai_suitable_pokemon_lb2_hook|1
        bx r0
        .pool

        .org 0x0803A158         // Upper bound
        // There isn't enough space for a standard hook
        // Instead, we break ARM/THUMB interworking to get some space
        ldr r0, =mb_ai_suitable_pokemon_ub2_hook|1
        bx r0
        .pool

@load_return_branch_fix:
        ldr r0, [sp, #8]
        add sp, #0x1C
        pop {r3-r5}
        mov r8, r3
        mov r9, r4
        mov r10, r5
        pop {r4-r7, pc}

        // There was a branch just before the return that was pushed
	// down by the above hook. Adjust it so that it loads the
	// correct value.
        .org 0x0803A048
        b @load_return_branch_fix

        // Find the index of the next Pokemon to switch to if the AI to pick one.
        // This will search through the party, so we need to split it in half
        // so that the allies don't share their Pokemon
        .org 0x08038780         // Lower bound
        ldr r1, =mb_opponent_faint_switch_lb_hook|1
        bx r1
        .pool

        .org 0x080387A4         // Lower bound
        ldr r1, =mb_opponent_faint_switch_lb_hook2|1
        bx r1
        .pool

        .org 0x080387AE         // Upper bound
        ldr r1, =mb_opponent_faint_switch_ub_hook|1
        bx r1
        .pool

        // TODO: Fix other switching cases (forced switching)
        // TODO: Switching battle messages
