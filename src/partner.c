#include <pokeagb/pokeagb.h>
#include "config.h"
#include "partner.h"

#define COMMAND_MAX 0x39

static void (*multi_partner_table[COMMAND_MAX])(void);

struct TagTeamPartner tag_team_partners[] = {
    {
        .name = _"MAY",
        .gender = GENDER_FEMALE,
        .class = 1,
        .sprite = 3,
        .party_size = 1,
        {
            {
                .nickname = _"",
                .species = SPECIES_MARSHTOMP,
                .item = ITEM_ORANBERRY,
                .moves = { MOVE_MUD_SHOT, MOVE_SURF },
                .level = 18
            }
        }
    }
};

#define TAG_TEAM_MAX_PARTNER_INDEX (sizeof(tag_team_partners) / sizeof(struct TagTeamPartner))

/* TODO: Inline */
struct TagTeamPartner* get_partner(void)
{
    /* TODO: Get this index from somewhere (script variable?) */
    u8 index = 0;

    if (index > TAG_TEAM_MAX_PARTNER_INDEX) {
        index = 0;
    }

    return &tag_team_partners[index];
}

void partner_load_pokemon(struct TagTeamPartner* partner,
                          u32 tid,
                          struct Pokemon* slot,
                          struct TagTeamPokemon* pokemon)
{
    /* Generate the PID and possibly filter it */
    u32 pid = rand() | (rand() << 16);
    if (pokemon->pid_hook) {
        pid = pokemon->pid_hook(pid, pokemon, pokemon->pid_hook_data);
    }

    pokemon_make_full(slot, pokemon->species, pokemon->level, 0xFF, true, pid, 1, tid);

    /* Set OT and origin information */
    struct PokemonBase* base = (struct PokemonBase*) slot;
    pokemon_setattr(base, REQUEST_OT_GENDER, &partner->gender);
    pokemon_setattr(base, REQUEST_OT_NAME, &partner->name);

    /* TODO: Figure out how to make catch location blank */

    /* Set the provided attributes*/
    if (*pokemon->nickname != 0xFF) {
        pokemon_setattr(base, REQUEST_NICK, pokemon->nickname);
    }

    if (pokemon->item) {
        pokemon_setattr(base, REQUEST_HELD_ITEM, &pokemon->item);
    }

    for (u8 i = 0; i < 4; i++) {
        pokemon_setattr(base, REQUEST_MOVE1 + i, &pokemon->moves[i]);
    }
}

void partner_load_party(void)
{
    struct TagTeamPartner* partner = get_partner();

    u8 party_size = partner->party_size;
    if (party_size > TAG_TEAM_PARTNER_MAX_PARTY_SIZE) {
        party_size = TAG_TEAM_PARTNER_MAX_PARTY_SIZE;
    }

    for (u8 i = 0; i < TAG_TEAM_PARTNER_MAX_PARTY_SIZE; i++) {
        struct Pokemon* slot = &party_player[3 + i];

        if (i < party_size) {
            struct TagTeamPokemon* pokemon = &partner->party[i];

            /* TODO: Use a proper trainer ID */
            partner_load_pokemon(partner, rand(), slot, pokemon);
        } else {
            pokemon_slot_purge_full(slot);
        }
    }
}

bool is_partner_battle(void)
{
    /* TODO: Have a custom flag */
    return battle_type_flags & BATTLE_FLAG_MULTI;
}

pchar* partner_load_name(void)
{
    struct TagTeamPartner* partner = get_partner();
    return partner->name;
}

pchar* partner_load_class(void)
{
    struct TagTeamPartner* partner = get_partner();
    return trainer_class_names[partner->class];
}

u8 partner_load_backsprite_index(void)
{
    struct TagTeamPartner* partner = get_partner();
    return partner->sprite;
}

void multi_bx_partner(void);

void multi_bx_partner_init(void)
{
    b_x[b_active_side] = multi_bx_partner;
}

void multi_partner_finish_exec(void)
{
    b_x[b_active_side] = multi_bx_partner;
    b_buffers_awaiting_execution_bitfield &= ~(1u << b_active_side);
}

void multi_bx_partner(void)
{
    if (b_buffers_awaiting_execution_bitfield & (1u << b_active_side)) {
        u8* buffer = b_buffer_A[b_active_side];

        if (*buffer > COMMAND_MAX) {
            multi_partner_finish_exec();
        } else {
            multi_partner_table[*buffer]();
        }
    }
}

/* TODO: Hook tai_switching_and_items to make it look at the player
 * party (see emerald) */
