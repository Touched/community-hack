#include <pokeagb/pokeagb.h>
#include "pokepad.h"

#define POKEPAD_MENU_ICON_TAG_BASE 0xF123

static struct OamData app_icon_oam = {
    .size = 3
};

static hblank_handler(void)
{
    pokepad_hblank_handler();

    if (REG_VCOUNT >= 120) {
        /* Enable darkening on BG1 and backdrop */
        REG_BLDCNT = BLDCNT_DARKEN | BLDCNT_BG1_SRC | BLDCNT_BACKDROP_SRC;
    } else {
        REG_BLDCNT = 0;
    }
}

static void load_applications(void)
{
    const struct PokepadApplication** it = NULL;
    u16 x = 36;

    while ((it = pokepad_application_next(it))) {
        const struct PokepadApplication* app = *it;

        /* Dynamically generate the icon template */
        const struct SpritePalette palette = {
            app->icon.palette,
            POKEPAD_MENU_ICON_TAG_BASE + pokepad_application_id(it),
        };

        const struct SpriteTiles tiles = {
            app->icon.tiles,
            app->icon.size,
            POKEPAD_MENU_ICON_TAG_BASE + pokepad_application_id(it),
        };

        const struct Template template = {
            tiles.tag,
            palette.tag,
            &app_icon_oam,
            SPRITE_NO_ANIMATION,
            NULL,
            SPRITE_NO_ROTSCALE,
            oac_nullsub,
        };

        gpu_pal_obj_alloc_tag_and_apply(&palette);
        gpu_tile_obj_alloc_tag_and_upload(&tiles);
        template_instanciate_forward_search(&template, x, 55, 0);
        x += 56;
    }
}

static bool setup(u8* trigger)
{
    switch (*trigger) {
    case 0:
        /* Create the application description box using HBlank */
        hblank_handler_set(hblank_handler);
        lcd_io_set(REG_ID_BLDY, 8);

        *trigger = 1;
        break;

    case 1:
        load_applications();
        *trigger = 2;
        break;

    default:
        return false;
    }

    return true;
}

static bool destroy(u8* trigger)
{
    return false;
}

const struct PokepadApplication pokepad_application_main = {
    .name = _"PokéPad",
    .description = "Main application",
    .role = POKEPAD_APP_MAIN,
    .setup = setup,
    .destroy = destroy,
};
