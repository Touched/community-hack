        .gba
        .thumb
        .open "roms/BPRE0.gba","build/multi.gba", 0x08000000

        .include "patches/bx_setup.s"
        .include "patches/opponent_table.s"
        .include "patches/battle_sequence.s"
        .include "patches/battle_strings.s"
        .include "patches/multi_pokemon_order.s"
        .include "patches/overworld.s"
        .include "patches/switching.s"
        .include "patches/tag_team.s"
        .include "patches/battle.s"
        .include "patches/mega.s"
        .include "patches/hpbox_gfx/hpbox.s"
	.include "patches/battle_bgs_pal/bgpals.s"
        .include "patches/battle_graphics.s"
	.include "patches/hm_remove.s"

        // Only do this if you haven't repointed your battle script
	// command table
        .include "patches/move_commands.s"

        .org 0x09000000
        .importobj "build/linked.o"
        .close
