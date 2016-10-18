
#include <pokeagb/pokeagb.h>
#include "hpbar_gfx.h"

#define BITS_TOXIC 0x80
#define BITS_POISON 0x8
#define BITS_PARALYZE 0x40
#define BITS_SLEEP 0x7
#define BITS_FREEZE 0x20
#define BITS_BURN 0x10

#define TILE_SIZE 32
#define NAME_TILE_PLAYER 10
#define NAME_TILE_OPPONENT 9
#define LEVEL_TILE_SINGLE_PLAYER 74
#define LEVEL_TILE_OPPONENT 40
#define LEVEL_TILE_DOUBLES_PLAYER 42
#define HEALTH_NUMS_TILE_PLAYER 30
#define AILMENT_TILE_SINLE_PLAYER 26
#define AILMENT_TILE_DOUBLE_PLAYER 17
#define AILMENT_TILE_OPPONENT 16


void hp_string_to_oam (u8 obj_id, u8 tile_num) {
	u8 tile = objects[obj_id].final_oam.tile_num + tile_num;
	u8 c_tile = tile_num;
	u8* dst = (u8*)((tile * TILE_SIZE) + (SPRITE_RAM));
	pchar* string_buff_ptr = string_buffer;
	pchar element = *string_buff_ptr;
	
	u8 size;
		u8 obj_tiles_total;
		switch (objects[obj_id].final_oam.shape) {
			case 0: //square
			{
				if ((objects[obj_id].final_oam.size) > 2) {
					size = 8;
					obj_tiles_total = 8 * 8;
				} else if ((objects[obj_id].final_oam.size) > 1) {
					size = 4;
					obj_tiles_total = 4 * 4;
				} else if ((objects[obj_id].final_oam.size) > 0) {
					size = 2;
					obj_tiles_total = 2 * 2;
				} else {
					size = 1;
					obj_tiles_total = 1 * 1;
				}
				break;
			}
			case 1: //horizontal
			{
				if ((objects[obj_id].final_oam.size) > 2) {
					size = 8;
					obj_tiles_total = 8 * 4;
				} else if ((objects[obj_id].final_oam.size) > 1) {
					size = 4;
					obj_tiles_total = 4 * 2;
				} else if ((objects[obj_id].final_oam.size) > 0) {
					size = 2;
					obj_tiles_total = 4 * 1;
				} else {
					size = 1;
					obj_tiles_total = 2 * 1;
				}
				break;
			}
			default: //vertical
			{
				if ((objects[obj_id].final_oam.size) > 2) {
					size = 4;
					obj_tiles_total = 8 * 4;
				} else if ((objects[obj_id].final_oam.size) > 1) {
					size = 2;
					obj_tiles_total = 4 * 2;
				} else if ((objects[obj_id].final_oam.size) > 0) {
					size = 1;
					obj_tiles_total = 4 * 1;
				} else {
					size = 1;
					obj_tiles_total = 2 * 1;
				}
				break;
			}
		};
	c_tile = c_tile % size;
	while (element != 0xFF) {
		if (element == 0xBA) {
			// slash
			memcpy((void*)dst, (void*)&bigHP_numsTiles[0], TILE_SIZE);
		} else {
			memcpy((void*)dst, (void*)(&bigHP_numsTiles[(element - 0xA0) * TILE_SIZE]), TILE_SIZE);
		}
		if ((c_tile == (size -1))) {
			dst += (TILE_SIZE * ((obj_tiles_total - size) + 1));
			c_tile = 0;
		} else {
			dst += TILE_SIZE; // next tile
			c_tile++;
		}
		string_buff_ptr++;
		element = *string_buff_ptr;
	}
}


