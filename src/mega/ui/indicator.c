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

const struct Template indicator_template = {
    MEGA_INDICATOR_GRAPHICS_TAG,
    MEGA_INDICATOR_PALETTE_TAG,
    &indicator_oam,
    SPRITE_NO_ANIMATION,
    NULL,
    SPRITE_NO_ROTSCALE,
    oac_nullsub,
};

void mega_ui_load_indicator(u8 side)
{
    u8 id = template_instanciate_forward_search(&indicator_template, 0, 0, 0);

    struct MegaEvolutionUI* ui = &extension_state.mega_evolution->ui;
    ui->indicators[side] = id;

    /* Hide the indicator */
    objects[id].bitfield2 |= 4;
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

void mega_ui_indicator_blink_callback(struct Object* object)
{
    if (object->private[0]++ >= FPS / 2) {
        /* Toggle visibility */
        object->bitfield2 ^= 4;
        object->private[0] = 0;
    }
}

void mega_ui_indicator_blinking(enum BattleBank bank, bool enable)
{
    struct MegaEvolutionUI* ui = &extension_state.mega_evolution->ui;
    u8 id = ui->indicators[bank];
    struct Object* object = &objects[id];

    if (enable) {
        object->callback = mega_ui_indicator_blink_callback;
        object->private[0] = 0;
        object->bitfield2 &= ~4; /* show */
    } else {
        object->callback = oac_nullsub;
        object->bitfield2 |= 4; /* hide */
    }
}

void mega_ui_indicator_type(enum BattleBank bank, u8 type)
{
    struct MegaEvolutionUI* ui = &extension_state.mega_evolution->ui;
    u8 id = ui->indicators[bank];

    /* Change the tile the indicator uses relative to the first tile */
    objects[id].final_oam.tile_num = objects[id].anim_data_offset + type;
}
