#ifndef DEVNAV_HUD_H_
#define DEVNAV_HUD_H_

#include <pokeagb/pokeagb.h>

#define ICONX 0x10
#define ICONY 0x92

// 60 frames per second. 30 seconds is the time out. Max of 1092 seconds allowed
#define DEXNAV_TIMEOUT 60 * 500

// chance of encountering egg move at search levels
#define SEARCHLEVEL0_MOVECHANCE 21
#define SEARCHLEVEL5_MOVECHANCE 46
#define SEARCHLEVEL10_MOVECHANCE 58
#define SEARCHLEVEL25_MOVECHANCE 63
#define SEARCHLEVEL50_MOVECHANCE 65
#define SEARCHLEVEL100_MOVECHANCE 83

// chance of encountering hidden abilities at search levels
#define SEARCHLEVEL0_ABILITYCHANCE 0
#define SEARCHLEVEL5_ABILITYCHANCE 0
#define SEARCHLEVEL10_ABILITYCHANCE 5
#define SEARCHLEVEL25_ABILITYCHANCE 15
#define SEARCHLEVEL50_ABILITYCHANCE 20
#define SEARCHLEVEL100_ABILITYCHANCE 23

// chance of encountering one star potential
#define SEARCHLEVEL0_ONESTAR 0
#define SEARCHLEVEL5_ONESTAR 14
#define SEARCHLEVEL10_ONESTAR 17
#define SEARCHLEVEL25_ONESTAR 17
#define SEARCHLEVEL50_ONESTAR 15
#define SEARCHLEVEL100_ONESTAR 8

// chance of encountering two star potential
#define SEARCHLEVEL0_TWOSTAR 0
#define SEARCHLEVEL5_TWOSTAR 1
#define SEARCHLEVEL10_TWOSTAR 9
#define SEARCHLEVEL25_TWOSTAR 16
#define SEARCHLEVEL50_TWOSTAR 17
#define SEARCHLEVEL100_TWOSTAR 24

// chance of encountering three star potential
#define SEARCHLEVEL0_THREESTAR 0
#define SEARCHLEVEL5_THREESTAR 0
#define SEARCHLEVEL10_THREESTAR 1
#define SEARCHLEVEL25_THREESTAR 7
#define SEARCHLEVEL50_THREESTAR 6
#define SEARCHLEVEL100_THREESTAR 12

// chance of encountering held item
#define SEARCHLEVEL0_ITEM 0
#define SEARCHLEVEL5_ITEM 0
#define SEARCHLEVEL10_ITEM 1
#define SEARCHLEVEL25_ITEM 7
#define SEARCHLEVEL50_ITEM 6
#define SEARCHLEVEL100_ITEM 12

struct DexnavHudData {
    u16 species;
    enum Move move_id[4]; // u16
    enum Item held_item;
    enum PokemonAbility ability; // u8
    u8 potential;
    u8 sight_level;
    u8 search_level;
    u8 pokemon_level;
    u8 move_name_length;
    u8 proximity;
    u8 environment;
    s16 tile_x; // position of shaking grass
    s16 tile_y;
    u8 obj_id_species;
    u8 obj_id_sight;
    u8 obj_id_ability;
    u8 obj_id_move;
    u8 obj_id_item;
    u8 obj_id_shaking_grass;
    u8 obj_id_potential[3];
    u8 movement_times;
};



#endif /* DEVNAV_HUD_H_ */
