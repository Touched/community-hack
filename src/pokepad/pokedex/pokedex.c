#include "pokedex.h"
#include "generated/images/pokepad/icons/pokedex.h"

#define POKEPAD_POKEDEX_ICON_BASE_PAL_TAG 0xD000
#define POKEPAD_POKEDEX_ICON_BASE_GRAY_PAL_TAG 0xE000
#define POKEPAD_POKEDEX_ICON_BASE_TILE_TAG 0xD000

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

static void pokepad_pokedex_load_icon(enum PokemonSpecies species, bool grayscale,
                                      struct Template* template)
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
    memcpy(template, &pokedex_icon_template_base, sizeof(struct Template));
    template->tiles_tag = tiles.tag;
    template->pal_tag = palette.tag;

    /* Upload */
    gpu_tile_obj_alloc_tag_and_upload(&tiles);
    gpu_pal_obj_alloc_tag_and_apply(&palette);
}

static void load_icon_grid(void)
{
    struct PokepadPokedexState* state = (struct PokepadPokedexState*) pokepad_state->app_state;

    u8 i = 0;
    for (u8 y = 0; y < POKEDEX_GRID_HEIGHT; y++) {
        for (u8 x = 0; x < POKEDEX_GRID_WIDTH; x++) {
            u16 xpos = x * 32 + 38,
                ypos = y * 32 + 36;

            u16 index = pokedex_order[i + state->index];
            enum PokemonSpecies species = pokedex_index_to_species(index);
            u8 id;

            struct Template* template = &state->icons[0].templates[id];
            pokepad_pokedex_load_icon(species, false, template);

            /* if (dex_flag(index, DEX_FLAG_CHECK_CAUGHT, false)) { */
            /*     id = pokepad_pokedex_load_icon(species, false, xpos, ypos, 0); */
            /* } else if (dex_flag(index, DEX_FLAG_CHECK_SEEN, false)) { */
            /*     id = pokepad_pokedex_load_icon(species, true, xpos, ypos, 0); */
            /* } else { */
            /*     id = pokepad_pokedex_load_icon(SPECIES_MISSINGNO, false, xpos, ypos, 0); */
            /* } */

            id = template_instanciate_forward_search(template, xpos, ypos, 0);
            state->icons[0].ids[i++] = id;
        }
    }
}

static void change_page(s8 amount)
{
    struct PokepadPokedexState* state = (struct PokepadPokedexState*) pokepad_state->app_state;
    u8 pages = amount < 0 ? -amount : amount;

    /* if (amount < 0) { */
    /*     if (amount >= pages) { */
    /*         state->page -= pages; */
    /*     } else { */
    /*         state->page = 0; */
    /*     } */
    /* } else { */
    /*     /\* TODO: Clamp page max *\/ */
    /*     state->page += pages; */
    /* } */

    state->index++;

    /* Swap page */
    for (u8 i = 0; i < POKEDEX_ICONS; i++) {
        obj_delete_and_free(&objects[state->icons[0].ids[i]]);
    }

    /* memcpy(state->icons_page, state->icons, sizeof(state->icons)); */
    load_icon_grid();
}

static bool setup(u8* trigger)
{
    struct PokepadPokedexState* state = (struct PokepadPokedexState*) pokepad_state->app_state;

    switch (*trigger) {
    case 0:
        pokepad_state->app_state = malloc_and_clear(sizeof(struct PokepadPokedexState));
        *trigger = 1;
        break;

    case 1:
        load_icon_grid();
        return false;
    }

    return true;
}

static bool destroy(u8* trigger)
{
    return false;
}

static void callback(void)
{
    if (super.buttons.held & KEY_L) {
        /* Page Left */
        change_page(-1);
    } else if (super.buttons.held & KEY_R) {
        /* Page Right */
        change_page(1);
    }
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
