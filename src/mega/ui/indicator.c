#include <pokeagb/pokeagb.h>
#include "generated/images/indicators.h"

#define MEGA_INDICATOR_PALETTE_TAG 0x1234
#define MEGA_INDICATOR_GRAPHICS_TAG 0x2345

struct SpritePalette indicator_palette = {
    indicatorsPal,
    MEGA_INDICATOR_PALETTE_TAG
};

struct SpriteTiles indicator_tiles = {
    indicatorsTiles,
    indicatorsTilesLen,
    MEGA_INDICATOR_GRAPHICS_TAG
};

struct OamData indicator_oam = {};

const struct Template indicator_template = {
    MEGA_INDICATOR_GRAPHICS_TAG,
    MEGA_INDICATOR_PALETTE_TAG,
    &indicator_oam,
    SPRITE_NO_ANIMATION,
    NULL,
    SPRITE_NO_ROTSCALE,
    oac_nullsub,
};

void mega_ui_load_indicators(u8 state)
{
    /* When the healthbar palettes are loaded */
    if (state == 1) {
        gpu_pal_obj_alloc_tag_and_apply(&indicator_palette);
        gpu_tile_obj_decompress_alloc_tag_and_upload(&indicator_tiles);
        template_instanciate_forward_search(&indicator_template, 10, 10, 0);
    }
}
