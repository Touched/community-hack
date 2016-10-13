#include <pokeagb/pokeagb.h>
#include "generated/images/indicators.h"
#include "battle.h"
#include "mega/mega.h"

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

void mega_ui_indicator_callback(struct Object* object);

const struct Template indicator_template = {
    MEGA_INDICATOR_GRAPHICS_TAG,
    MEGA_INDICATOR_PALETTE_TAG,
    &indicator_oam,
    SPRITE_NO_ANIMATION,
    NULL,
    SPRITE_NO_ROTSCALE,
    mega_ui_indicator_callback,
};

struct IndicatorCallbackPrivate {
    u8 side;
    u8 frames;
};

void mega_ui_load_indicator(u8 side)
{
    u8 id = template_instanciate_forward_search(&indicator_template, 10, 10, 0);

    struct MegaEvolutionUI* ui = &extension_state.mega_evolution->ui;
    ui->indicators[side] = id;

    struct IndicatorCallbackPrivate* state = (struct IndicatorCallbackPrivate*) objects[id].private;
    state->side = side;
    state->frames = 0;
}

void mega_ui_load_indicators(u8 state)
{
    /* When the healthbar palettes are loaded */
    if (state == 1) {
        struct MegaEvolutionUI* ui = &extension_state.mega_evolution->ui;

        gpu_pal_obj_alloc_tag_and_apply(&indicator_palette);
        gpu_tile_obj_decompress_alloc_tag_and_upload(&indicator_tiles);

        for (u8 i = 0; i < b_num_active_sides; i++) {
            mega_ui_load_indicator(i);
        }
    }
}

void mega_ui_indicator_callback(struct Object* object)
{
    struct MegaEvolutionState* mega = extension_state.mega_evolution;
    struct IndicatorCallbackPrivate* state = (struct IndicatorCallbackPrivate*) object->private;

    if (mega->trigger[state->side]) {
        /* Blink */
        if (state->frames-- <= 0) {
            object->bitfield2 ^= 4;
            state->frames = FPS / 2;
        }
    } else {
        state->frames = 0;

        /* Hidden */
        object->bitfield2 |= 4;

        /* object->bitfield2 &= ~4; */
    }
}
