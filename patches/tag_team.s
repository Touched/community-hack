	// Quick hack to enable the tag team party menu
        .org 0x0811FA20
        ldr r0, =tag_team_is_tag_team_hook|1
        bx r0
        .pool

        // Load the partner's name in the Pokemon menu
        .org 0x08127AE8
        ldr r0, =tag_team_partner_name_in_switch_hook|1
        bx r0
        .pool

        // Fix backsprite x coordinates in tag team
        .org 0x08032280
        ldr r1, =tag_team_partner_slide_in_xpos_hook|1
        bx r1
        .pool

        // Send command 0x24 to partner bank (send out Pokemon)
        .org 0x08013758
        ldr r0, =tag_team_partner_sendout_pokemon_hook|1
        bx r0
        .pool

        // Make the player only throws one Pokeball
        .org 0x080336A0
        ldr r0, =tag_team_partner_throw_pokeball_hook|1
        bx r0
        .pool

        // Needed to continue from send out to the fight menu
        // FIXME: Find out why this isn't being executed, don't nop
        .org 0x0802F918
        nop

        // Needed to continue from send out to the fight menu
	// FIXME: Find out why this isn't being executed, don't nop
        .org 0x0802F932
        nop

        // Send out message
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

        // Hook the table execution completion functions to set the
	// correct bank.
        .org 0x0802E34C
        bx r1
        .org 0x0802E384
        .word tag_team_tbl1_exec_complete_hook|1

        // Make sure the ally's Pokemon cannot disobey
        .org 0x0801D480
        ldr r1, =tag_team_obedience_hook|1
        bx r1
        .pool

        // Make the Pokeball readout split for the player
        .org 0x08048FAC
        ldr r1, =tag_team_pokeball_readout_hook|1
        bx r1
        .pool

        // Fix pagination of the summary screen (pressing up or down
	// on the summary menu) so that it loops through the player's
	// half of the party before moving onto the ally's half.
        .org 0x0813B19E
        nop
        .org 0x0813B1A6
        nop

        // Stop the ally Pokemon from gaining experience (treat it as
	// level 100)
        .org 0x08021CFC
        ldr r1, =tag_team_partner_exp_gain_hook|1
        bx r1
        .pool
