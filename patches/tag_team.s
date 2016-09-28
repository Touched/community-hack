	// Quick hack to enable the tag team party menu
        // FIXME: Do it properly
        .org 0x0811FA2E
        mov r0, #1

        // Load the partner's name in the Pokemon menu
        .org 0x08127AE8
        ldr r0, =tag_team_partner_name_in_switch_hook|1
        bx r0
        .pool

        // Fix backsprite x coordinates in tag team
        // TODO: Load custom backsprite (see 0803227C dp01t_07_1_trainer_enter_field)
        .org 0x08032280
        ldr r1, =tag_team_partner_slide_in_xpos_hook|1
        bx r1
        .pool

        // Send command 0x24 to partner bank (send out Pokemon)
        // FIXME: Actually check for tag team flag, don't nop
        .org 0x08013762
        nop

        // Make the player only throw one Pokeball
        // FIXME: Actually check for tag team flag, don't nop
        .org 0x080336AA
        nop

        // Needed to continue from send out to the fight menu
        // FIXME: Find out why this isn't being executed, don't nop
        .org 0x0802F918
        nop

        // Needed to continue from send out to the fight menu
	// FIXME: Find out why this isn't being executed, don't nop
        .org 0x0802F932
        nop

        // Send out message
        // TODO: Move this to battle strings?
        .org 0x080D7444
        ldr r7, =tag_team_sendout_string_hook|1
        bx r7
        .pool

        // Tag team partner backsprite
        .org 0x0803233E
        bx r0
        .org 0x08032400
        .word tag_team_backsprite_hook|1

        // Tag team partner backsprite pallete fix
        // This is needed to keep the palette consistent during animation
        .org 0x08033524
        bx r0
        .org 0x080335D8
        .word tag_team_backsprite_pal_hook|1
