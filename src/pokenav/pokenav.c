#include <pokeagb/pokeagb.h>
#include "../../images/pokenav/pokenav_main.c"
#include "../../images/pokenav/pokenav_main_back.c"


// reset screen attributes
extern void setup(void);
extern void vcb_pokenav(void);
extern void vblank_cb_pal(void);
extern void vblank_cb_spq(void);
extern void c2_pokenav(void);
extern void dexnav_draw_icons(u16, u8);

void close_startmenu() {
	safari_stepscount_close();
	sm_close_description();
	sm_close_menu();
}

void launch_pokenav_gfx() {
	switch (super.multi_purpose_state_tracker) {
		case 0:
		{
			gpu_tile_bg_drop_all_sets(0);
			struct BgConfig bg0_config = { 	.padding = 0,
											.b_padding = 0,
											.priority = 0,
											.palette = 0,
											.size = 0,
											.map_base = 31,
											.character_base = 0,
											.bgid = 0
										};
			struct BgConfig bg1_config = {	.padding = 0,
											.b_padding = 0,
											.priority = 1,
											.palette = 0,
											.size = 0,
											.map_base = 30,
											.character_base = 1,
											.bgid = 1
										};
			struct BgConfig bg2_config = {	.padding = 0,
											.b_padding = 0,
											.priority = 2,
											.palette = 0,
											.size = 0,
											.map_base = 29,
											.character_base = 2,
											.bgid = 2
										};
			struct BgConfig bg3_config = {	.padding = 0,
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
			super.multi_purpose_state_tracker++;
			break;
		}
		case 1:
		{
			// TODO = Make this not hard coded.
			void *char_base = (void *)0x6000000 + (0x4000 * 0);
			void *map_base = (void *)0x6000000 + (0xF800 - (0x800 * 0));
			lz77UnCompVram((void *)pokenav_mainTiles, char_base);
			lz77UnCompVram((void *)pokenav_mainMap, map_base);
			
			char_base += 0x4000;
			map_base -= 0x800;
			lz77UnCompVram((void *)pokenav_main_backTiles, char_base);
			lz77UnCompVram((void *)pokenav_main_backMap, map_base);
			
			gpu_pal_apply((void *)pokenav_mainPal, 0, 64);
			gpu_pal_apply((void *)pokenav_main_backPal, 32, 64);
			super.multi_purpose_state_tracker++;
			break;
		}
		default:
			bgid_mark_for_sync(0);
			bgid_mark_for_sync(1);
			bgid_mark_for_sync(2);
			bgid_mark_for_sync(3);
			gpu_sync_bg_show(0);
			gpu_sync_bg_show(1);
			gpu_sync_bg_show(2);
			gpu_sync_bg_show(3);
			//super.multi_purpose_state_tracker++;
			break;
	};
	return;
}



u8 prelaunch_pokenav_setup() {
	close_startmenu();

	extern void init_dexnav_hud(u16, u8 environment);
    u8* var = (u8*)0x20370B8;
    u8* var1 = (u8*)0x20370BA;
	init_dexnav_hud(*var, *var1);//0x48, 1);
   // u8 obj_id =  sub_8083970(0x13, 0x75, 0x45);
  
	
    //objects[obj_id].final_oam.obj_mode = 1;
	return 1;
}