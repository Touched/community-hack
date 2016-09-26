#include <pokeagb/pokeagb.h>
#include "config.h"
#include "overworld.h"

u16 multi_second_opponent_id(void)
{
    return var_load(MULTI_SECOND_OPPONENT_TRAINER_ID);
}

bool is_multi_battle(void)
{
    return (battle_type_flags & BATTLE_FLAG_MULTI) == BATTLE_FLAG_MULTI;
}

void multi_trainer_flags_set(void)
{
    // TODO: Find a function that calculates trainer flag from trainer ID
    flag_set(trainerbattle_flag_id + 0x500);

    if (battle_type_flags & BATTLE_FLAG_MULTI) {
        flag_set(multi_second_opponent_id() + 0x500);
    }
}

void multi_opponent_slide_out(void)
{
    /* Reconfigure the battle to display the correct loss message */
    if (trainers_spotted.count == 2) {
        battle_configure_by_script(trainers_spotted.trainers[1].script + 1);
    }

    b_active_side = 1;
    dp01_build_cmdbuf_x09(0);
    dp01_battle_side_mark_buffer_for_execution(b_active_side);
}

bool is_multi_or_tag_battle(void)
{
    return battle_type_flags & (BATTLE_FLAG_MULTI | BATTLE_FLAG_PARTNER);
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

u8 multi_opponent_should_send_out(u8 side)
{
    /* Make sure enemy trainers only use the half of the team that
     * belongs to them. */

    struct Pokemon* party = party_opponent;

    if (battle_side_get_owner(side) == SIDE_OPPONENT) {
        u8 team_half = side == BANK_OPPONENT_ALLY;
        u8 lower = 3 * team_half;
        u8 upper = lower + 3;

        for (u8 i = lower; i < upper; i++) {
            struct PokemonBase* pokemon = (struct PokemonBase*) &party[i];
            enum PokemonSpecies species = pokemon_getattr(pokemon, REQUEST_SPECIES2, NULL);
            u16 hp = pokemon_getattr(pokemon, REQUEST_CURRENT_HP, NULL);

            if (hp && species && species != SPECIES_MAX) {
                return 0;
            }
        }

        return 1;
    }

    /* FIXME: Do player half for both tag and non-tag battles */

    return 0;
}

u8 multi_get_party_index_lower_bound(void)
{
    if (battle_type_flags & BATTLE_FLAG_MULTI) {
        return b_active_side == BANK_OPPONENT_ALLY ? 3 : 0;
    }

    return 0;
}

u8 multi_get_party_index_upper_bound(void)
{
    if (battle_type_flags & BATTLE_FLAG_MULTI) {
        return b_active_side == BANK_OPPONENT_ALLY ? 6 : 3;
    }

    return 6;
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

void multi_init_parties(void)
{
    if ((battle_type_flags & 0x80908) == 8) {
        pokemon_purge_opponent();
    }

    setup_opponent_party(party_opponent, trainerbattle_flag_id);

    /* FIXME: Remove the true */
    if (true || battle_type_flags & BATTLE_FLAG_MULTI) {
        /* Load the second half of the party */
        /* TODO: Get trainer ID from somewhere */
        setup_opponent_party(&party_opponent[3], multi_second_opponent_id());
    }
}

u8 multi_load_trainer_sprite(void)
{
    u16 trainer_id = trainerbattle_flag_id;

    if (battle_type_flags & BATTLE_FLAG_MULTI && b_active_side == 1) {
        trainer_id = multi_second_opponent_id();
    }

    return trainer_data[trainer_id].sprite;
}

pchar* multi_buffer_trainer2_class(void)
{
    u16 trainer_id = trainer_data[multi_second_opponent_id()].class;
    return trainer_class_names[trainer_id];
}

pchar* multi_buffer_trainer2_name(void)
{
    return trainer_data[multi_second_opponent_id()].name;
}
