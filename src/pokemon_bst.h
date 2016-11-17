#include <pokeagb/pokeagb.h>

struct PokemonBaseStatNew {
    u8 hp;
    u8 atk;
    u8 def;
    u8 spd;
    u8 spatk;
    u8 spdef;
    enum PokemonType type[2];
    u8 catch_rate;
    u8 xp_yield;
    u16 effort_yield;
    enum Item item[2];
    u8 gender_ratio;
    u8 steps_to_hatch;
    u8 base_friendship;
    enum PokemonExpGrowth exp_growth;
    enum PokemonEggGroup eggroup[2];
    enum PokemonAbility ability[2];
    u8 safari_zone_flee_rate;
    enum PokemonColor color;
    enum PokemonAbility hidden_ability;
    u8 padding;
};