void outlined_font_draw(u8 obj_id, u8 tile_num, u16 size) {
	u8 tile = objects[obj_id].final_oam.tile_num + tile_num;
	u8* towrite = (u8*)((tile * TILE_SIZE) + (SPRITE_RAM));
	u8* dst = (u8*)0x203D000;
	pchar *string_buff_ptr = string_buffer;
	pchar element = *string_buff_ptr;
	u8 counter = 1;
	u16 index = 320;
	u16 prev_index;
	while (element != 0xFF) {
		prev_index = index;
		if ((element <= 0xEE) && (element >= 0xD5)) {
			// lower case letters
			index = (((element - 0xD5) * TILE_SIZE) + 1600);	
		} else if ((element <= 0xD4) && (element >= 0xBB)) {	
			// upper case letters
			index = (((element - 0xBB) * TILE_SIZE) + 768);	
		} else if ((element <= 0xAA) && (element >= 0xA1)) {
			// numbers
			index = (element - 0xA1) * TILE_SIZE;	
		} else {
			// misc pchars
			u8 symbol_id = 0;
			switch (element) {
				case 0xF0: // colon
				case 0x0: // space bar
				{
					symbol_id = 1;
					break;
				}
				case 0x36: // semi colon used indication of str end
				{
					symbol_id = 2;
					break;
				}
				case 0xAC: // question mark
				{
					symbol_id = 3;
					break;
				}
				case 0xAE: // dash
				{
					symbol_id = 4;
					break;
				}
				case 0xAD: // period
				{
					symbol_id = 5;
					break;
				}
				case 0xBA: // slash
				{
					symbol_id = 6;
					break;
				}
				case 0xB1: // open double quote
				{
					symbol_id = 7;
					break;
				}
				case 0xB2: // close double quote
				{
					symbol_id = 8;
					break;
				}
				case 0xB3: // open single quote
				{
					symbol_id = 9;
					break;
				}
				case 0xB4: // close single quote
				{
					symbol_id = 10;
					break;
				}
				case 0xB0: // elipsis ...
				{
					symbol_id = 11;
					break;
				}
				case 0xB8: // comma
				{
					symbol_id = 12;
					break;
				}
				case 0xB5: // male
				{
					symbol_id = 13;
					//dst = 
					break;
				}
				case 0xB6: // f
				{
					symbol_id = 14;
					break;
				}
				case 0xFF: // empty
				{
					symbol_id = 1;
					break;
				}
			};
			index = (symbol_id + 9) * TILE_SIZE;
		}
		
		/* TODO: Use macros here */
		
		if ((counter == 0) || (*(string_buff_ptr + 1) == 0xFF))  {
			// first or last pcharacters don't need pixel merging
			memcpy((void*)dst, (void*)(&outlined_names_fontTiles[index]), TILE_SIZE);
		} else if ((element == 0x0)){
			memcpy((void*)dst, (void*)(&outlined_names_fontTiles[index]), TILE_SIZE);
			u8 *prev_letter = (u8*)(&outlined_names_fontTiles[prev_index]);
			*(dst + 0) = *(prev_letter + 2);
			*(dst + 4) = *(prev_letter + 6);
			*(dst + 8) = *(prev_letter + 10);
			*(dst + 12) = *(prev_letter + 14);
			*(dst + 16) = *(prev_letter + 18);
			*(dst + 20) = *(prev_letter + 22);
			*(dst + 24) = *(prev_letter + 26);
			*(dst + 28) = *(prev_letter + 30);			
		} else if ((*(string_buff_ptr + 1) != 0xFF)) {
		
			// pcharacter in middle, if blank space fill blank with previous pcharacter's last pixel row IFF previous pchar's last pixel row non-empty
			memcpy((void*)dst, (void*)(&outlined_names_fontTiles[index]), TILE_SIZE);
			u8 *prev_letter = (u8*)(&outlined_names_fontTiles[prev_index]);
			*(dst) |= (((*(prev_letter + 0) & 0xF) == 0) ? (*(dst + 0) & 0xF) : (*(prev_letter + 0) & 0xF));
			*(dst + 4) |= (((*(prev_letter + 4) & 0xF) == 0) ? (*(dst + 4) & 0xF) : (*(prev_letter + 4) & 0xF));
			*(dst + 8) |= (((*(prev_letter + 8) & 0xF) == 0) ? (*(dst + 8) & 0xF) : (*(prev_letter + 8) & 0xF));
			*(dst + 12) |= (((*(prev_letter + 12) & 0xF) == 0) ? (*(dst + 12) & 0xF) : (*(prev_letter + 12) & 0xF));
			*(dst + 16) |= (((*(prev_letter + 16) & 0xF) == 0) ? (*(dst + 16) & 0xF) : (*(prev_letter + 16) & 0xF));
			*(dst + 20) |= (((*(prev_letter + 20) & 0xF) == 0) ? (*(dst + 20) & 0xF) : (*(prev_letter + 20) & 0xF));
			*(dst + 24) |= (((*(prev_letter + 24) & 0xF) == 0) ? (*(dst + 24) & 0xF) : (*(prev_letter + 24) & 0xF));
			*(dst + 28) |= (((*(prev_letter + 28) & 0xF) == 0) ? (*(dst + 28) & 0xF) : (*(prev_letter + 28) & 0xF));
		}
		
		if ((counter == 2) && (*(string_buff_ptr + 1) != 0xFF)) {
			// every two pchars, we need to merge
			// 8x8px made of 4x8px from previous pchar and 4x8px of this pchar
			*(dst - 30) = (((*(dst - 30) & 0x0F) == 0) ? (*(dst) & 0xF) :(*(dst - 30) & 0x0F)) | (*(dst) & 0xF0);
			*(dst - 26) = (((*(dst - 26) & 0x0F) == 0) ? (*(dst + 4) & 0xF): (*(dst - 26) & 0x0F))  | (*(dst + 4) & 0xF0);
			*(dst - 22) = (((*(dst - 22) & 0x0F) == 0) ? (*(dst + 8) & 0xF): (*(dst - 22) & 0x0F)) | (*(dst + 8) & 0xF0);
			*(dst - 18) = (((*(dst - 18) & 0x0F) == 0) ? (*(dst + 12) & 0xF): (*(dst - 18) & 0x0F)) | (*(dst + 12) & 0xF0);
			*(dst - 14) = (((*(dst - 14) & 0x0F) == 0) ? (*(dst + 16) & 0xF): (*(dst - 14) & 0x0F)) | (*(dst + 16) & 0xF0);
			*(dst - 10) = (((*(dst - 10) & 0x0F) == 0) ? (*(dst + 20) & 0xF): (*(dst - 10) & 0x0F)) | (*(dst + 20) & 0xF0);
			*(dst - 6) = (((*(dst - 6) & 0x0F) == 0) ? (*(dst + 24) & 0xF): (*(dst - 6) & 0x0F)) | (*(dst + 24) & 0xF0);
			*(dst - 2) = (((*(dst - 2) & 0x0F) == 0) ? (*(dst + 28) & 0xF): (*(dst - 2) & 0x0F)) | (*(dst + 28) & 0xF0);
			
			// last two pixels unconditional
			*(dst - 29) |= *(dst + 1);
			*(dst - 25) |= *(dst + 5);
			*(dst - 21) |= *(dst + 9);
			*(dst - 17) |= *(dst + 13);
			*(dst - 13) |= *(dst + 17);
			*(dst - 9) |= *(dst + 21);
			*(dst - 5) |= *(dst + 25);
			*(dst - 1) |= *(dst + 29);

			dst -= TILE_SIZE;
			counter = 0;
		}
		counter++;
		dst += TILE_SIZE; // next tile
		string_buff_ptr++;
		element = *string_buff_ptr;	
	}
	memcpy((void*)towrite, (void*)(0x203D000), size);
	memset((void*)(0x203D000), 0x0, size + TILE_SIZE);
}


