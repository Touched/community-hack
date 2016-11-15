#include <pokeagb/pokeagb.h>
#include "dexnav_hud.h"
#include "../../../generated/images/pokenav/dexnav_stars.h"
#include "../../../generated/images/pokenav/empty.h"
#include "../../../generated/images/pokenav/cave_smoke.h"

extern void outlined_font_draw(u8 obj_id, u8 tile_num, u16 size, u8 font);

// place holders
static struct DexnavHudData** PokePadState = (struct DexnavHudData**)(0x203C000 + SPECIES_MAX);
const struct SpritePalette heldpal = {(const u8*)&pal_heldicons, 0x8472};

// 32x64 oam with highest priority
const struct OamData font_oam = {    .y = ICONY,
                                    .affine_mode = 0,
                                    .obj_mode = 0,
                                    .mosaic = 0,
                                    .bpp = 0,
                                    .shape = 1,
                                    .x = ICONX,
                                    .matrix_num = 0,
                                    .size = 3,
                                    .tile_num = 0,
                                    .priority = 0,
                                    .palette_num = 0,
                                    .affine_param = 0
};

// 8x8 oam with highest priority
const struct OamData held_oam = {   .y = ICONY,
                                    .affine_mode = 0,
                                    .obj_mode = 0,
                                    .mosaic = 0,
                                    .bpp = 0,
                                    .shape = 0,
                                    .x = ICONX,
                                    .matrix_num = 0,
                                    .size = 0,
                                    .tile_num = 0,
                                    .priority = 0,
                                    .palette_num = 0,
                                    .affine_param = 0
};                            



void dexnav_draw_ability(enum PokemonAbility ability, u8* objid) {
    
    // create empty object of size 32x64 to draw font on
    const struct SpriteTiles fontsprite_ability = {(const u8*)(&emptyTiles), 0x800, 0x1EE7};
    const struct Template font_temp_ability = {0x1EE7, 0x8472, &font_oam, (struct Frame**)0x8231CF0, 0x0,
                                (struct RotscaleFrame**)0x8231CFC, (ObjectCallback)0x800760D}; 
    gpu_tile_obj_alloc_tag_and_upload(&fontsprite_ability);
    gpu_pal_obj_alloc_tag_and_apply(&heldpal);
    u8 obj_id = template_instanciate_forward_search(&font_temp_ability, ICONX + 80, ICONY + 0x12, 0x0);
    *objid = obj_id;
    objects[obj_id].final_oam.affine_mode = 2;
    
    // ability name beside move name
    u8 len = (*PokePadState)->move_name_length;
    objects[obj_id].pos1.x += ((8 * (len/2)) + (4 * (len % 2)));
    
    // Copy ability string from table using state id
    memcpy((void *)(string_buffer), (void *)&(pokemon_ability_names[ability]), POKEAGB_ABILITY_NAME_LENGTH);

    // format string so it's even length or if it's odd ends in two spaces
    string_buffer[POKEAGB_ABILITY_NAME_LENGTH + 1] = 0xFF;
    len = pstrlen(string_buffer);
    if (!(len % 2)) {
        string_buffer[len] = 0x0;
        string_buffer[len + 1] = 0x0;
        string_buffer[len + 2] = 0xFF;
    }
    
    // write name to object
    outlined_font_draw(obj_id, 0, 32 * 8, 0);
    objects[obj_id].final_oam.obj_mode = 1;
    return;
}


void dexnav_draw_move(u16 move, u8 search_level, u8* objid) {
    // create empty object of size 32x64 to draw font on
    const struct SpriteTiles fontsprite_move = {(const u8*)(&emptyTiles), 0x800, 0x4736};
    const struct Template font_temp_move = {0x4736, 0x8472, &font_oam, (struct Frame**)0x8231CF0, 0x0,
                            (struct RotscaleFrame**)0x8231CFC, (ObjectCallback)0x800760D}; 
    gpu_tile_obj_alloc_tag_and_upload(&fontsprite_move);
    gpu_pal_obj_alloc_tag_and_apply(&heldpal);                            
    u8 obj_id = template_instanciate_forward_search(&font_temp_move, ICONX + 80, ICONY + 0x12, 0x0);
    *objid = obj_id;
    objects[obj_id].final_oam.affine_mode = 2;
    
    // Copy move string from table using state id, add '/' character to the end of it
    memcpy((void *)string_buffer, (void *)pokemon_move_names[move], POKEAGB_MOVE_NAME_LENGTH);
    string_buffer[POKEAGB_MOVE_NAME_LENGTH + 1] = 0xFF;
    u8 len = pstrlen(string_buffer);
    
    if (search_level > 2) {
        string_buffer[len] = 0xBA;
        len += 1;
    }
    
    // record length of move with slash for ability name to be placed beside it
    (*PokePadState)->move_name_length = len;
    
    // adjust string to be even chars, if odd end in two spaces
    if (!(len % 2)) {
        string_buffer[len] = 0x0;
        string_buffer[len + 1] = 0x0;
        string_buffer[len + 2] = 0xFF;
    } else {
        string_buffer[len] = 0xFF;
    }
    
    // write name to object
    outlined_font_draw(obj_id, 0, 32 * 8, 0);
    objects[obj_id].final_oam.obj_mode = 1;
}


