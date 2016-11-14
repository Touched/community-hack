#include <pokeagb/pokeagb.h>



void vblank_cb_spq() {
	gpu_sprites_upload();
	copy_queue_process();
	gpu_pal_upload();
}


void vblank_cb_pal() {
	copy_queue_process();
	gpu_pal_upload();
}


void vcb_pokenav() {
	task_exec();
	remoboxes_upload_tilesets();
	objc_exec();
	obj_sync_superstate();
	process_palfade();
}

void c2_pokenav() {
	tilemaps_sync();
	copy_queue_process();
	gpu_pal_upload();
}


void handlers_clear() {
	vblank_handler_set(0);
	hblank_handler_set(0);
	set_callback1(0);
	set_callback2(0);
}


void reset_pal_settings() {
	pal_fade_control_and_dead_struct_reset();
	palette_bg_faded_fill_black();
	gpu_pal_allocator_reset();
	*gpu_pal_tag_search_lower_boundary = 0;
}


void reset_bg_settings() {
	overworld_free_bgmaps();
	gpu_tile_bg_drop_all_sets(0);
	bgid_mod_x_offset(0, 0, 0);
    bgid_mod_y_offset(0, 0, 0);
	bgid_mod_x_offset(1, 0, 0);
    bgid_mod_y_offset(1, 0, 0);
	bgid_mod_x_offset(2, 0, 0);
    bgid_mod_y_offset(2, 0, 0);
	bgid_mod_x_offset(3, 0, 0);
    bgid_mod_y_offset(3, 0, 0);
}

void reset_boxes() {
	remo_reset_acknowledgement_flags();
	rboxes_free();
}

void setup_ioregs_bg() {
	u8 i;
	for (i = 0; i < 4; i ++) {
		BG_CNT[i].priority = i;
		BG_CNT[i].char_index = i;
		BG_CNT[i].padding = 0;
		BG_CNT[i].mosiac = 0;
		BG_CNT[i].color = 0;
		BG_CNT[i].map_index = (0x1F - i);
		BG_CNT[i].screen_over = 0;
		BG_CNT[i].size = 0;
		
	}
	
	
	lcd_io_set(0x50, 0x2F00);
	lcd_io_set(0x52, 0x0F);
}


void setup() {
	// callbacks
	handlers_clear();
	// BGs
	reset_bg_settings();
	setup_ioregs_bg();
	// pals
	reset_pal_settings();
	// objs
	obj_and_aux_reset_all();
	gpu_tile_obj_tags_reset();
	// VRAM clear
	memset((void *)(ADDR_VRAM), 0x0, 0x10000);
	// tasks
	tasks_init();
	// textboxes
	reset_boxes();
	
}
