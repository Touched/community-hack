#include <pokeagb/pokeagb.h>
#include "generated/images/pokenav/pokenav_main.h"
#include "generated/images/pokenav/pokenav_main_back.h"

// reset screen attributes
extern void setup(void);
extern void vcb_pokenav(void);
extern void vblank_cb_spq(void);
extern void c2_pokenav(void);

const pchar pokepad_sm_name[] = _"Poképad";
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

static void build_gradient(void)
{
    /* TODO: Build a 3 stop gradient */
    /* Gradient 4E68 -> DD09 */
    /* There is a colour in the gradient for every 8 pixels in the screen height */
    u16 gradient[20] = {
        0x684E, 0x684E,
        0x684E, 0x644F, 0x5C70, 0x5491, 0x50B2, 0x48D3, 0x44F4, 0x3D15,
        0x3536, 0x3137, 0x2958, 0x2179, 0x1D9A, 0x15BB, 0x11DC, 0x09DD,
        0x09DD, 0x09DD
    };

    u16* buffer = (u16*) 0x02038700;
    u8 line = 0;
    for (u8 i = 0; i < sizeof(gradient) / sizeof(u16); i++) {
        u16 color = gradient[i];

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
    u16 tilemap[32][32];

    for (u8 y = 0; y < 32; y++) {
        for (u8 x = 0; x < 32; x++) {
            u8 tile = rand() & 0xF;
            tilemap[y][x] = tile;
        }
    }

    memcpy(mapbase, tilemap, sizeof(tilemap));
}


#define REG_BASE 0x04000000
#define REG_VCOUNT	*(vu16*)(REG_BASE + 6)
#define REG_BLDCNT	*(vu16*)(REG_BASE + 0x50)
#define REG_BLDY	*(vu16*)(REG_BASE + 0x54)
#define REG_WIN	*(vu16*)(REG_BASE + 0x40)

static void hblank_gradient(void)
{
    /* HBlank saves memory compared to the DMA implementation as the
     * gradient only changes colour every 8 scanlines. */

    u16* buffer = (void*) 0x02038700;

    if (REG_VCOUNT < 160) {
        u16 index = (REG_VCOUNT + 1) / 8;
        memcpy((void*) 0x05000000, &buffer[index * 16], 32);

        if (REG_VCOUNT >= 120) {
            /* Enable darkening on BG1 and backdrop */
            REG_BLDCNT = (3 << 6) | (1 << 1) | (1 << 5);
            REG_BLDY = 8;
        }
    }
}

static void vblank_cb_pal()
{
    gpu_sprites_upload();
    copy_queue_process();
    gpu_pal_upload();
    REG_BLDCNT = 0;
    REG_BLDY = 8;
}

void launch_pokenav_gfx() {
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
        /* TODO: Neaten up */

        struct Textbox pokepad_app_description_textbox = {
            0, 1, 0x10, 0x1C, 3, 0xF, 0x8F, 0x20A00,
        };

        struct TextColor color = {
            0, 1, 2
        };

        u8 id = rboxid_init(&pokepad_app_description_textbox);
        pchar s[] = _"I am an application";

        rboxid_clear_pixels(id, 0x0);
        rboxid_print(id, 1, 0, 0, &color, 0xFF, s);
        rboxid_tilemap_update(id);
        rboxid_update(id, 3);

        /*  */

        struct Textbox pokepad_app_notification_textbox = {
            0, 25, 0, 0x9, 2, 0xF, 0x50, 0x20A00,
        };

        u8 id2 = rboxid_init(&pokepad_app_notification_textbox);

        pchar s2[] = _"- 10:11";
        rboxid_clear_pixels(id2, 0x0);
        rboxid_print(id2, 0, 0, 0, &color, 0xFF, s2);
        rboxid_tilemap_update(id2);
        rboxid_update(id2, 3);

        /*  */

        struct Textbox pokepad_app_title_textbox = {
            0, 1, 0, 0x9, 2, 0xF, 0x6F, 0x20A00,
        };

        u8 id3 = rboxid_init(&pokepad_app_title_textbox);

        pchar s3[] = _"PokéPad";
        rboxid_clear_pixels(id3, 0x0);
        rboxid_print(id3, 0, 0, 0, &color, 0xFF, s3);
        rboxid_tilemap_update(id3);
        rboxid_update(id3, 3);

        super.multi_purpose_state_tracker++;
        break;
    }
    case 3:
        vblank_handler_set(vblank_cb_pal);
        hblank_handler_set(hblank_gradient);
        ((void (*)(u16)) (0x08000B68|1))(3);

        /* Apply the gradient palette for the first line as hblank
         * DMA only takes over after that */
        gpu_pal_apply((void*) 0x02038700, 0, 32);

        /* Backgrounds */
        bgid_mark_for_sync(0);
        bgid_mark_for_sync(1);
        bgid_mark_for_sync(2);
        bgid_mark_for_sync(3);
        gpu_sync_bg_show(0);
        gpu_sync_bg_show(1);
        gpu_sync_bg_show(2);
        gpu_sync_bg_show(3);
        fade_screen(~0, 10, 0x10, 0x0, 0);
        super.multi_purpose_state_tracker++;
        break;
    default:
        if (super.buttons.new_remapped & KEY_B) {
            m4aMPlayVolumeControl(&mplay_BGM, 0xFFFF, 256);
            set_callback1(c1_overworld);
            set_callback2(c2_overworld_switch_start_menu);
        }
        break;
    };

    /* TODO: Rbox free */
}

u8 prelaunch_pokenav_setup() {
    if (pal_fade_control.active) {
        return 0;
    } else {
        lcd_io_set(0, 0);
        help_system_disable__sp198();
        vblank_handler_set(vblank_cb_pal);
        setup();
        set_callback1(launch_pokenav_gfx);
        set_callback2(c2_pokenav);
        super.multi_purpose_state_tracker = 0;
        return 1;
    }

}

void c2_pokenav() {
    build_gradient();
    fade_and_return_progress_probably();
}

/* 0806F380 */