void draw_hp_nums(struct Pokemon* pokemon, u8 obj_id, u8 t_id) {
	//u8 tile_id = t_id;
	u8 tile_id = 30;
	pchar empty_string[9] = _"000/000";//{0xAB, 0xAB, 0xAB, 0xBA, 0xAB, 0xAB, 0xAB, 0xFF};
	pstrcpy(string_buffer, empty_string);
	hp_string_to_oam(obj_id, tile_id - 1);
	
	u16 max_hp = get_attr(pokemon, REQUEST_TOTL_HP);
	u16 c_hp = get_attr(pokemon, REQUEST_CURRENT_HP);
	fmt_int_10(string_buffer, c_hp, 0, 3);
	u8 str_len = string_length(string_buffer);
	*(string_buffer + str_len - 1) = 0xBA;
	fmt_int_10(string_buffer + str_len, max_hp, 0, 3);
	hp_string_to_oam(obj_id, tile_id + (3 - str_len));
}


void hp_nums_update(u8 obj_id, u16 new_hp, u8 t_id) {
	if ((objects[obj_id].pos1.x == TILE_SIZE) || battle_type_is_double()) {
		return;
	}
	if (t_id) {
		// max HP
		fmt_int_10(string_buffer, new_hp, 0, 3);
		hp_string_to_oam(obj_id, 30 + 4);
	} else {
		// current HP
		pchar value[4];
		pchar empty_value[4] = _"000";//{0xAB, 0xAB, 0xAB, 0xFF};
		pstrcpy(string_buffer, empty_value);
		fmt_int_10(value, new_hp, 0, 3);
		u8 str_len = string_length(value);
		pstrcpy(((string_buffer + 3) - str_len), value);
		hp_string_to_oam(obj_id, 30);
	}
}


