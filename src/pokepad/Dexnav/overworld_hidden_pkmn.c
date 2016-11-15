#include <pokeagb/pokeagb.h>
#include "dexnav_hud.h"

static struct DexnavHudData** PokePadState = (struct DexnavHudData**)(0x203C000 + SPECIES_MAX);

#define SCANSTART_X 0
#define SCANSTART_Y 0

#define SCANSIZE_X 12
#define SCANSIZE_Y 12

u8 get_distance_player(s16 x, s16 y) {
    u16 delta_x = ABS(x - (saveblock1->camera_x + 7));
    u16 delta_y = ABS(y - (saveblock1->camera_y + 7));
    return delta_x + delta_y;
}

u8 pick_tile_screen(u8 target_behaviour, u8 area_x, u8 area_y, s16 *x_buff, s16 *y_buff, u8 find_chance, u8 small_scan) {  
    // area of map to cover starting from camera position {-7, -7}
    s16 top_x = saveblock1->camera_x - SCANSTART_X + (small_scan * 5);
    s16 top_y = saveblock1->camera_y - SCANSTART_Y + (small_scan * 5);
    s16 bot_x = top_x + area_x;
    s16 bot_y = top_y + area_y;
    u8 i = 0;
    // loop through every tile in area and evaluate
    while (top_y < bot_y) {
        while (top_x < bot_x) {

            u32 tile_behaviour = cur_mapdata_block_get_field_at(top_x, top_y, 0xFF);
            u8 block_properties = blockinfo_get_field(tile_behaviour, 4);
            u8* t = (u8 *)0x203E000;
            *(t + i) = cur_mapdata_height_mismatch(npc_states[walkrun_state.oamid].height, top_x, top_y);
            i++;
            // tile must be target behaviour (wild tile) and must be passable
            if ((target_behaviour == block_properties)){// ) {
                
                
                 // caves and water need to have their encounter values scaled higher 
                u8 weight = 0;
                if (target_behaviour == 2) {
                    // water
                    u8 scale = 320 - (small_scan * 200) - (get_distance_player(top_x, top_y) / 2);
                    weight = (((rand() % scale) <= 1) && (cur_mapdata_height_mismatch(npc_states[walkrun_state.oamid].height, top_x, top_y))) ;
                } else if (!is_light_level_1_2_3_or_6__opensky(currentmap_header.light)) {
                    // cave basically needs another check to see if the tile is passable
                    u8 scale = 440 - (small_scan * 200) - (get_distance_player(top_x, top_y) / 2)  - (2 * (top_x + top_y));
                    u8* var = (u8*)(0x20370BC);
                    *var = scale;
                    weight = ((rand() % scale) < 1) && (!blockinfo_get_field(tile_behaviour, 6));

                } else {
                    // grass
                    u8 scale = 100 - (get_distance_player(top_x, top_y) * 2);
                    weight = ((rand() % scale) <= 5);
                }

                if (weight) {
                    *x_buff = top_x;
                    *y_buff = top_y;
                    return 1;
                }
            }
            top_x += 1;
        }
        top_y += 1;
        top_x = saveblock1->camera_x - SCANSTART_X + (small_scan * 5);
    }
    return 0;
}

u8 dexnav_pokepad_picktile(u8 environment, u8 x_size, u8 y_size, bool small_scan) {
    // Pick a specific tile based on environment
    u8 target_behaviour = 0;
    switch (environment) {
        case 0:
        {
            target_behaviour = 1; // grass
            break;
        }
        default:
        {
            target_behaviour = 2; // water
            break;
        }
    };
    // tile pick chance is 5% + 1/5 search level
    u8 search_level = (*PokePadState)->search_level / 5;
    u8 chance = search_level + 1;

    return pick_tile_screen(target_behaviour, x_size, y_size, &((*PokePadState)->tile_x),
    &((*PokePadState)->tile_y) , chance, small_scan);
}

u8 shaking_grass(u8 environment, u8 x_size, u8 y_size, bool small_scan) {
    if (dexnav_pokepad_picktile(environment, x_size, y_size, small_scan)) {

        oe_state.effect_pos.x = (*PokePadState)->tile_x;
        oe_state.effect_pos.y = (*PokePadState)->tile_y;
        oe_state.priority = 0xFF; // height. 
        switch (environment) {
            case 0:
                {
                if (!is_light_level_1_2_3_or_6__opensky(currentmap_header.light)) {
                    oe_exec(0x1A);
                } else {
                    oe_exec(0x13);
                }
                break;
                }
            case 1:
                oe_exec(0x16);
                break;
            default:
                oe_exec(0x31);
                break;
        };//oe_exec(*d); //16 water, 13 grass

        // get objid of shaking grass
        u8 i;
        for (i = 0; i < 64; i++) {
            if (objects[i].callback == (ObjectCallback)0x080DCD1D) {
                (*PokePadState)->obj_id_shaking_grass = i;
                return 1;
            }
        }
        return 1;
    } else {
        return 0;
    }
    return 0;
    
}