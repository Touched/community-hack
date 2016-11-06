#include <pokeagb/pokeagb.h>

int dialog_add_simple(u8, u8, char*, u8, u8, u8, u8);
/*
void merger(u8 t_id) {
	
	// merge Tilemaps?
	u16 i;
	u8 *dst = (u8 *)0x02001840;
	u8 *src = (u8 *)testingMap;//(u32 *)0x0600F800;
	for (i = 0; i < 2048; i++) {
		if (!*(dst + i)) {
			*(dst + i) = *(src + i);
		}
	}
	gpu_pal_apply((u8 *)testingPal, 14 * 16, 32);
	tasks[t_id].priv[0]++;
	if (tasks[t_id].priv[0] == 100) {
		char ca_upd[] = {0xFC, 0x2, 0x0, 0x0, 0xE9, 0xE4, 0xD8, 0xD5, 0xE8, 0xD9, 0x00,0xE8, 0xD9, 0xEC, 0xE8, 0xFF };
		rboxid_clear_pixels(tasks[t_id].priv[1], 0x0);
		rboxid_clear_pixels(tasks[t_id].priv[2], 0x0);
		dialog_add_simple(tasks[t_id].priv[1], 1, (char *)ca_upd, 0, 1, 0xFF, 0);
		dialog_add_simple(tasks[t_id].priv[2], 1, (char *)ca_upd, 0, 1, 0xFF, 0);
		rboxid_upload_a(tasks[t_id].priv[1], 2);
		rboxid_upload_a(tasks[t_id].priv[2], 2);
		task_del(t_id);
	}
}


void init_scene() {
	//superstate.multipurpose_state_tracker = 0;
	//help_system_disable__sp198();
	//setup();
	//superstate.callback1 = 0;//(super_callback)(background_make + 1);
	//superstate.callback2 = 0;
	
	char ca_Sea[] = { 0xCD, 0xD9, 0xD5, 0xE6, 0xD7, 0xDC, 0x00, 0xC6, 0xD9, 0xEA, 0xD9, 0xE0, 0xFF };

	
	mem_set((void *)0x02001840, 0x0, 2048);
	mem_cpy(0x06008000, (u8 *)testingTiles, 1920);	
	task_add(merger, 1);
	char ca_Hel[] = {0xFC, 0x2, 0x0, 0x0, 0xC2, 0xD9, 0xE0, 0xE0, 0xE3, 0x00, 0xD8, 0xD5, 0xE6, 0xDF, 0xE2, 0xD9, 0xE7, 0xE7, 0x00, 0xE1, 0xED, 0x00, 0xE3, 0xE0, 0xD8, 0x00, 0xDA, 0xE6, 0xDD, 0xD9, 0xE2, 0xD8, 0xFB, 0xFF };
	
	char ca_And[] = { 0xFC, 0x2, 0x0, 0x0, 0xBB, 0xE2, 0xD8, 0x00, 0xD5, 0xE2, 0xE3, 0xE8, 0xDC, 0xD9, 0xE6, 0x00, 0xE3, 0xE2, 0xD9, 0xFB, 0xFF };
	
	u32 *space = (u32 *)malloc_and_clear(0x500);
	u32 *space2 = (u32 *)malloc_and_clear(0x500);
	struct rbox this_box = {0x0, 0x2, 0x9, 0x1A, 0x2, 0xF, 132, (u32)space};
	struct rbox this_box2 = {0x0, 0x2, 0xB, 0x1A, 0x2, 0xF, 67, (u32)space2};
	u8 r_id = rbox_get_id_something(&this_box);
	u8 r_id2 = rbox_get_id_something(&this_box2);
	rboxid_clear_pixels(r_id, 0x0);
	rboxid_clear_pixels(r_id2, 0x0);
	dialog_add_simple(r_id, 1, (char *)ca_Hel, 0, 1, 0xFF, 0);
	dialog_add_simple(r_id2, 1, (char *)ca_And, 0, 1, 0xFF, 0);
	rboxid_tilemap_update(r_id);
	rboxid_tilemap_update(r_id2);
	rboxid_upload_a(r_id, 1);
	rboxid_upload_a(r_id2, 1);
	u8 t_id = task_add(merger, 1);
	tasks[t_id].priv[1] = r_id;
	tasks[t_id].priv[2] = r_id2;
	gpu_pal_apply((u8 *)testingPal, 7 * 16, 32);
};*/