void draw_name_gender(struct Pokemon* pokemon, u8 obj_id, u8 tile_id) {
	char male [] = {0x0, 0xB5, 0xFF};
	char female [] = {0x0, 0xB6, 0xFF};
	u8 gender = pokemon_get_gender(pokemon);
	
	string_buffer[0] = 0x0;
	string_buffer[1] = 0xFF;
	outlined_font_draw(obj_id, tile_id -1, TILE_SIZE);
	
	memcpy(string_buffer, pokemon->name, 10);
	memset(string_buffer + 10, 0xFF, 1);
	u8 str_len = string_length(string_buffer);
	
	if (!gender) {
		pstrcpy(string_buffer + str_len - 1, (pchar*)male + !(str_len % 2));
	}
	if (gender == 0xFE) {
		pstrcpy(string_buffer + str_len - 1, (pchar*)female + !(str_len % 2));	
	}
	if (str_len > 10) {
		// long names need to be force shifted 1 tile left and takes an extra tile of space 
		outlined_font_draw(obj_id, tile_id - 1, 7 * TILE_SIZE);
	} else {
		outlined_font_draw(obj_id, tile_id, 6 * TILE_SIZE);
	}
}


void draw_level(struct Pokemon* pokemon, u8 obj_id, u8 tile_id) {
	u8 level = get_attr(pokemon, REQUEST_LEVEL);
	fmt_int_10(string_buffer, level, 0, 3);
	pchar period [] = _"   ";
	pstrcat(string_buffer, (pchar *)period);
	outlined_font_draw(obj_id, tile_id, 3 * TILE_SIZE);
	return;
};


void draw_hp_bar(u8 oam_side_maybe, u8 obj_id, struct Pokemon* p) {
	load_gfxc_health_bar();
	u16 max_hp = get_attr(p, REQUEST_TOTL_HP);
	u16 c_hp = get_attr(p, REQUEST_CURRENT_HP);
	
	// syncs the data with dp11 structure index[0], which is the HP bar
	hpbox_data_set(oam_side_maybe, obj_id, max_hp, c_hp, 0);
	// deals with gfx commit
	sync_hpbox_vram(oam_side_maybe, obj_id, 0);
	
	if ((oam_side_maybe < 1) && (!battle_type_is_double())) {
		u16 species = get_attr(p, REQUEST_SPECIES);
		u8 level = get_attr(p, REQUEST_LEVEL);
		u32 total_exp = get_attr(p, REQUEST_EXP_POINTS);
		u32 species_exp_index = (pokemon_base_stats[species].exp_growth * 0x194);
		u32 *exp_have = (u32*) (0x8253AE4 + (species_exp_index + (level * 4)));
		u32 *exp_needed = (u32*) (0x8253AE8 + (species_exp_index + (level * 4)));
		
		// syncs the data with dp11 structure index[1], which is the exp bar
		hpbox_data_set(oam_side_maybe, obj_id, (*exp_needed - *exp_have), (total_exp - *exp_have),  0);
		sync_hpbox_vram(oam_side_maybe, obj_id, 1);
	}
}


