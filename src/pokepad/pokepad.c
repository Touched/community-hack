#include <pokeagb/pokeagb.h>
#include "pokepad.h"
#include "generated/images/pokepad/pokenav_main.h"

// reset screen attributes
extern void setup(void);
extern void vcb_pokenav(void);
extern void vblank_cb_spq(void);

const pchar pokepad_sm_name[] = _"PokéPad";
const pchar pokepad_sm_description[] = _"A high-tech multifunctional device.";

static const struct BgConfig bg_config[4] = {
    {
        .padding = 0,
        .b_padding = 0,
        .priority = 0,
        .palette = 0,
        .size = 0,
        .map_base = 31,
        .character_base = 0,
        .bgid = 0
    }, {
        .padding = 0,
        .b_padding = 0,
        .priority = 1,
        .palette = 0,
        .size = 0,
        .map_base = 30,
        .character_base = 1,
        .bgid = 1
    }, {
        .padding = 0,
        .b_padding = 0,
        .priority = 2,
        .palette = 0,
        .size = 0,
        .map_base = 29,
        .character_base = 2,
        .bgid = 2
    }, {
        .padding = 0,
        .b_padding = 0,
        .priority = 3,
        .palette = 0,
        .size = 0,
        .map_base = 28,
        .character_base = 3,
        .bgid = 3
    }
};

struct PlttData
{
    u16 r:5; // red
    u16 g:5; // green
    u16 b:5; // blue
    u16 unused_15:1;
};

u16 blend_color(u16 color, u8 coeff, u16 blendColor)
{
    struct PlttData *data1 = (struct PlttData *) &color;
    s8 r = data1->r;
    s8 g = data1->g;
    s8 b = data1->b;
    struct PlttData *data2 = (struct PlttData *) &blendColor;
    return ((r + (((data2->r - r) * coeff) >> 4)) << 0)
        | ((g + (((data2->g - g) * coeff) >> 4)) << 5)
        | ((b + (((data2->b - b) * coeff) >> 4)) << 10);
}

static void build_gradient(u16 start_color, u16 stop_color)
{
    struct ColorComponents* start = (struct ColorComponents*) &start_color;
    struct ColorComponents* stop = (struct ColorComponents*) &stop_color;
    u16 lines = sizeof(pokepad_state->shared_state->gradient) / sizeof(u16);
    u16 start_line = 2;

    /* RGB 8 bit components */
    u16 mr = ((stop->r << 3) - (start->r << 3)) / (lines - start_line);
    u16 mg = ((stop->g << 3) - (start->g << 3)) / (lines - start_line);
    u16 mb = ((stop->b << 3) - (start->b << 3)) / (lines - start_line);
    u16 r = start->r << 3;
    u16 g = start->g << 3;
    u16 b = start->b << 3;

    union Color color = {};
    for (u8 i = 0; i < lines; i++) {
        if (i < start_line) {
            pokepad_state->shared_state->gradient[i] = 0;
        } else {
            color.components.r = r >> 3;
            color.components.g = g >> 3;
            color.components.b = b >> 3;

            pokepad_state->shared_state->gradient[i] = color.packed;

            r += mr;
            g += mg;
            b += mb;
        }
    }
}

static void build_gradient_palette(void)
{
    u16* buffer = (u16*) pokepad_state->shared_state->gradient_palette;
    for (u8 i = 0; i < sizeof(pokepad_state->shared_state->gradient) / sizeof(u16); i++) {
        u16 color = pokepad_state->shared_state->gradient[i];

        for (u8 j = 0; j < 16; j++) {
            u8 r = color & 0x1f;
            u8 g = (color >> 5) & 0x1f;
            u8 b = (color >> 10) & 0x1f;

#define SUB_OR_ZERO(a, b) (a) >= (b) ? (a) - (b) : 0
            r = SUB_OR_ZERO(r, (j >> 2) + 2);
            g = SUB_OR_ZERO(g, (j >> 2) + 2);
            b = SUB_OR_ZERO(b, (j >> 2) + 2);
#undef SUB_OR_ZERO

            u16 final_color = r | (g << 5) | (b << 10);
            *buffer++ = blend_color(final_color, pal_fade_control.y, pal_fade_control.blend_color);
        }
    }
}

