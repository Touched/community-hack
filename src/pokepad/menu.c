#include <pokeagb/pokeagb.h>
#include "pokepad.h"
#include "generated/images/pokepad/menu/arrow.h"
#include "generated/images/pokepad/menu/page_indicator.h"

#define POKEPAD_MENU_ICON_TAG_BASE 0xF123
#define POKEPAD_MENU_ICON_X 36
#define POKEPAD_MENU_ICON_Y 55
#define POKEPAD_MENU_ICON_SIZE 42
#define POKEPAD_MENU_ICON_SIZE_PADDED 56
#define POKEPAD_MENU_PAGE_INDICATOR_WIDTH 8
#define POKEPAD_APP_NAME_WIDTH 8
#define POKEPAD_MENU_APPS_PER_PAGE 4
#define POKEPAD_MENU_MAX_APPS 12

struct PokepadMenuState {
    struct {
        u8 app_name;
        u8 app_description;
    } textboxes;

    u8 icons[POKEPAD_MENU_APPS_PER_PAGE];

    /* Loaded apps */
    const struct PokepadApplication* apps[POKEPAD_MENU_MAX_APPS];

    u8 index;
    u8 page;
    u8 page_index;
    u8 app_count;
    u8 page_count;
    u8 arrow_id;
};

static struct OamData app_icon_oam = {
    .size = 3
};

static void menu_arrow_update_position();

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

static void hblank_handler(void)
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
    struct PokepadMenuState* state = (struct PokepadMenuState*) pokepad_state->app_state;

    if (state->page_count > 1) {
        gpu_pal_obj_alloc_tag_and_apply(&menu_page_indicator_palette);
        gpu_tile_obj_alloc_tag_and_upload(&menu_page_indicator_tiles);

        u16 width = state->page_count * POKEPAD_MENU_PAGE_INDICATOR_WIDTH;
        u16 x = (SCREEN_WIDTH - width + POKEPAD_MENU_PAGE_INDICATOR_WIDTH) / 2;

        for (u8 i = 0; i < state->page_count; i++) {
            u8 id = template_instanciate_forward_search(&menu_page_indicator_template,
                                                        x + i * 8, 104, 0);
            objects[id].private[0] = i;
        }
    }
}

static void load_applications_for_page(u8 page)
{
    struct PokepadMenuState* state = (struct PokepadMenuState*) pokepad_state->app_state;
    u16 x = POKEPAD_MENU_ICON_X;

    u8* objects = state->icons;
    u8 offset = POKEPAD_MENU_APPS_PER_PAGE * page;

    for (u8 i = 0; i < POKEPAD_MENU_APPS_PER_PAGE; i++) {
        const struct PokepadApplication* app = state->apps[i + offset];

        if (!app) {
            continue;
        }

        /* Dynamically generate the icon template */
        const struct SpritePalette palette = {
            app->icon.palette,
            POKEPAD_MENU_ICON_TAG_BASE + i + offset,
        };

        const struct SpriteTiles tiles = {
            app->icon.tiles,
            app->icon.size,
            POKEPAD_MENU_ICON_TAG_BASE + i + offset,
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
        *objects++ = template_instanciate_forward_search(&template, x, POKEPAD_MENU_ICON_Y, 0);
        x += POKEPAD_MENU_ICON_SIZE_PADDED;
    }
}

static void count_applications(void)
{
    struct PokepadMenuState* state = (struct PokepadMenuState*) pokepad_state->app_state;

    for (u8 i = 0; i < POKEPAD_MENU_MAX_APPS; i++) {
        const struct PokepadApplication* app = state->apps[i];

        if (app) {
            state->app_count++;
        }
    }

    /* Ceiling division */
    state->page_count = (state->app_count + POKEPAD_MENU_APPS_PER_PAGE - 1) /
        POKEPAD_MENU_APPS_PER_PAGE;
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
        NULL,
    }, {
        0, 1, 15, 0x1C, 5, 0xF, 0x100, NULL,
    }
};