void draw_ailment(struct Pokemon* p, u8 obj_id, u8 tile_num, u8 type) {
	u8 *src;
	
	// single's status graphic is too big for doubles. Doubles uses a second graphic
	if (!type) {
		src = (u8*)singles_ailmentsTiles;
	} else {
		src = (u8*)doubles_ailmentsTiles;
	}
	
	// draw status ailment into tile ID of obj
	u8 tile = objects[obj_id].final_oam.tile_num + tile_num;
	u8 *dst = (u8*)((tile * TILE_SIZE) + (SPRITE_RAM));
	u16 ailment = get_attr(p, REQUEST_STATUS_AILMENT);
	if (ailment & BITS_TOXIC) {
		memcpy((void*)dst, (void*)(src + (TILE_SIZE * 15)), TILE_SIZE * 3);
	} else if (ailment & BITS_POISON) {
		memcpy((void*)dst, (void*)(src + (TILE_SIZE * 0)), TILE_SIZE * 3);
	} else if (ailment & BITS_PARALYZE) {
		memcpy((void*)dst, (void*)(src + (TILE_SIZE * 3)), TILE_SIZE * 3);
	} else if (ailment & BITS_SLEEP) {
		memcpy((void*)dst, (void*)(src + (TILE_SIZE * 6)), TILE_SIZE * 3);
	} else if (ailment & BITS_FREEZE) {
		memcpy((void*)dst, (void*)(src + (TILE_SIZE * 9)), TILE_SIZE * 3);
	} else if (ailment & BITS_BURN) {
		memcpy((void*)dst, (void*)(src + (TILE_SIZE * 12)), TILE_SIZE * 3);
	} else {
		// Covers Edge case when ailment pokemon swaps out, and OAM is not refreshed.
		memcpy((void*)dst, (void*)(src + (TILE_SIZE * 18)), TILE_SIZE * 3);
	}
	
}


void draw_hp_box_elements(u8 obj_id, struct Pokemon* p, u8 battle_style) {
	// 0-4 value indicating ID for DP11 structure
	u8 oam_side_maybe = objects[obj_id].private[6] & 0xFF;
        
	// battle is single
	if (!battle_type_is_double()) {
		
		// player's team member
		if ((u32)(p) >= 0x02024284) {
			draw_name_gender(p, obj_id, NAME_TILE_PLAYER);
			draw_level(p, obj_id, LEVEL_TILE_SINGLE_PLAYER);
			draw_hp_nums(p, obj_id, HEALTH_NUMS_TILE_PLAYER);
			draw_hp_bar(oam_side_maybe, obj_id, p);
			draw_ailment(p, obj_id, AILMENT_TILE_SINLE_PLAYER, 0);
			
		// opponent's team member
		} else {
			objects[obj_id].pos1.x = TILE_SIZE;
			draw_name_gender(p, obj_id, NAME_TILE_OPPONENT);
			draw_level(p, obj_id, LEVEL_TILE_OPPONENT);
			draw_hp_bar(oam_side_maybe, obj_id, p);
			draw_ailment(p, obj_id, AILMENT_TILE_OPPONENT, 1);
			
		}
	
	// battle is double 
	} else {
	
		//player's team member
		if ((u32)(p) >= 0x02024284) {
			draw_name_gender(p, obj_id, NAME_TILE_PLAYER);
			draw_level(p, obj_id, LEVEL_TILE_DOUBLES_PLAYER);
			draw_hp_bar(oam_side_maybe, obj_id, p);
			draw_ailment(p, obj_id, AILMENT_TILE_DOUBLE_PLAYER, 1);
		} else {
		
		// opponent's team member
			objects[obj_id].pos1.x = TILE_SIZE;
			draw_name_gender(p, obj_id, NAME_TILE_OPPONENT);
			draw_level(p, obj_id, LEVEL_TILE_OPPONENT);
			draw_hp_bar(oam_side_maybe, obj_id, p);
			draw_ailment(p, obj_id, AILMENT_TILE_OPPONENT, 1);
		}
		return;
		
	}
};


int main() {
	return 1;
}

