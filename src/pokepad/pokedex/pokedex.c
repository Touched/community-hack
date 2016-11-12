#include "../pokepad.h"
#include "generated/images/pokepad/icons/pokedex.h"

#define POKEPAD_POKEDEX_ICON_BASE_PAL_TAG 0xD000
#define POKEPAD_POKEDEX_ICON_BASE_GRAY_PAL_TAG 0xE000
#define POKEPAD_POKEDEX_ICON_BASE_TILE_TAG 0xD000
#define POKEDEX_GRID_HEIGHT 4
#define POKEDEX_GRID_WIDTH 6

const struct OamData icon_oam = {
    .size = 2,
};

static const struct Template pokedex_icon_template_base = {
    0,
    0,
    &icon_oam,
    SPRITE_NO_ANIMATION,
    NULL,
    SPRITE_NO_ROTSCALE,
    oac_nullsub,
};

static u8 pokepad_pokedex_load_icon(enum PokemonSpecies species, bool grayscale,
                                    u8 x, u8 y, u8 priority)
{
    /* Unown have a separate icon for each form */
    u32 pid = 0;
    if (species == SPECIES_UNOWN) {
        pid = saveblock2->unown_pid;
    }

    struct SpriteTiles tiles = {
        .data = load_party_icon_tiles_with_form(species, pid, true),
        .size = 576,
        .tag = POKEPAD_POKEDEX_ICON_BASE_TILE_TAG + species,
    };

    u8 palette_index = load_party_icon_palette_index(species);

    /* Build palette */
    u16 palette_data[16];
    u16 palette_tag_base = grayscale ? POKEPAD_POKEDEX_ICON_BASE_GRAY_PAL_TAG
        : POKEPAD_POKEDEX_ICON_BASE_PAL_TAG;

    memcpy(palette_data, pokeicon_pals[palette_index], sizeof(palette_data));

    if (grayscale) {
        for (u8 i = 0; i < 16; i++) {
            struct ColorComponents* color = (struct ColorComponents*) &palette_data[i];

            u16 lum = (color->r * 0x4C + color->g * 0x97 + color->b * 0x1D) >> 8;

            union Color gray = {
                .components = {
                    .r = lum,
                    .g = lum,
                    .b = lum,
                }
            };

            palette_data[i] = gray.packed;
        }
    }

    struct SpritePalette palette = {
        .data = palette_data,
        .tag = palette_tag_base + palette_index,
    };

    /* Build the template */
    struct Template template = pokedex_icon_template_base;
    template.tiles_tag = tiles.tag;
    template.pal_tag = palette.tag;

    /* Upload */
    gpu_tile_obj_alloc_tag_and_upload(&tiles);
    gpu_pal_obj_alloc_tag_and_apply(&palette);

    return template_instanciate_forward_search(&template, x, y, priority);
}

static bool setup(u8* trigger)
{
    /* TODO: Dex order */
    enum PokemonSpecies species = SPECIES_MISSINGNO;

    dex_flag(SPECIES_PIDGEOT, DEX_FLAG_SET_CAUGHT, true);


    for (u8 y = 0; y < POKEDEX_GRID_HEIGHT; y++) {
        for (u8 x = 0; x < POKEDEX_GRID_WIDTH; x++) {
            u16 xpos = x * 32 + 38,
                ypos = y * 32 + 36;

            if (dex_flag(species, DEX_FLAG_CHECK_CAUGHT, true)) {
                pokepad_pokedex_load_icon(species, false, xpos, ypos, 0);
            } else if (dex_flag(species, DEX_FLAG_CHECK_SEEN, true)) {
                pokepad_pokedex_load_icon(species, true, xpos, ypos, 0);
            } else {
                pokepad_pokedex_load_icon(SPECIES_MISSINGNO, false, xpos, ypos, 0);
            }

            species++;
        }
    }

    return false;
}

static bool destroy(u8* trigger)
{
    return false;
}

static void callback(void)
{

}

static bool unlocked(void)
{
    /* FIXME: Check Pokedex flag */
    return true;
}

static const pchar description[] = _"An application that records Pokémon\nsecrets "
    "upon meeting or catching them.";

const struct PokepadApplication pokepad_application_pokedex = {
    .name = _"Pokédex",
    .description = description,
    .role = POKEPAD_APP_EXTENSION,
    .icon = POKEPAD_BUILD_APP_ICON(pokedex),
    .setup = setup,
    .destroy = destroy,
    .callback = callback,
    .unlocked = unlocked,
};
