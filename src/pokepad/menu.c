#include <pokeagb/pokeagb.h>
#include "pokepad.h"
#include "generated/images/pokepad/menu/arrow.h"
#include "generated/images/pokepad/menu/page_indicator.h"

#define POKEPAD_MENU_ICON_TAG_BASE 0xF123
#define POKEPAD_MENU_ICON_X 36
#define POKEPAD_MENU_ICON_Y 55
#define POKEPAD_MENU_ICON_SIZE 42
#define POKEPAD_MENU_PAGE_INDICATOR_WIDTH 8
#define POKEPAD_APP_NAME_WIDTH 8

struct PokepadMenuState {
    struct {
        u8 app_name;
        u8 app_description;
    } textboxes;
};

static struct OamData app_icon_oam = {
    .size = 3
};

static void menu_arrow_update_position(u8 id);

static struct OamData menu_arrow_oam = {};
const struct SpritePalette menu_arrow_palette = { arrowPal, 0xF000 };
const struct SpriteTiles menu_arrow_tiles = { arrowTiles, arrowTilesLen, 0xF000 };
const struct Template menu_arrow_template = {
    0xF000, 0xF000,
    &menu_arrow_oam,
    SPRITE_NO_ANIMATION,
    NULL,
    SPRITE_NO_ROTSCALE,
    oac_pingpong,
};

static void oac_page_indicator(struct Object*);
static struct OamData menu_page_indicator_oam = {};
const struct SpritePalette menu_page_indicator_palette = { page_indicatorPal, 0xF001 };
const struct SpriteTiles menu_page_indicator_tiles = { page_indicatorTiles,
                                                       page_indicatorTilesLen, 0xF001 };
const struct Template menu_page_indicator_template = {
    0xF001, 0xF001,
    &menu_page_indicator_oam,
    SPRITE_NO_ANIMATION,
    NULL,
    SPRITE_NO_ROTSCALE,
    oac_page_indicator,
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

static void load_page_indicators(void)
{
    u8 pages = 3;
    gpu_pal_obj_alloc_tag_and_apply(&menu_page_indicator_palette);
    gpu_tile_obj_alloc_tag_and_upload(&menu_page_indicator_tiles);

    u16 width = pages * POKEPAD_MENU_PAGE_INDICATOR_WIDTH;
    u16 x = (SCREEN_WIDTH - width + POKEPAD_MENU_PAGE_INDICATOR_WIDTH) / 2;

    for (u8 i = 0; i < pages; i++) {
        u8 id = template_instanciate_forward_search(&menu_page_indicator_template,
                                                    x + i * 8, 104, 0);
        objects[id].private[0] = i;
    }
}

static void load_applications(void)
{
    const struct PokepadApplication** it = NULL;
    u16 x = POKEPAD_MENU_ICON_X;

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
        template_instanciate_forward_search(&template, x, POKEPAD_MENU_ICON_Y, 0);
        x += 56;
    }
}

static struct Textbox pokepad_menu_textboxes[2] = {
    {
        0,
        ((SCREEN_WIDTH / 8) - POKEPAD_APP_NAME_WIDTH) / 2,
        10,
        POKEPAD_APP_NAME_WIDTH,
        2,
        0xF,
        0x8F,
        0x20A00,
    }, {
        0, 1, 16, 0x1C, 3, 0xF, 0x100, 0x20A00,
    }
};

static struct TextColor pokepad_menu_color = {
    0, 1, 2,
};

static void update_app_description(u8 id, const struct PokepadApplication* app)
{
    rboxid_clear_pixels(id, 0x0);
    rboxid_print(id, 1, 0, 0, &pokepad_menu_color, 0xFF, app->description);
    rboxid_tilemap_update(id);
    rboxid_update(id, 3);
}

static void update_app_name(u8 id, const struct PokepadApplication* app)
{
    u32 width = font_get_width_of_string(1, app->name, 0);
    u8 x = (rboxes[id].width * 8 - width) / 2;

    rboxid_clear_pixels(id, 0x0);
    rboxid_print(id, 1, x, 0, &pokepad_menu_color, 0xFF, app->name);
    rboxid_tilemap_update(id);
    rboxid_update(id, 3);
}

static void update_app_textboxes(void)
{
    struct PokepadMenuState* state = (struct PokepadMenuState*) pokepad_state->app_state;

    /* TODO: Load currently selected app */
    const struct PokepadApplication* app = pokepad_state->current_app;

    update_app_description(state->textboxes.app_description, app);
    update_app_name(state->textboxes.app_name, app);
}


static bool setup(u8* trigger)
{
    struct PokepadMenuState* state = (struct PokepadMenuState*) pokepad_state->app_state;

    switch (*trigger) {
    case 0:
        pokepad_state->app_state = malloc(sizeof(struct PokepadMenuState));

        /* Create the application description box using HBlank */
        hblank_handler_set(hblank_handler);
        lcd_io_set(REG_ID_BLDY, 8);

        *trigger = 1;
        break;

    case 1:
        /* Load arrow */
        gpu_pal_obj_alloc_tag_and_apply(&menu_arrow_palette);
        gpu_tile_obj_alloc_tag_and_upload(&menu_arrow_tiles);
        u8 id = template_instanciate_forward_search(&menu_arrow_template, 0, 0, 0);
        menu_arrow_update_position(id);
        objects[id].private[0] = 1; /* Enable */
        objects[id].private[2] = 1; /* Vertical */
        objects[id].private[3] = 2; /* Distance */
        objects[id].private[4] = 6; /* Speed */
        objects[id].private[5] = 1; /* Phase */

        load_page_indicators();

        load_applications();
        *trigger = 2;
        break;

    case 2:
        /* Load descriptions */
        state->textboxes.app_name = rboxid_init(&pokepad_menu_textboxes[0]);
        state->textboxes.app_description = rboxid_init(&pokepad_menu_textboxes[1]);
        update_app_textboxes();
        *trigger = 3;
        break;

    default:
        return false;
    }

    return true;
}

static bool destroy(u8* trigger)
{
    free(pokepad_state->app_state);
    return false;
}

static void callback(void)
{

}

static void oac_page_indicator(struct Object* obj)
{
    /* TODO: Check if the page ID matches the current page  */
    u8 tile = 1;
    if (obj->private[0] == 0) {
        tile = 0;
    }

    obj->final_oam.tile_num = obj->anim_data_offset + tile;
}

static void menu_arrow_update_position(u8 id)
{
    struct Object* obj = &objects[id];
    obj->pos1.x = POKEPAD_MENU_ICON_X;
    obj->pos1.y = POKEPAD_MENU_ICON_Y - POKEPAD_MENU_ICON_SIZE / 2 - 8;
}

static const pchar description[] = _"Main application";
const struct PokepadApplication pokepad_application_main = {
    .name = _"PokéPad",
    .description = &description,
    .role = POKEPAD_APP_MAIN,
    .setup = setup,
    .destroy = destroy,
    .callback = callback,
};
