#include <pokeagb/pokeagb.h>
#include "config.h"

bool is_multi_battle(void)
{
    return (battle_type_flags & BATTLE_FLAG_MULTI) == BATTLE_FLAG_MULTI;
}

void multi_battle_add_second_opponent(void)
{
    if (battle_type_flags & BATTLE_FLAG_MULTI) {
        if (battle_get_per_side_status(b_active_side) == 3) {
            dp01_build_cmdbuf_x07(0);
            dp01_battle_side_mark_buffer_for_execution(b_active_side);
        }
    }
}

void multi_init_pokemon_order(void)
{
    /*
     * In a regular double battle, the first two pokemon are the first
     * two be set out. However in a multi battle, both trainers need
     * to send out their first Pokemon, which corresponds to slot 0
     * and 3.
     */
    if (battle_type_flags & BATTLE_FLAG_MULTI) {
        b_pokemon_team_id_by_side[1] = 0;
        b_pokemon_team_id_by_side[3] = 3;
    }
}
