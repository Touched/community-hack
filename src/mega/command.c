/*
 * Adjustments to Battle Buffer 0x21 calling and handling.
 */

#include <pokeagb/pokeagb.h>
#include "mega.h"
#include "battle.h"

void mega_cmd_attack(u8 buffer, u8 move_slot, u8 target, bool mega_evolve)
{
    dp01_scratchpad[0] = 0x21;  /* Command 0x21 */
    dp01_scratchpad[1] = 0xA;   /* Select move */
    dp01_scratchpad[2] = target;
    dp01_scratchpad[3] = move_slot;
    dp01_scratchpad[4] = mega_evolve;
    dp01_prepare_buffer(buffer, dp01_scratchpad, 5);
}

void mega_cmd_player_select_move(void)
{
    mega_cmd_attack(1, selected_battle_side, move_index_chosen_per_side[b_active_side], true);
}

void mega_cmd_sync_trigger_from_buffer(void)
{
    /*
     * Synchronise the trigger from the buffer (see
     * mega_cmd_attack). These buffers are transmitted to other
     * consoles in link battles so we use this to synchronise the mega
     * triggers.
     */
    extension_state.mega_evolution->trigger[b_active_side] = b_buffer_B[b_active_side][4];
}