void dexnav_draw_potential(u8 potential, u8* objid) {
    // 19 tiles per row, stars are on the 4th row. 1 tile is 32 bytes. Hence 19 * 4 *32
    const struct SpriteTiles staricon_lit = {(const u8*)(&(dexnav_starsTiles[19 * 4 * 32])), 64, 0x61};
    const struct SpriteTiles staricon_off = {(const u8*)(&(dexnav_starsTiles[((19 * 4) + 1) *32])), 64, 0x2613};
    
    // TODO: put these function pointers and other data ptrs in PokeAGB.
    const struct Template star_lit_temp = {0x61, 0x8472, &held_oam, (struct Frame**)0x8231CF0, 0x0,
                                        (struct RotscaleFrame**)0x8231CFC, (ObjectCallback)0x800760D};
    const struct Template star_off_temp = {0x2613, 0x8472, &held_oam, (struct Frame**)0x8231CF0, 0x0,
                                        (struct RotscaleFrame**)0x8231CFC, (ObjectCallback)0x800760D};
                                        
    // allocate both the lit and unlit star to VRAM
    gpu_tile_obj_alloc_tag_and_upload(&staricon_lit);
    gpu_tile_obj_alloc_tag_and_upload(&staricon_off);
    gpu_pal_obj_alloc_tag_and_apply(&heldpal);
    
    // create star objects and space them according to potential 0 - 3
    u8 obj_id;
    u8 i;
    for (i = 0; i < 3; i++) {
        if (potential > i) {
            obj_id = template_instanciate_forward_search(&star_lit_temp, ICONX + 23 + (i * 8), ICONY + 0x5, 0x0);
        } else {
            obj_id = template_instanciate_forward_search(&star_off_temp, ICONX + 23 + (i * 8), ICONY + 0x5, 0x0);
        }
        objid[i] = obj_id;
        objects[obj_id].final_oam.affine_mode = 2;
        objects[obj_id].final_oam.obj_mode = 1;
    }

    
}


void dexnav_draw_sight(u8 sight_lvl, u8* objid) {

    // create empty object of size 64x32 to draw icons on
    const struct SpriteTiles sight_tiles = {(const u8*)(&emptyTiles), 0x800, 0x5424};
    const struct Template font_temp_sight = {0x5424, 0x8472, &font_oam, (struct Frame**)0x8231CF0, 0x0,
                                            (struct RotscaleFrame**)0x8231CFC, (ObjectCallback)0x800760D};
    gpu_tile_obj_alloc_tag_and_upload(&sight_tiles);
    gpu_pal_obj_alloc_tag_and_apply(&heldpal);
    u8 obj_id = template_instanciate_forward_search(&font_temp_sight, ICONX + 192, ICONY + 0x12, 0x0);
    *objid = obj_id;
    objects[obj_id].final_oam.affine_mode = 2;
    extern void dexnav_sight_update(u8);
    dexnav_sight_update(sight_lvl);
    objects[obj_id].final_oam.obj_mode = 1;
}

void dexnav_sight_update(u8 sight) {
    u8 obj_id = (*PokePadState)->obj_id_sight;
    // draw sight eye on first tile, takes up two tiles 
    u8 tileid = objects[obj_id].final_oam.tile_num;
    u8* towrite = (u8*)((tileid * 32) + (SPRITE_RAM));
    memcpy((void*)towrite, (void*) &(dexnav_starsTiles[((19 * 4) + (7 - (2 * sight))) * 32]), 64);
    
    // draw the B button tile 
    memcpy((void*)(towrite + 128), (void*) &(dexnav_starsTiles[((19 * 4) + 2) *32]), 64);
    
    // draw info text on the 5th tile
    pchar back[] = _" Back  ";
    pstrcpy(string_buffer, back);
    outlined_font_draw(obj_id, 5, 32 * 8, 0);
}