static void build_mosaic(void* charbase, void* mapbase)
{
    /* Tile tileset, one tile for each palette */
    u8 tiles[16][32];

    for (u8 i = 0; i < 16; i++) {
        u8 tile = i | (i << 4);
        memset(tiles[i], tile, 32);
    }

    memcpy(charbase, tiles, sizeof(tiles));

    /* Build tilemap */
    u16 tilemap[20][32];

    for (u8 y = 0; y < 20; y++) {
        for (u8 x = 0; x < 32; x++) {
            u8 tile = rand() & 0xF;
            tilemap[y][x] = tile;
        }
    }

    memcpy(mapbase, tilemap, sizeof(tilemap));
}

void pokepad_hblank_handler(void)
{
    /* HBlank saves memory compared to the DMA implementation as the
     * gradient only changes colour every 8 scanlines. */

    u16* buffer = (u16*) pokepad_state->shared_state->gradient_palette;

    if (REG_VCOUNT < 160) {
        u16 index = (REG_VCOUNT + 1) / 8;
        memcpy((void*) 0x05000000, &buffer[index * 16], 32);
    }
}

void pokepad_vblank_handler(void)
{
    gpu_sprites_upload();
    copy_queue_process();
    gpu_pal_upload();
}

static struct Textbox pokepad_bar_textboxes[2] = {
    {
        0, 24, 0, 6, 2, 0xF, 0x50, NULL,
    }, {
        0, 1, 0, 0x9, 2, 0xF, 0x6F, NULL,
    }
};

static struct TextColor pokepad_bar_color = {
    0, 1, 2,
};

void pokepad_update_time(void)
{
    /* TODO: Get actual time */
    pchar time_string[] = _"10:11";
    u8 id = pokepad_state->shared_state->bar_textboxes[0];

    u32 width = font_get_width_of_string(0, time_string, 0);
    u8 x = rboxes[id].width * 8 - width - 8; /* 8 pixels from the right */

    rboxid_clear_pixels(id, 0x0);
    rboxid_print(id, 0, x, 0, &pokepad_bar_color, 0xFF, time_string);
    rboxid_tilemap_update(id);
    rboxid_update(id, 3);
}

void pokepad_update_application(void)
{
    u8 id = pokepad_state->shared_state->bar_textboxes[1];
    rboxid_clear_pixels(id, 0x0);
    rboxid_print(id, 0, 0, 0, &pokepad_bar_color, 0xFF, pokepad_state->current_app->name);
    rboxid_tilemap_update(id);
    rboxid_update(id, 3);
}

static void pokepad_callback(void);

void launch_pokepad_app()
{
    switch (super.multi_purpose_state_tracker) {
    case 0:
    {
        m4aMPlayVolumeControl(&mplay_BGM, 0xFFFF, 128);
        gpu_tile_bg_drop_all_sets(0);
        bg_vram_setup(0, bg_config, 4);
        super.multi_purpose_state_tracker++;
        break;
    }
    case 1:
    {
        // TODO : Make this not hard coded.
        void *char_base = (void*) 0x6000000 + (0x4000 * 0);
        void *map_base = (void*) 0x6000000 + (0xF800 - (0x800 * 0));
        lz77UnCompVram((void*) pokenav_mainTiles, char_base);
        lz77UnCompVram((void*) pokenav_mainMap, map_base);

        char_base += 0x4000;
        map_base -= 0x800;
        build_mosaic(char_base, map_base);
        super.multi_purpose_state_tracker++;
        break;
    }
    case 2: {
        pokepad_state->shared_state->bar_textboxes[0] = rboxid_init(&pokepad_bar_textboxes[0]);
        pokepad_state->shared_state->bar_textboxes[1] = rboxid_init(&pokepad_bar_textboxes[1]);
        pokepad_update_time();
        pokepad_update_application();
        /* TODO: Update signal and draw logo */

        super.multi_purpose_state_tracker++;
        break;
    }
    case 3:
        /* Apply the gradient palette for the first line as hblank DMA
         * only takes over after that. */
        /*  */
        build_gradient(0x684E, 0x09DD);
        build_gradient_palette();
        gpu_pal_apply(pokepad_state->shared_state->gradient_palette, 0, 32);

        /* Clean the palette buffer so HBlank doesn't start copying it */
        /* FIXME: Use macro for size */
        memset(pokepad_state->shared_state->gradient_palette, 0, 32 * 20);
        fade_screen(~0, 0, 0x10, 0x0, 0);
        vblank_handler_set(pokepad_vblank_handler);
        hblank_handler_set(pokepad_hblank_handler);
        interrupts_enable(INTERRUPT_VBLANK | INTERRUPT_HBLANK);

        /* Get ready to call setup */
        pokepad_state->tracker = 0;

        super.multi_purpose_state_tracker++;
        break;
    case 4:
        /* Run the app setup function */
        if (!pokepad_state->current_app->setup(&pokepad_state->tracker)) {
            super.multi_purpose_state_tracker++;
        }

        break;

    case 5:
        lcd_io_set(REG_ID_DISPCNT, DISPCNT_OBJ | DISPCNT_OAM_1D);
        bgid_mark_for_sync(0);
        bgid_mark_for_sync(1);
        bgid_mark_for_sync(2);
        bgid_mark_for_sync(3);
        gpu_sync_bg_show(0);
        gpu_sync_bg_show(1);
        gpu_sync_bg_show(2);
        gpu_sync_bg_show(3);
        super.multi_purpose_state_tracker++;
        break;

    case 6:
        /* TODO: Possible boot animation */
        if (pal_fade_control.active) {
            build_gradient_palette();
        } else {
            super.multi_purpose_state_tracker++;
        }
        break;

    case 7:
        /* TODO: Wait forfade */
        set_callback2(pokepad_callback);
        set_callback1(NULL);
        break;
    };

    task_exec();
    remoboxes_upload_tilesets();
    objc_exec();
    obj_sync_superstate();
    process_palfade();
}

