#include <pokeagb/pokeagb.h>
#include "config.h"
#include "partner.h"
#include "battle.h"

#define COMMAND_MAX 0x39

static void (*multi_partner_table[COMMAND_MAX])(void);

struct TagTeamPartner tag_team_partners[] = {
    {
        .name = _("MAY"),
        .gender = GENDER_FEMALE,
        .class = 1,
        .sprite = 3,
        .party_size = 2,
        {
            {
                .nickname = _(""),
                .species = SPECIES_CHARMELEON,
                .item = ITEM_FIRESTONE,
                .moves = { MOVE_FLAMETHROWER, MOVE_SURF },
                .level = 40
            },
            {
                .nickname = _(""),
                .species = SPECIES_DIGLETT,
                .item = ITEM_ORANBERRY,
                .moves = { MOVE_MUD_SHOT, MOVE_SURF },
                .level = 1
            }
        }
    }
};

#define TAG_TEAM_MAX_PARTNER_INDEX (sizeof(tag_team_partners) / sizeof(struct TagTeamPartner))

struct TagTeamPartner* get_partner(void)
{
    u16 index = var_load(BATTLE_PARTNER_ID_VAR);

    if (index >= TAG_TEAM_MAX_PARTNER_INDEX) {
        index = 0;
    }

    return &tag_team_partners[index];
}

u16 partner_count_item(enum Item item)
{
    /* TODO: Allow partner's to have items */
    return 0;
}

void partner_unload_pokemon(void)
{
    memcpy(&party_player[3], extension_state.party_backup, sizeof(struct Pokemon) * 3);
    free(extension_state.party_backup);
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
    /*
     * Back up the player's party so we can partially overwrite it
     * with the partner's team.
     */
    extension_state.party_backup = malloc(sizeof(struct Pokemon) * 3);
    if (!extension_state.party_backup) {
        return;
    }
    memcpy(extension_state.party_backup, &party_player[3], sizeof(struct Pokemon) * 3);

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
    return battle_type_flags & BATTLE_FLAG_TAG_TEAM;
}

bool is_link_or_partner_battle(void)
{
    return is_partner_battle() || (battle_type_flags & BATTLE_FLAG_PARTNER);
}

