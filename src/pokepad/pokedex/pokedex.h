#ifndef POKEPAD_POKEDEX_H
#define POKEPAD_POKEDEX_H

#include "../pokepad.h"

#define POKEDEX_GRID_HEIGHT 4
#define POKEDEX_GRID_WIDTH 6
#define POKEDEX_ICONS (POKEDEX_GRID_WIDTH * POKEDEX_GRID_HEIGHT)
#define POKEDEX_ICON_HEIGHT 32
#define POKEDEX_ICON_X(x) ((x) * 32 + 38)
#define POKEDEX_ICON_Y(y) ((y) * POKEDEX_ICON_HEIGHT + 36)

struct PokepadPokedexState {
    u8 icons[POKEDEX_ICONS];
    s16 index;
};

struct PokedexIconState {
    u8 id;
    s16 index;
    s16 x;
    s16 y;
};

struct PokedexScrollState {
    u8* icons;
    u8 icon_count;
    u8 shift;
    s8 direction;
    u8 rows;
    u8 row;
    u8 counter;
};

#endif /* POKEPAD_POKEDEX_H */
