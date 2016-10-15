#include <pokeagb/pokeagb.h>
#include "mega.h"
#include "battle.h"

#define LAST_MESSAGE 0x181

void mega_transformation_cb(void);

const pchar* mega_get_trainer_name(void)
{
    /* TODO: Handle other banks */
    return &saveblock2->name;
}

const pchar* mega_get_keystone_name(void)
{
    /* TODO: Fetch keystone for bank */
    return &items[ITEM_GOLDTEETH].name;
}

void mega_evolution_handler(void)
{
    struct MegaEvolutionState* mega = extension_state.mega_evolution;

    u8 side = b_attackers_in_order[bs_mode_pbs_index];
    if (mega->trigger[side]) {
        /*
         * Setup transformation for Mega Evolution and wait for it to
         * complete.
         */
        mega->bc_backup = b_c;
        b_c = mega_transformation_cb;
        mega->trigger[side] = 0;

        /* Display the first message */
        b_active_side = side;
        pstrcpy(fcode_buffer2, mega_get_trainer_name());
        pstrcpy(fcode_buffer3, mega_get_keystone_name());
        dp01_build_cmdbuf_x10(BUFFER_A, LAST_MESSAGE + 1);
        dp01_battle_side_mark_buffer_for_execution(b_active_side);
    } else {
        /*
         * Only run the attack start if we're not busy handling a mega
         * transformation
         */
        bs0_start_attack();
    }
}

void mega_transformation_wait_animation_cb(void)
{
    /* Wait for animation */
    if (!b_buffers_awaiting_execution_bitfield) {
        /* Final message */
        b_active_side = b_attackers_in_order[bs_mode_pbs_index];
        dp01_build_cmdbuf_x10(BUFFER_A, LAST_MESSAGE + 3);
        dp01_battle_side_mark_buffer_for_execution(b_active_side);
        b_c = extension_state.mega_evolution->bc_backup;
    }
}

void mega_transformation_cb(void)
{
    /* Wait for the message */
    if (!b_buffers_awaiting_execution_bitfield) {
        /* Run an animation script */
        b_active_side = b_attackers_in_order[bs_mode_pbs_index];
        dp01_build_cmdbuf_x34(BUFFER_A, 2, 55);
        dp01_battle_side_mark_buffer_for_execution(b_active_side);
        b_c = mega_transformation_wait_animation_cb;
    }
}