bool is_partner_attacker(void)
{
    return battle_get_per_side_status(b_attacker) == BANK_PLAYER_ALLY;
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

static void partner_cmd14_pick_move(void)
{
    u16* moves = (u16*) &b_buffer_A[b_active_side][4];

    u8 move_count = 0;
    for (u8 i = 0; i < 4; i++) {
        if (moves[i]) {
            move_count++;
        }
    }

    u8 move_slot = rand() % move_count;
    u16 move_index = moves[move_slot];

    struct MoveData* move = &pokemon_moves[move_index];
    if (move->target & (MOVE_TARGET_USER | MOVE_TARGET_EVERYONE)) {
        b_defender = b_active_side;
    } else if (move->target & MOVE_TARGET_BOTH) {
        b_defender = 1;

        if (absent_flags_for_banks & (1 << b_defender)) {
            b_defender = 3;
        }
    } else {
        b_defender = ((rand() % 2) << 1) | 1;
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
    /* Attack */
    dp01_build_cmdbuf_x21_a_bb(1, 0, (b_active_side ^ 1) << 8);

    /* Switch */
    /* dp01_build_cmdbuf_x21_a_bb(1, 2, 0); */

    /* Bag */
    /* dp01_build_cmdbuf_x21_a_bb(1, 1, 0); */

    multi_partner_finish_exec();
}

static void partner_cmd16_switch(void)
{
    /* TODO: Actually pick a viable Pokemon instead of just picking
     * the next available one */
    u8 i = 0;
    for (i = 3; i < 6; i++) {
        if (i == b_pokemon_team_id_by_side[BANK_PLAYER] ||
            i == b_pokemon_team_id_by_side[BANK_PLAYER_ALLY]) {
            continue;
        }

        if (pokemon_getattr(&party_player[i], REQUEST_CURRENT_HP, NULL)) {
            break;
        }
    }

    /* Perform switchout */
    battle_stuff->team_id_by_side_5C[b_active_side] = i;
    dp01_build_cmdbuf_x22(1, i, 0);
    multi_partner_finish_exec();
}

#define WRAP_COMMAND_ADDRESS(x) ((void (*)(void)) (x))

/*
 * Most of these are just reused from the player's command table to
 * save rewriting them all.
 */
static void (*multi_partner_table[COMMAND_MAX])(void) = {
    WRAP_COMMAND_ADDRESS(0x8030B91), /* 00 */
    WRAP_COMMAND_ADDRESS(0x80313B1), /* 01 */
    WRAP_COMMAND_ADDRESS(0x8031439), /* 02 */
    WRAP_COMMAND_ADDRESS(0x8031E8D), /* 03 */
    WRAP_COMMAND_ADDRESS(0x8031F01), /* 04 */
    WRAP_COMMAND_ADDRESS(0x8031F69), /* 05 */
    WRAP_COMMAND_ADDRESS(0x8032161), /* 06 */
    WRAP_COMMAND_ADDRESS(0x803227D), /* 07 */
    WRAP_COMMAND_ADDRESS(0x8032429), /* 08 */
    WRAP_COMMAND_ADDRESS(0x8032591), /* 09 */
    WRAP_COMMAND_ADDRESS(0x8032651), /* 0a */
    WRAP_COMMAND_ADDRESS(0x803273D), /* 0b */
    WRAP_COMMAND_ADDRESS(0x803275D), /* 0c */
    WRAP_COMMAND_ADDRESS(0x80327B1), /* 0d */
    WRAP_COMMAND_ADDRESS(0x8032811), /* 0e */
    WRAP_COMMAND_ADDRESS(0x8032841), /* 0f */
    WRAP_COMMAND_ADDRESS(0x8032AFD), /* 10 */
    partner_cmd11_message,           /* 11 */
    partner_cmd12_select_action,     /* 12 */
    WRAP_COMMAND_ADDRESS(0x8032C49), /* 13 */
    partner_cmd14_pick_move,         /* 14 */
    WRAP_COMMAND_ADDRESS(0x8032CED), /* 15 */
    partner_cmd16_switch,            /* 16 */
    WRAP_COMMAND_ADDRESS(0x8032E29), /* 17 */
    WRAP_COMMAND_ADDRESS(0x8032E4D), /* 18 */
    WRAP_COMMAND_ADDRESS(0x8032F4D), /* 19 */
    WRAP_COMMAND_ADDRESS(0x8032FE9), /* 1a */
    WRAP_COMMAND_ADDRESS(0x8033061), /* 1b */
    WRAP_COMMAND_ADDRESS(0x80330C9), /* 1c */
    WRAP_COMMAND_ADDRESS(0x8033135), /* 1d */
    WRAP_COMMAND_ADDRESS(0x8033141), /* 1e */
    WRAP_COMMAND_ADDRESS(0x80331F5), /* 1f */
    WRAP_COMMAND_ADDRESS(0x8033225), /* 20 */
    WRAP_COMMAND_ADDRESS(0x8033231), /* 21 */
    WRAP_COMMAND_ADDRESS(0x8033245), /* 22 */
    WRAP_COMMAND_ADDRESS(0x8033259), /* 23 */
    WRAP_COMMAND_ADDRESS(0x803326D), /* 24 */
    WRAP_COMMAND_ADDRESS(0x8033281), /* 25 */
    WRAP_COMMAND_ADDRESS(0x803329D), /* 26 */
    WRAP_COMMAND_ADDRESS(0x80332D5), /* 27 */
    WRAP_COMMAND_ADDRESS(0x80332ED), /* 28 */
    WRAP_COMMAND_ADDRESS(0x8033315), /* 29 */
    WRAP_COMMAND_ADDRESS(0x8033385), /* 2a */
    WRAP_COMMAND_ADDRESS(0x8033391), /* 2b */
    WRAP_COMMAND_ADDRESS(0x80333D5), /* 2c */
    WRAP_COMMAND_ADDRESS(0x8033405), /* 2d */
    WRAP_COMMAND_ADDRESS(0x8033445), /* 2e */
    WRAP_COMMAND_ADDRESS(0x8033479), /* 2f */
    WRAP_COMMAND_ADDRESS(0x803376D), /* 30 */
    WRAP_COMMAND_ADDRESS(0x8033879), /* 31 */
    WRAP_COMMAND_ADDRESS(0x80338C9), /* 32 */
    WRAP_COMMAND_ADDRESS(0x80338ED), /* 33 */
    WRAP_COMMAND_ADDRESS(0x803394D), /* 34 */
    WRAP_COMMAND_ADDRESS(0x80339B5), /* 35 */
    WRAP_COMMAND_ADDRESS(0x8033A11), /* 36 */
    WRAP_COMMAND_ADDRESS(0x8033A79), /* 37 */
    WRAP_COMMAND_ADDRESS(0x8033AC5), /* 38 */
};
