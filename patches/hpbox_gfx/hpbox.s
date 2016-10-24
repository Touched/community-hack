	.relativeinclude on
		.include "update_hpbox_txt.s"
		
		// HP box graphics for player and opponent
        .org 0x08260208
		.word player_singles_hpboxTiles
		.pool
		
        .org 0x08260210
		.word opponent_hpboxTiles
		.pool
		
        .org 0x08260218
		.word player_doubles_hpboxTiles
		.pool
		
		.org 0x08260220
		.word player_doubles_hpboxTiles
		.pool

        .org 0x08260228
		.word opponent_hpboxTiles
		.pool
		
        .org 0x08260230
		.word opponent_hpboxTiles
		.pool
		
		// HP bar and exp bar graphics
		.org 0x080480B4
		.word hpbox_iconsTiles
		.pool

		// Palettes for HP boxes and icons
		.org 0x08260260
		.word player_singles_hpboxPal
		.pool

		.org 0x08260268
		.word hpbox_iconsPal
		.pool
		
		.relativeinclude off
        
        