void dexnav_draw_helditem(u8* objid) {
    // create object for held item icon
    const struct SpriteTiles heldicon = {(const u8*)(&gfx_heldicons), 64, 0x8472};
    const struct Template heldtemp = {0x8472, 0x8472, &held_oam, (struct Frame**)0x8231CF0, 0x0,
                                    (struct RotscaleFrame**)0x8231CFC, (ObjectCallback)0x800760D};
     gpu_tile_obj_alloc_tag_and_upload(&heldicon);
    gpu_pal_obj_alloc_tag_and_apply(&heldpal);
    u8 obj_id = template_instanciate_forward_search(&heldtemp, ICONX + 0x8, ICONY + 0xC, 0x0);
    *objid = obj_id;
    objects[obj_id].final_oam.affine_mode = 2;
    objects[obj_id].final_oam.obj_mode = 1;
}


void dexnav_draw_speciesicon(u16 species, u8* objid) {

    // check which palette the species icon uses
    u8 icon_pal = pokeicon_pal_indices[species];
    const struct SpriteTiles bulbicon = {(pokeicon_table[species]), 0x400, 0x3039};
    const struct SpritePalette bulbpal = {(const u8*)&(pokeicon_pals[icon_pal]), 0x3039};
    
    // 32x32 object with priority 1, one less than held item which overlaps it
    const struct OamData picon_oam = {  .y = ICONY,
                                        .affine_mode = 0,
                                        .obj_mode = 0,
                                        .mosaic = 0,
                                        .bpp = 0,
                                        .shape = 0,
                                        .x = ICONX,
                                        .matrix_num = 0,
                                        .size = 2,
                                        .tile_num = 0,
                                        .priority = 1,
                                        .palette_num = 0,
                                        .affine_param = 0
    };
    const struct Template bulbtemp = {0x3039, 0x3039, &picon_oam, (struct Frame**)0x8231CF0, 0x0,
                                (struct RotscaleFrame**)0x8231CFC, (ObjectCallback)0x800760D};
    // create object
    gpu_tile_obj_alloc_tag_and_upload(&bulbicon);
    gpu_pal_obj_alloc_tag_and_apply(&bulbpal);
    u8 obj_id = template_instanciate_forward_search(&bulbtemp, ICONX, ICONY, 0x0);
    *objid = obj_id;
    objects[obj_id].final_oam.affine_mode = 2;
    objects[obj_id].final_oam.obj_mode = 1;
}


void oei_1A_cave() {
    const struct SpritePalette cave_smoke = {(const u8*)&cave_smokePal, 0x1005};
    gpu_pal_obj_alloc_tag_and_apply(&cave_smoke);
    gpu_pal_apply((u8 *)&cave_smokePal, 29 * 16, 32);
    const static struct SpriteTiles cave_gfx[4] = {
                                    {.data = (u8*)&cave_smokeTiles[128 * 0],
                                    .size = 0x80,
                                    .tag = 0x0
                                    },
                                    {.data = (u8*)&cave_smokeTiles[128 * 1],
                                    .size = 0x80,
                                    .tag = 0x0
                                    },
                                    {.data = (u8*)&cave_smokeTiles[128 * 2],
                                    .size = 0x80,
                                    .tag = 0x0
                                    },
                                    {.data = (u8*)(u8*)&cave_smokeTiles[128 * 3],
                                    .size = 0x80,
                                    .tag = 0x0
                                    },
                                    };
    const static struct SpriteTiles* ptr_cave_gfx = &cave_gfx[0];
    const struct Template objt_cave = { .tiles_tag = 0xFFFF,
                                        .pal_tag = 0x1005,
                                        .oam = (struct OamData*)0x83A36F0,
                                        .animation = (struct Frame**)0x83A5B70,
                                        .graphics = (ptr_cave_gfx),
                                        .rotscale = (struct RotscaleFrame**)0x8231CFC,
                                        .callback = (ObjectCallback)0x80DCD1D
    };
    log_coords_relative_camera(&oe_state.effect_pos.x, &oe_state.effect_pos.y, 8, 8);
    u8 obj_id = template_instanciate_reverse_search(&objt_cave, oe_state.effect_pos.x, oe_state.effect_pos.y, 0xFF); 
    if (obj_id != 64) {
        objects[obj_id].bitfield2 |= 2;
        objects[obj_id].private[0] = 22;
    }
    
}

void dexnav_draw_icons() {
    u8 search_level = (*PokePadState)->search_level;
    dexnav_draw_sight((*PokePadState)->proximity, &(*PokePadState)->obj_id_sight);
    dexnav_draw_speciesicon((*PokePadState)->species, &(*PokePadState)->obj_id_species);
    dexnav_draw_move((*PokePadState)->move_id[0], search_level, &(*PokePadState)->obj_id_move);
    dexnav_draw_ability((*PokePadState)->ability, &(*PokePadState)->obj_id_ability);
    dexnav_draw_helditem(&(*PokePadState)->obj_id_item);
    dexnav_draw_potential((*PokePadState)->potential, &(*PokePadState)->obj_id_potential[0]);
            
}

