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

#define WRAP_COMMAND(name, address)                     \
    void name(void) {                                   \
        ((void (*)(void)) address)();                   \
        if ((u32) b_x[b_active_side] == 0x0802E3B4 + 1) {        \
            multi_partner_finish_exec();                \
        }                                               \
    }

#define WRAP_COMMAND2(name, address)                    \
    void name(void) {                                   \
        ((void (*)(void)) address)();                   \
        if ((u32) b_x[b_active_side] == 0x08035A98 + 1) {        \
            multi_partner_finish_exec();                \
        }                                               \
    }

WRAP_COMMAND(player_cmd00, 0x8030B91);
WRAP_COMMAND(player_cmd01, 0x80313B1);
WRAP_COMMAND(player_cmd02, 0x8031439);
WRAP_COMMAND(player_cmd03, 0x8031E8D);
WRAP_COMMAND(player_cmd04, 0x8031F01);
WRAP_COMMAND(player_cmd05, 0x8031F69);
WRAP_COMMAND(player_cmd06, 0x8032161);
WRAP_COMMAND(player_cmd07, 0x803227D);
WRAP_COMMAND(player_cmd08, 0x8032429);
WRAP_COMMAND(player_cmd09, 0x8032591);
WRAP_COMMAND(player_cmd0a, 0x8032651);
WRAP_COMMAND(player_cmd0b, 0x803273D);
WRAP_COMMAND(player_cmd0c, 0x803275D);
WRAP_COMMAND(player_cmd0d, 0x80327B1);
WRAP_COMMAND(player_cmd0e, 0x8032811);
WRAP_COMMAND(player_cmd0f, 0x8032841);
WRAP_COMMAND(player_cmd10, 0x8032AFD);
WRAP_COMMAND(player_cmd11, 0x8032B71);
WRAP_COMMAND(player_cmd12, 0x8032BD5);
WRAP_COMMAND(player_cmd13, 0x8032C49);
WRAP_COMMAND(player_cmd14, 0x8032C91);
WRAP_COMMAND(player_cmd15, 0x8032CED);
WRAP_COMMAND(player_cmd16, 0x8032D51);
WRAP_COMMAND(player_cmd17, 0x8032E29);
WRAP_COMMAND(player_cmd18, 0x8032E4D);
WRAP_COMMAND(player_cmd19, 0x8032F4D);
WRAP_COMMAND(player_cmd1a, 0x8032FE9);
WRAP_COMMAND(player_cmd1b, 0x8033061);
WRAP_COMMAND(player_cmd1c, 0x80330C9);
WRAP_COMMAND(player_cmd1d, 0x8033135);
WRAP_COMMAND(player_cmd1e, 0x8033141);
WRAP_COMMAND(player_cmd1f, 0x80331F5);
WRAP_COMMAND(player_cmd20, 0x8033225);
WRAP_COMMAND(player_cmd21, 0x8033231);
WRAP_COMMAND(player_cmd22, 0x8033245);
WRAP_COMMAND(player_cmd23, 0x8033259);
WRAP_COMMAND(player_cmd24, 0x803326D);
WRAP_COMMAND(player_cmd25, 0x8033281);
WRAP_COMMAND(player_cmd26, 0x803329D);
WRAP_COMMAND(player_cmd27, 0x80332D5);
WRAP_COMMAND(player_cmd28, 0x80332ED);
WRAP_COMMAND(player_cmd29, 0x8033315);
WRAP_COMMAND(player_cmd2a, 0x8033385);
WRAP_COMMAND(player_cmd2b, 0x8033391);
WRAP_COMMAND(player_cmd2c, 0x80333D5);
WRAP_COMMAND(player_cmd2d, 0x8033405);
WRAP_COMMAND(player_cmd2e, 0x8033445);
WRAP_COMMAND(player_cmd2f, 0x8033479);
WRAP_COMMAND(player_cmd30, 0x803376D);
WRAP_COMMAND(player_cmd31, 0x8033879);
WRAP_COMMAND(player_cmd32, 0x80338C9);
WRAP_COMMAND(player_cmd33, 0x80338ED);
WRAP_COMMAND(player_cmd34, 0x803394D);
WRAP_COMMAND(player_cmd35, 0x80339B5);
WRAP_COMMAND(player_cmd36, 0x8033A11);
WRAP_COMMAND(player_cmd37, 0x8033A79);
WRAP_COMMAND(player_cmd38, 0x8033AC5);

WRAP_COMMAND2(enemy_cmd12, 0x08038595);
WRAP_COMMAND2(enemy_cmd14, 0x080385B1);

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


static void (*multi_partner_table[COMMAND_MAX])(void) = {
    player_cmd00,
    player_cmd01,
    player_cmd02,
    player_cmd03,
    player_cmd04,
    player_cmd05,
    player_cmd06,
    player_cmd07,               /* see 0805B664 */
    player_cmd08,
    player_cmd09,
    player_cmd0a,
    player_cmd0b,
    player_cmd0c,
    player_cmd0d,
    player_cmd0e,
    player_cmd0f,
    partner_cmd10_message,
    partner_cmd11_message,
    enemy_cmd12,
    player_cmd13,
    partner_cmd14_pick_move,
    player_cmd15,
    player_cmd16,
    player_cmd17,
    player_cmd18,
    player_cmd19,
    player_cmd1a,
    player_cmd1b,
    player_cmd1c,
    player_cmd1d,
    player_cmd1e,
    player_cmd1f,
    player_cmd20,
    player_cmd21,
    player_cmd22,
    player_cmd23,
    player_cmd24,
    player_cmd25,
    player_cmd26,
    player_cmd27,
    player_cmd28,
    player_cmd29,
    player_cmd2a,
    player_cmd2b,
    player_cmd2c,
    player_cmd2d,
    player_cmd2e,
    player_cmd2f,
    player_cmd30,
    player_cmd31,
    player_cmd32,
    player_cmd33,
    player_cmd34,
    player_cmd35,
    player_cmd36,
    player_cmd37,
    player_cmd38,
};