static struct TextColor pokepad_menu_color = {
    0, 1, 2,
};

static void update_app_description(u8 id, const struct PokepadApplication* app)
{
    rboxid_clear_pixels(id, 0x0);
    rboxid_print(id, 1, 0, 6, &pokepad_menu_color, 0xFF, app->description);
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
    const struct PokepadApplication* app = state->apps[state->index];

    update_app_description(state->textboxes.app_description, app);
    update_app_name(state->textboxes.app_name, app);
}

static bool setup(u8* trigger)
{
    struct PokepadMenuState* state = (struct PokepadMenuState*) pokepad_state->app_state;

    switch (*trigger) {
    case 0:
        pokepad_state->app_state = malloc_and_clear(sizeof(struct PokepadMenuState));

        /* Create the application description box using HBlank */
        hblank_handler_set(hblank_handler);
        lcd_io_set(REG_ID_BLDY, 8);

        *trigger = 1;
        break;

    case 1:
        pokepad_applications_load(state->apps, POKEPAD_MENU_MAX_APPS);
        count_applications();

        /* Load arrow */
        gpu_pal_obj_alloc_tag_and_apply(&menu_arrow_palette);
        gpu_tile_obj_alloc_tag_and_upload(&menu_arrow_tiles);
        state->arrow_id = template_instanciate_forward_search(&menu_arrow_template, 0, 0, 0);
        menu_arrow_update_position();
        objects[state->arrow_id].private[0] = 1; /* Enable */
        objects[state->arrow_id].private[2] = 1; /* Vertical */
        objects[state->arrow_id].private[3] = 2; /* Distance */
        objects[state->arrow_id].private[4] = 20; /* Speed */
        objects[state->arrow_id].private[5] = 0; /* Phase */

        load_page_indicators();

        load_applications_for_page(0);
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
    struct PokepadMenuState* state = (struct PokepadMenuState*) pokepad_state->app_state;

    if (super.buttons.new_remapped & KEY_LEFT) {
        if (state->index > 0) {
            state->index -= 1;
        }
    } else if (super.buttons.new_remapped & KEY_RIGHT) {
        if (state->index < state->app_count - 1) {
            state->index += 1;
        }
    }

    u8 old_page = state->page;

    state->page = state->index / POKEPAD_MENU_APPS_PER_PAGE;
    state->page_index = state->index % POKEPAD_MENU_APPS_PER_PAGE;

    if (old_page != state->page) {
        /* Update page */
        for (u8 i = 0; i < POKEPAD_MENU_APPS_PER_PAGE; i++) {
            obj_delete_and_free(&objects[state->icons[i]]);
        }

        load_applications_for_page(state->page);
    }

    menu_arrow_update_position();
    update_app_textboxes();
}

static void oac_page_indicator(struct Object* obj)
{
    struct PokepadMenuState* state = (struct PokepadMenuState*) pokepad_state->app_state;

    u8 tile = 1;
    if (obj->private[0] == state->page) {
        tile = 0;
    }

    obj->final_oam.tile_num = obj->anim_data_offset + tile;
}

static void menu_arrow_update_position()
{
    struct PokepadMenuState* state = (struct PokepadMenuState*) pokepad_state->app_state;

    struct Object* obj = &objects[state->arrow_id];
    obj->pos1.x = POKEPAD_MENU_ICON_X + state->page_index * POKEPAD_MENU_ICON_SIZE_PADDED;
    obj->pos1.y = POKEPAD_MENU_ICON_Y - POKEPAD_MENU_ICON_SIZE / 2 - 8;
}

const struct PokepadApplication pokepad_application_main = {
    .name = _"PokéPad",
    .description = NULL,
    .role = POKEPAD_APP_MAIN,
    .setup = setup,
    .destroy = destroy,
    .callback = callback,
};
