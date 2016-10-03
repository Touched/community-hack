#ifndef PARTNER_H
#define PARTNER_H

bool is_partner_battle(void);
void partner_unload_pokemon(void);

#define TAG_TEAM_PARTNER_MAX_NAME_LEN 8
#define TAG_TEAM_PARTNER_MAX_PARTY_SIZE 3

struct TagTeamPokemon {
    pchar nickname[10];
    enum PokemonSpecies species;
    enum Item item;
    enum Move moves[4];
    u8 level;

    /**
     * If non-null, this function will be called with a random PID and
     * should return a new PID. This can be used to generate Pokemon
     * with a specific gender, nature or any other PID determined
     * factor.
     */
    u32 (*pid_hook)(u32 pid, struct TagTeamPokemon* pokemon, void* data);

    /**
     * The PID hook function will be passed this data, so you can
     * write generic functions.
     */
    void* pid_hook_data;

    /* TODO: Allow automatic move selection, IV/EV spread, PP, Origin info, etc. */
};

struct TagTeamPartner {
    /** The trainer's name. */
    pchar name[TAG_TEAM_PARTNER_MAX_NAME_LEN];

    enum Gender gender;

    /** The trainer's trainer class index. */
    u8 class;

    /**
     * The trainer's backsprite index.  In FR, the backsprite table is
     * at 0825DF50 (object templates) and the palettes are at 08239FD4
     * (the palette tag must equal sprite/table index).
     */
    u8 sprite;

    /** The amount of Pokemon they have in their party. Max 3 */
    u8 party_size;

    /** Actual party data */
    struct TagTeamPokemon party[TAG_TEAM_PARTNER_MAX_PARTY_SIZE];

    /** TODO: Items, AI flaags, trainer ID etc. */
};

#endif /* PARTNER_H */
