        // Battle Buffer Command 0x21 handles sending commands to
	// buffer B, the buffer that handles selecting a move. When it is
	// called with 0xA as the first argument, the second argument is
	// a halfword where the LSB is the move slot (0 - 3) and the MSB
	// is the target bank. Since this second argument is a halfword,
	// we can't fit any more data in there. We thus patch it so that
	// it can fit more than one halfword in order to squeeze an
	// additional boolean inside indicating whether Mega Evolution
	// was triggered or not. This is then synchronised across the link
        // cable, so we'll hook somewhere else to read the boolean and set
        // the relevant state.

        // Patch all the calls to the command 0x21 preparation function
        // that cause the Pokemon to attack
        .org 0x0802EB5A
        bx r0
        .org 0x0802EB84
        .word mega_cmd_player_select_move_hook|1

        .org 0x0802E6E6
        bx r0
        .org 0x0802E728
        .word mega_cmd_player_select_move_doubles_hook|1

        // Patch the move handler so that it sets the trigger from the
	// battle menu buffer (buffer B)
        .org 0x080175A4
        ldr r0, =mega_cmd_sync_trigger_from_buffer_hook|1
        bx r0
        .pool
