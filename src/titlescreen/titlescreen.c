#include <pokeagb/pokeagb.h>
#include "titlescreen.h"
#include "generated/images/titlescreen/logo.h"
#include "generated/images/titlescreen/background.h"
#include "generated/images/titlescreen/lugia1.h"
#include "generated/images/titlescreen/rocks1.h"
#include "util/debug.h"

extern struct TitlescreenState* titlescreen_state;

static void vblank(void)
{
    gpu_sprites_upload();
    copy_queue_process();
    gpu_pal_upload();
}

static const struct BgConfig bg_config[4] = {
    {
        .padding = 0,
        .b_padding = 0,
        .priority = 0,
        .palette = 1,
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

static void titlescreen_set_callback(SuperCallback callback) {
    titlescreen_animation_free(&titlescreen_state->lugia);
    titlescreen_animation_free(&titlescreen_state->rocks);
    free(titlescreen_state);
    set_callback2(callback);
}

static void fadeout(void) {
    if (!pal_fade_control.active) {
        titlescreen_set_callback(c2_main_menu_or_new_game);
    }

    process_palfade();
}

static void callback(void)
{
    tilescreen_animation_step(&titlescreen_state->lugia, BG_LAYER_LUGIA);
    tilescreen_animation_step(&titlescreen_state->rocks, BG_LAYER_ROCKS);

    /* Bounce lugia */
    bgid_mod_y_offset(BG_LAYER_LUGIA, get_spring_animation(titlescreen_state->counter) / 2, 0);
    titlescreen_state->counter += 6;

    if (KEYS_ALL_PRESSED(super.buttons_held_remapped, KEY_UP | KEY_B | KEY_SELECT)) {
        titlescreen_set_callback(c2_erase_save_menu);
    } else if (super.buttons_new_remapped & (KEY_A | KEY_START)) {
        /* Initialization */
        seed_from_timer1();
        saveblock_randomize_position();
        reset_save_mapped_memory();
        flash_reset_globals();
        flash_read(0);

        if (!save_load_result || save_load_result == 2) {
            initialize_savebocks();
        }

        audio_set_mono(saveblock2->options_sound);

        /* Next screen */
        fade_screen(~0, 0, 0x0, 0x10, 0);
        set_callback2(fadeout);
    }

    task_exec();
    objc_exec();
    obj_sync_superstate();
    process_palfade();
}

void custom_titlescreen_setup(void)
{
    switch (super.multi_purpose_state_tracker) {
    case 0:
        start_timer1();
        vblank_handler_set(vblank);

	obj_and_aux_reset_all();
	gpu_tile_obj_tags_reset();
	memset((void *)(ADDR_VRAM), 0x0, 0x10000);

        gpu_tile_bg_drop_all_sets(0);
	bgid_mod_x_offset(0, 0, 0);
        bgid_mod_y_offset(0, 0, 0);
	bgid_mod_x_offset(1, 0, 0);
        bgid_mod_y_offset(1, 0, 0);
	bgid_mod_x_offset(2, 0, 0);
        bgid_mod_y_offset(2, 0, 0);
	bgid_mod_x_offset(3, 0, 0);
        bgid_mod_y_offset(3, 0, 0);
        bg_vram_setup(0, bg_config, 4);

	tasks_init();
        lcd_io_set(REG_ID_DISPCNT, 0);


        super.multi_purpose_state_tracker++;
        break;

    case 1:
        init_malloc(dynamic_area, sizeof(dynamic_area));
        titlescreen_state = malloc_and_clear(sizeof(struct TitlescreenState));

        // Configure tilemap
        bgid_set_tilemap(3, titlescreen_state->tilemaps.background);
        bgid_set_tilemap(BG_LAYER_LUGIA, titlescreen_state->tilemaps.lugia);
        bgid_set_tilemap(BG_LAYER_ROCKS, titlescreen_state->tilemaps.rocks);
        bgid_set_tilemap(BG_LAYER_LOGO, titlescreen_state->tilemaps.logo);

        super.multi_purpose_state_tracker++;
        break;

    case 2:
        // Logo Layer
        gpu_copy_tilemap(BG_LAYER_LOGO, logoMap, logoMapLen, 0);
        gpu_copy_to_tileset(BG_LAYER_LOGO, logoTiles, logoTilesLen, 0);
        bgid_send_tilemap(BG_LAYER_LOGO);
        gpu_pal_apply(logoPal, 0, logoPalLen);

        // background layer
        gpu_copy_tilemap(3, backgroundMap, backgroundMapLen, 0);
        gpu_copy_to_tileset(3, backgroundTiles, backgroundTilesLen, 0);
        bgid_send_tilemap(3);
        gpu_pal_apply(backgroundPal, logoPalLen / 2, backgroundPalLen);

        super.multi_purpose_state_tracker++;
        break;

    case 3:
        // Lugia layer
        gpu_pal_apply(lugia1Pal, (logoPalLen + backgroundPalLen) / 2, lugia1PalLen);
        titlescreen_animation_init(&titlescreen_state->lugia, &lugia_animation_config);
        titlescreen_animation_load_image(&titlescreen_state->lugia, BG_LAYER_LUGIA, 0);
        super.multi_purpose_state_tracker++;
        break;

    case 4:
        // Rocks layer
        gpu_pal_apply(rocks1Pal, (logoPalLen + backgroundPalLen + lugia1PalLen) / 2, rocks1PalLen);
        titlescreen_animation_init(&titlescreen_state->rocks, &rocks_animation_config);
        titlescreen_animation_load_image(&titlescreen_state->rocks, BG_LAYER_ROCKS, 0);
        super.multi_purpose_state_tracker++;
        break;

    case 5:
        fade_screen(~0, 0, 0x10, 0x0, 0);
        lcd_io_set(REG_ID_DISPCNT, DISPCNT_OBJ | DISPCNT_OAM_1D);
        bgid_mark_for_sync(0);
        bgid_mark_for_sync(1);
        bgid_mark_for_sync(2);
        bgid_mark_for_sync(3);
        gpu_sync_bg_show(0);
        gpu_sync_bg_show(1);
        gpu_sync_bg_show(2);
        gpu_sync_bg_show(3);
        set_callback2(callback);
        break;
    }

    task_exec();
    objc_exec();
    obj_sync_superstate();
}