u8 prelaunch_pokenav_setup()
{
    if (pal_fade_control.active) {
        return 0;
    } else {
        /* Malloc state */
        pokepad_state = (struct Pokepad*) malloc(sizeof(struct Pokepad));
        pokepad_state->shared_state = (struct PokepadShared*) malloc(sizeof(struct PokepadShared));
        pokepad_state->current_app = pokepad_application_find_main();

        lcd_io_set(REG_ID_DISPCNT, 0);
        help_system_disable__sp198();
        setup();
        set_callback1(launch_pokepad_app);
        set_callback2(NULL);
        super.multi_purpose_state_tracker = 0;
        return 1;
    }
}

static void pokepad_exit(void)
{
    switch (super.multi_purpose_state_tracker) {
    case 0:
        fade_screen(~0, 0, 0x0, 0x10, 0);
        super.multi_purpose_state_tracker++;
        break;
    case 1:
        if (!pal_fade_control.active) {
            super.multi_purpose_state_tracker++;
            pokepad_state->tracker = 0;
        }
        break;
    case 2:
        if (!pokepad_state->current_app->destroy(&pokepad_state->tracker)) {
            super.multi_purpose_state_tracker++;
        }

        break;
    case 3:
        /* Unmalloc */
        free(pokepad_state->shared_state);
        free(pokepad_state);

        m4aMPlayVolumeControl(&mplay_BGM, 0xFFFF, 256);
        set_callback1(c1_overworld);
        set_callback2(c2_overworld_switch_start_menu);
        break;
    }

    build_gradient_palette();
    process_palfade();
}

static void pokepad_callback(void)
{
    if (super.buttons_new_remapped & KEY_B) {
        set_callback2(pokepad_exit);
    }

    pokepad_state->current_app->callback();

    task_exec();
    remoboxes_upload_tilesets();
    objc_exec();
    obj_sync_superstate();
    process_palfade();
}

static void pokepad_app_switcher(void)
{
    switch (super.multi_purpose_state_tracker) {
    case 0:
        if (pal_fade_control.active) {
            break;
        }

        pokepad_state->tracker = 0;
        super.multi_purpose_state_tracker++;
    case 1:
        if (!pokepad_state->current_app->destroy(&pokepad_state->tracker)) {
            pokepad_state->tracker = 0;
            pokepad_state->current_app = pokepad_state->next_app;
            super.multi_purpose_state_tracker++;
        }
        break;
    case 2:
        if (!pokepad_state->current_app->setup(&pokepad_state->tracker)) {
            pokepad_state->tracker = 0;
            super.multi_purpose_state_tracker++;
            pokepad_update_application();
            fade_screen(~0, 0, 0x10, 0x0, 0);
        }
        break;
    case 3:
        if (pal_fade_control.active) {
            break;
        }

        super.multi_purpose_state_tracker++;
    case 4:
        set_callback2(pokepad_callback);
        break;
    }

    build_gradient_palette();
    process_palfade();
    task_exec();
    remoboxes_upload_tilesets();
    objc_exec();
    obj_sync_superstate();
}

void pokepad_switch_app(const struct PokepadApplication* app)
{
    fade_screen(~0, 0, 0x0, 0x10, 0);
    pokepad_state->next_app = app;
    pokepad_state->shared_state->coefficient = 0x10 << 1;
    super.multi_purpose_state_tracker = 0;
    set_callback2(pokepad_app_switcher);
}