static void partner_cmd14_pick_move(void)
{
    u16* moves = (u16*) &b_buffer_A[b_active_side];

    tai_prepare_struct();
    u8 move_slot = tai_pick_move();
    move_slot = 1;
    u16 move_index = moves[move_slot];

    struct MoveData* move = &pokemon_moves[move_index];

    /* FIXME: Targeting logic is broken.  */
    if (move->target & (MOVE_TARGET_USER | MOVE_TARGET_EVERYONE)) {
        b_defender = b_active_side;
    } else if (move->target & MOVE_TARGET_BOTH) {
        b_defender = 1;//battle_get_side_with_given_state(0);

        if (absent_flags_for_banks & (1 << b_defender)) {
            b_defender = 3;//battle_get_side_with_given_state(2);
        }
    } else {
        /* TODO: Add bank picking logic to AI scripts, as in Emerald */
        b_defender = rand() % 2 + 1;
    }

    dp01_build_cmdbuf_x21_a_bb(1, 10, move_slot | (b_defender << 8));
    multi_partner_finish_exec();
}

static void partner_cmd10_message(void)
{
    multi_partner_finish_exec();
}

static void partner_cmd11_message(void)
{
    multi_partner_finish_exec();
}

static void partner_cmd12_select_action(void)
{
    /* FIXME: Rewrite this function */
    ((void(*)(void)) (0x08039C84|1))();
    multi_partner_finish_exec();
}

#define WRAP_COMMAND_ADDRESS(x) ((void (*)(void)) (x))

/*
 * Most of these are just reused from the player's command table to
 * save rewriting them all.
 */
static void (*multi_partner_table[COMMAND_MAX])(void) = {
    WRAP_COMMAND_ADDRESS(0x8030B91),
    WRAP_COMMAND_ADDRESS(0x80313B1),
    WRAP_COMMAND_ADDRESS(0x8031439),
    WRAP_COMMAND_ADDRESS(0x8031E8D),
    WRAP_COMMAND_ADDRESS(0x8031F01),
    WRAP_COMMAND_ADDRESS(0x8031F69),
    WRAP_COMMAND_ADDRESS(0x8032161),
    WRAP_COMMAND_ADDRESS(0x803227D),
    WRAP_COMMAND_ADDRESS(0x8032429),
    WRAP_COMMAND_ADDRESS(0x8032591),
    WRAP_COMMAND_ADDRESS(0x8032651),
    WRAP_COMMAND_ADDRESS(0x803273D),
    WRAP_COMMAND_ADDRESS(0x803275D),
    WRAP_COMMAND_ADDRESS(0x80327B1),
    WRAP_COMMAND_ADDRESS(0x8032811),
    WRAP_COMMAND_ADDRESS(0x8032841),
    WRAP_COMMAND_ADDRESS(0x8032AFD),
    partner_cmd11_message,
    partner_cmd12_select_action,
    WRAP_COMMAND_ADDRESS(0x8032C49),
    partner_cmd14_pick_move,
    WRAP_COMMAND_ADDRESS(0x8032CED),
    WRAP_COMMAND_ADDRESS(0x8032D51),
    WRAP_COMMAND_ADDRESS(0x8032E29),
    WRAP_COMMAND_ADDRESS(0x8032E4D),
    WRAP_COMMAND_ADDRESS(0x8032F4D),
    WRAP_COMMAND_ADDRESS(0x8032FE9),
    WRAP_COMMAND_ADDRESS(0x8033061),
    WRAP_COMMAND_ADDRESS(0x80330C9),
    WRAP_COMMAND_ADDRESS(0x8033135),
    WRAP_COMMAND_ADDRESS(0x8033141),
    WRAP_COMMAND_ADDRESS(0x80331F5),
    WRAP_COMMAND_ADDRESS(0x8033225),
    WRAP_COMMAND_ADDRESS(0x8033231),
    WRAP_COMMAND_ADDRESS(0x8033245),
    WRAP_COMMAND_ADDRESS(0x8033259),
    WRAP_COMMAND_ADDRESS(0x803326D),
    WRAP_COMMAND_ADDRESS(0x8033281),
    WRAP_COMMAND_ADDRESS(0x803329D),
    WRAP_COMMAND_ADDRESS(0x80332D5),
    WRAP_COMMAND_ADDRESS(0x80332ED),
    WRAP_COMMAND_ADDRESS(0x8033315),
    WRAP_COMMAND_ADDRESS(0x8033385),
    WRAP_COMMAND_ADDRESS(0x8033391),
    WRAP_COMMAND_ADDRESS(0x80333D5),
    WRAP_COMMAND_ADDRESS(0x8033405),
    WRAP_COMMAND_ADDRESS(0x8033445),
    WRAP_COMMAND_ADDRESS(0x8033479),
    WRAP_COMMAND_ADDRESS(0x803376D),
    WRAP_COMMAND_ADDRESS(0x8033879),
    WRAP_COMMAND_ADDRESS(0x80338C9),
    WRAP_COMMAND_ADDRESS(0x80338ED),
    WRAP_COMMAND_ADDRESS(0x803394D),
    WRAP_COMMAND_ADDRESS(0x80339B5),
    WRAP_COMMAND_ADDRESS(0x8033A11),
    WRAP_COMMAND_ADDRESS(0x8033A79),
    WRAP_COMMAND_ADDRESS(0x8033AC5),
};
