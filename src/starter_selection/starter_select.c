#include <pokeagb/pokeagb.h>
#include "../../generated/images/beach_floor.h"
#include "../../generated/images/sky_starter.h"
#include "starter_select.h"
#include "../../generated/images/textbox.h"

extern void setup(void);
extern void bg_setup_starters(void);
extern void make_balls(void);
extern void make_pointer(void);
extern void make_pokemon_pics(void);
extern void messagebox_init(void);
extern void hide_pointer(void);
extern void show_pointer(void);
extern void pointer_move(u8 dir);
extern void highlight_ball(u8 id);
extern void hideall_starterpic(void);
extern void display_mode_starterpic(u8 id, u8 mode);
extern void update_message(u8);
extern void hide_messagebox(void);
extern void vblank_starter_select(void);
extern void init_indicator(void);
extern void update_indicator(u8 dir);
extern void hide_indicator(void);
extern void show_indicator(void);

void oac_nullsub(struct Object* obj) {
    return;
}


void starter_select_prelaunch() {
    extern void starter_select(void);
    extern void c2_starter_select(void);
    pal_fill_black();
    setup();
    super.multi_purpose_state_tracker = 0;
    set_callback1((SuperCallback)starter_select);
    set_callback2((SuperCallback)c2_starter_select);
}


void starter_select() {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    switch (super.multi_purpose_state_tracker) {
        case 0:
        {
            // setup
            vblank_handler_set(vblank_starter_select);
            REG_BG3VOFS = 0;
            REG_BG2VOFS = -79;
            REG_BG1VOFS = 0;
            REG_BG1HOFS = 0;
            struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
            *sdp = (struct starter_select_data*)malloc(sizeof(struct starter_select_data));
            // malloc ball frame buffer
            (*sdp)->ball_tiles = (void*)malloc_and_clear(512 * 4); // 4 32x32 frames
            super.multi_purpose_state_tracker++;
            break;
        }
        case 1:
        {
            // bg init and commit to VRAM
            bg_setup_starters();
            void* char_base = (void*)0x600C000;
            void* map_base = (void*)0x600E000;
            lz77UnCompVram((void *)sky_starterTiles, char_base);
            lz77UnCompVram((void *)sky_starterMap, map_base);   
            gpu_pal_apply((void *)sky_starterPal, 0, 32);
            
            char_base = (void*)0x6008000;
            map_base = (void*)0x600E800;
            lz77UnCompVram((void *)beach_floorTiles, char_base);
            lz77UnCompVram((void *)beach_floorMap, map_base);    
            gpu_pal_apply((void *)beach_floorPal, 16, 32);
            
            
            char_base = (void*)0x6004000;
            map_base = (void*)0x600F000;
            lz77UnCompVram((void *)textboxTiles, char_base);
            lz77UnCompVram((void *)textboxMap, map_base);    
            gpu_pal_apply((void *)textboxPal, 16 * 7, 32);
            gpu_pal_apply((void*)stdpal_get(0), 16*15, 32);
            
            // objects load
            make_balls();
            make_pointer();
            make_pokemon_pics();
            messagebox_init();
            hide_messagebox();
            init_indicator();
            hide_indicator();
            super.multi_purpose_state_tracker++;
            break;
        }
        case 2:
        {
            bgid_mark_for_sync(3);
            bgid_mark_for_sync(2);
            gpu_sync_bg_show(3);
            gpu_sync_bg_show(2);
            super.multi_purpose_state_tracker++;
            break;
        }
        case 3:
        {
            switch (super.buttons_new) {
                case KEY_A:                     
                    hide_pointer();
                    gpu_sync_bg_show(1);
                    display_mode_starterpic((*sdp)->pointer_pos, 0);
                    update_message((*sdp)->pointer_pos);
                    show_indicator();
                    super.multi_purpose_state_tracker++;
                    break;
                case KEY_B:
                    show_pointer();
                    hide_indicator();
                    break;
                case KEY_LEFT:
                    // move pointer to the left
                    show_pointer();
                    pointer_move(0);
                    highlight_ball((*sdp)->pointer_pos);
                    break;
                case KEY_RIGHT:
                    show_pointer();
                    pointer_move(1);
                    highlight_ball((*sdp)->pointer_pos);
                    break;
                default:
                    break;
            };
            break;
        }
        case 4:
        {
            // yes/no selection
            switch(super.buttons_new) {
                case KEY_UP:
                    update_indicator(0);
                    break;
                case KEY_DOWN:
                    update_indicator(1);
                    break;
                case KEY_A:
                    if ((*sdp)->yesno_cursor_pos) {
                        super.multi_purpose_state_tracker = 3;
                        gpu_sync_bg_hide(1);
                        hideall_starterpic();
                        hide_messagebox();
                        hide_indicator();
                        show_pointer();
                    } else {
                        super.multi_purpose_state_tracker = 5;
                    }
                    break;
                case 0:
                    // idle state
                    break;
                default:
                    gpu_sync_bg_hide(1);
                    hideall_starterpic();
                    hide_messagebox();
                    hide_indicator();
                    show_pointer();
                    super.multi_purpose_state_tracker = 3;
                    break;
                    
            };
            break;
        }
        case 5:
        {
            pal_fill_black();
            var_set(0x4015, (*sdp)->pointer_pos);
            super.multi_purpose_state_tracker++;
            set_callback1(c1_overworld);
            c2_exit_to_overworld_continue_music_scripts();
            free((*sdp));
            break;
        }
        default:
            break;
            
    };
    bgid_mod_x_offset(3, 1 << 5 , 1);
}


void bg_setup_starters() {
       gpu_tile_bg_drop_all_sets(0);
        struct BgConfig bg0_config = { 	
                                    .padding = 0,
                                    .b_padding = 0,
                                    .priority = 0,
                                    .palette = 0,
                                    .size = 0,
                                    .map_base = 31,
                                    .character_base = 0,
                                    .bgid = 0
        };
        struct BgConfig bg1_config = {	
                                    .padding = 0,
                                    .b_padding = 0,
                                    .priority = 1,
                                    .palette = 0,
                                    .size = 0,
                                    .map_base = 30,
                                    .character_base = 1,
                                    .bgid = 1
        };
        struct BgConfig bg2_config = {
                                    .padding = 0,
                                    .b_padding = 0,
                                    .priority = 2,
                                    .palette = 0,
                                    .size = 0,
                                    .map_base = 29,
                                    .character_base = 2,
                                    .bgid = 2
        };
        struct BgConfig bg3_config = {
                                    .padding = 0,
                                    .b_padding = 0,
                                    .priority = 3,
                                    .palette = 0,
                                    .size = 0,
                                    .map_base = 28,
                                    .character_base = 3,
                                    .bgid = 3
        };
        struct BgConfig bg_config_data[4] = {bg0_config, bg1_config, bg2_config, bg3_config};
        bg_vram_setup(0, (struct BgConfig *)&bg_config_data, 4);
}
