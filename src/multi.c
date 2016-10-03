#include <pokeagb/pokeagb.h>
#include "config.h"
#include "battle.h"
#include "partner.h"

u16 multi_second_opponent_id(void)
{
    return var_load(MULTI_SECOND_OPPONENT_TRAINER_ID);
}

bool is_multi_battle(void)
{
    return (battle_type_flags & BATTLE_FLAG_MULTI) == BATTLE_FLAG_MULTI;
}

bool is_link_or_multi_battle(void)
{
    return is_multi_battle() || (battle_type_flags & BATTLE_FLAG_PARTNER);
}

void multi_trainer_flags_set(void)
{
    // TODO: Find a function that calculates trainer flag from trainer ID
    flag_set(trainerbattle_flag_id + 0x500);

    if (battle_type_flags & BATTLE_FLAG_MULTI) {
        flag_set(multi_second_opponent_id() + 0x500);
    }
}

static u32 multi_money_calc_for_trainer(u16 trainer_id)
{
    struct Trainer* trainer = &trainer_data[trainer_id];

    /* Find level of the last Pokemon in trainer's party */
    u8 level = 0;
    if (trainer->flags & TRAINER_PARTY_MOVESET) {
        struct TrainerPokemonMoves* pokemon = (struct TrainerPokemonMoves*) trainer->party;
        level = pokemon[trainer->party_size - 1].base.level;
    } else {
        struct TrainerPokemonBase* pokemon = (struct TrainerPokemonBase*) trainer->party;
        level = pokemon[trainer->party_size - 1].level;
    }

    struct TrainerMoneyRate* rate;
    for (rate = trainer_class_money_rate; rate->class != trainer->class; rate++) {
        if (rate->class == 0xFF) {
            /* Default to the first class's rate */
            rate = trainer_class_money_rate;
            break;
        }
    }

    u32 money = rate->money * level * battle_stuff->money_multiplier * 4;

    if (battle_type_flags & BATTLE_FLAG_DOUBLE && !(battle_type_flags & BATTLE_FLAG_MULTI)) {
        money *= 2;
    }

    return money;
}

u32 multi_money_calc(void)
{
    u16 money = multi_money_calc_for_trainer(trainerbattle_flag_id);

    if (battle_type_flags & BATTLE_FLAG_MULTI) {
        money += multi_money_calc_for_trainer(multi_second_opponent_id());
    }

    return money;
}

void multi_opponent_slide_out(void)
{
    /* Reconfigure the battle to display the correct loss message */
    /* FIXME: Get rid of this hack. Store the messages elsewhere */
    if (extension_state.spotted.count == 2) {
        u8 temp = trainerbattle_flag_id;
        battle_configure_by_script(extension_state.spotted.trainers[1].script + 1);
        trainerbattle_flag_id = temp;
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

    /* Send command to partner in case of tag team battle */
    if (is_partner_battle()) {
        if (battle_get_per_side_status(b_active_side) == 2) {
            dp01_build_cmdbuf_x07(0);
            dp01_battle_side_mark_buffer_for_execution(b_active_side);
        }
    }
}

u8 multi_opponent_should_send_out(u8 side)
{
    /*
     * Make sure trainers only use the half of the team that belongs
     * to them.
     */

    struct Pokemon* party = NULL;
    u8 lower = 0, upper = 6;

    /* Figure out what indices to look through */
    if (battle_side_get_owner(side) == SIDE_OPPONENT) {
        party = party_opponent;
        lower = 3 * (side == BANK_OPPONENT_ALLY);
        upper = lower + 3;
    } else {
        party = party_player;

        if (is_partner_battle()) {
            lower = 3 * (side == BANK_PLAYER_ALLY);
            upper = lower + 3;
        }
    }

    /* Check if there is a healthy Pokemon */
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

    if (is_partner_battle()) {
        b_pokemon_team_id_by_side[BANK_PLAYER_ALLY] = 3;
    }
}

void multi_init_parties(void)
{
    if ((battle_type_flags & 0x80908) == 8) {
        pokemon_purge_opponent();
    }

    setup_opponent_party(party_opponent, trainerbattle_flag_id);

    if (battle_type_flags & BATTLE_FLAG_MULTI) {
        /* Load the second half of the party */
        setup_opponent_party(&party_opponent[3], multi_second_opponent_id());
    }

    if (is_partner_battle()) {
        partner_load_party();
    }
}

u8 multi_load_trainer_sprite(void)
{
    u16 trainer_id = trainerbattle_flag_id;

    if (battle_type_flags & BATTLE_FLAG_MULTI && b_active_side == 3) {
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
