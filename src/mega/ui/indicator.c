#include <pokeagb/pokeagb.h>
#include "generated/images/indicators.h"

#define MEGA_INDICATOR_PALETTE_TAG 0x2345
#define MEGA_INDICATOR_GRAPHICS_TAG 0x2345

struct SpritePalette indicator_palette = { indicatorsPal, MEGA_INDICATOR_PALETTE_TAG };
struct SpriteTiles indicator_tiles = { indicatorsTiles, MEGA_INDICATOR_GRAPHICS_TAG };

void mega_ui_load_indicators(void)
{
    /* gpu_pal_obj_alloc_tag_and_apply(&indicator_palette); */
    /* gpu_tile_obj_decompress_alloc_tag_and_upload(&indicator_tiles); */
    /* template_instanciate_forward_search(&indicator_template, 0, 0, 0); */
}
