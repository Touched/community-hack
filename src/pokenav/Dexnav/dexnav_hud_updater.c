#include <pokeagb/pokeagb.h>
#include "dexnav_hud.h"

static struct DexnavHudData** PokePadState = (struct DexnavHudData**)(0x203C000 + SPECIES_MAX);
static u8* SearchLevels = (u8*)0x203C000;

void dexnav_message_show(u8 id) {
    switch(id) {
        case 0:
        {
            extern void msg_normal(pchar *);
            pchar empty_string[] = _"The Pokémon got away!\p";
            msg_normal(empty_string);
            break;
        }
        case 1:
        {
            extern void msg_normal(pchar *);
            pchar empty_string[] = _"There is no reaction.\nThe signal was lost!\p";
            msg_normal(empty_string);
            break;
        }
    };
    return;
}

void dexhud_hblank(void) {
    if (REG_VCOUNT > (160 - 19)) { // fill 19 pixels

        //WRITE_REG_WININ(0x3F3F);
        REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_BLD,  WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_BLD);
                        
        //WRITE_REG_BLDCNT(0x0FFE);
        REG_BLDCNT = (BLDCNT_BG1_SRC | BLDCNT_BG2_SRC | BLDCNT_BG3_SRC | BLDCNT_SPRITES_SRC | BLDCNT_BACKDROP_SRC | BLDCNT_BG0_DST | BLDCNT_BG1_DST | BLDCNT_BG2_DST | BLDCNT_BG3_DST | BLDCNT_DARKEN);
        REG_BLDY = 0xC;
        

    } else {
        // default values from overworld
        // WRITE_REG_WININ(0x1F1F);
        REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ, WIN_BG0 |
                        WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
        //WRITE_REG_BLDCNT(0x401E);
        REG_BLDCNT = BLDALPHA_BUILD(BLDCNT_BG1_SRC | BLDCNT_BG2_SRC | BLDCNT_BG3_SRC | BLDCNT_SPRITES_SRC, 0);
    }
}

void dexnav_proximity_update() {
    extern u8 get_distance_player(s16 x, s16 y);
    (*PokePadState)->proximity =  get_distance_player((*PokePadState)->tile_x, (*PokePadState)->tile_y);
}

void dexnav_icons_vision_update(u8 proximity, u8 search_level) {
  /*
        The sight eye and species icon are always drawn in the HUD
        At search level 2, first move is drawn
        At search level 3, Ability and Held item are drawn
        At search level 5, Potential is drawn
    */
    
    // species and sight/info are always shown, regardless of proximity
    objects[(*PokePadState)->obj_id_species].final_oam.affine_mode = 0;
    objects[(*PokePadState)->obj_id_sight].final_oam.affine_mode = 0;
    
    // hide everything at the start, and recalc what needs to be shown
    objects[(*PokePadState)->obj_id_move].final_oam.affine_mode = 2;
    objects[(*PokePadState)->obj_id_ability].final_oam.affine_mode = 2;
    objects[(*PokePadState)->obj_id_item].final_oam.affine_mode = 2;
    objects[(*PokePadState)->obj_id_potential[0]].final_oam.affine_mode = 2;
    objects[(*PokePadState)->obj_id_potential[1]].final_oam.affine_mode = 2;
    objects[(*PokePadState)->obj_id_potential[2]].final_oam.affine_mode = 2;
    
    extern void dexnav_sight_update(u8);
    if (proximity < 3) {
        // at proximity 5, the sight should start to show and indicate sneaking is required
        dexnav_sight_update(0); // alert level TODO: Update in a better way utiliting sight_level
        if (search_level > 1) {
            // show move, hide others
            objects[(*PokePadState)->obj_id_move].final_oam.affine_mode = 0;
        }
        if (search_level > 2) {
            // show ability, move, hide others
            objects[(*PokePadState)->obj_id_ability].final_oam.affine_mode = 0;
            if ((*PokePadState)->held_item) {
                // toggle item view
                objects[(*PokePadState)->obj_id_item].final_oam.affine_mode = 0;
            }
        }
        if (search_level > 4) {
            objects[(*PokePadState)->obj_id_potential[0]].final_oam.affine_mode = 0;
            objects[(*PokePadState)->obj_id_potential[1]].final_oam.affine_mode = 0;
            objects[(*PokePadState)->obj_id_potential[2]].final_oam.affine_mode = 0;
        }

    } else if (proximity < 6) {
        dexnav_sight_update(1); // Sneaking is required flag
    } else {
        dexnav_sight_update(2); // Sneaking is not required
    }
}

void nullsubhblank(void) {
    return;
}


void dexnav_hud_free() {
    switch ((*PokePadState)->environment) {
        case 0:
            {
            if (!is_light_level_1_2_3_or_6__opensky(currentmap_header.light)) {
                // cave
            } else {
                oe_stop(&objects[(*PokePadState)->obj_id_shaking_grass], 0x13);
            }
            break;
            }
        case 1:
            oe_stop(&objects[(*PokePadState)->obj_id_shaking_grass], 0x13);
            break;
    };
    obj_free(&objects[(*PokePadState)->obj_id_species]);
    obj_delete((*PokePadState)->obj_id_species);
    
    obj_free(&objects[(*PokePadState)->obj_id_sight]);
    obj_delete((*PokePadState)->obj_id_sight);
    
    obj_free(&objects[(*PokePadState)->obj_id_ability]);
    obj_delete((*PokePadState)->obj_id_ability);
    
    obj_free(&objects[(*PokePadState)->obj_id_move]);
    obj_delete((*PokePadState)->obj_id_move);
    
    obj_free(&objects[(*PokePadState)->obj_id_item]);
    obj_delete((*PokePadState)->obj_id_item);
    
    obj_free(&objects[(*PokePadState)->obj_id_potential[0]]);
    obj_delete((*PokePadState)->obj_id_potential[0]);
    
    obj_free(&objects[(*PokePadState)->obj_id_potential[1]]);
    obj_delete((*PokePadState)->obj_id_potential[1]);
    
    obj_free(&objects[(*PokePadState)->obj_id_potential[2]]);
    obj_delete((*PokePadState)->obj_id_potential[2]);
  
    gpu_tile_obj_free_by_tag(0x4736);
    gpu_tile_obj_free_by_tag(0x61);
    gpu_tile_obj_free_by_tag(0x2613);
    gpu_tile_obj_free_by_tag(0x5424);
    gpu_tile_obj_free_by_tag(0x8472);
    gpu_tile_obj_free_by_tag(0x3039);
    gpu_tile_obj_free_by_tag(0x1EE7);
    gpu_pal_free_by_tag(0x8472);
    gpu_pal_free_by_tag(0x3039);

    free((void *)*PokePadState); 
    interrupts_disable(2);
    hblank_handler_set(nullsubhblank); 
    
    // WRITE_REG_WININ(0x1F1F);
    REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ, WIN_BG0 |
                            WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
    //WRITE_REG_BLDCNT(0x401E);
    REG_BLDCNT = BLDALPHA_BUILD(BLDCNT_BG1_SRC | BLDCNT_BG2_SRC | BLDCNT_BG3_SRC | BLDCNT_SPRITES_SRC, 0);
}


void dexnav_hud_manage(u8 task_id) {
    // check for out of range
    if ((*PokePadState)->proximity > 20) {
        task_del(task_id);
        dexnav_hud_free();
        dexnav_message_show(1);        
        return;
    }

    // check for timeout.
    tasks[task_id].priv[1]++;
    if (tasks[task_id].priv[1] > (0xFFFF)) {
        task_del(task_id);
        dexnav_hud_free();
        dexnav_message_show(0);
        return;
    }
    
    // check if script just executed
    if (script_env_2_is_enabled()) {
        task_del(task_id);
        dexnav_hud_free();
        return;
    }
    
    // caves and water the pokemon moves around
    if ((((*PokePadState)->environment == 1) || (!is_light_level_1_2_3_or_6__opensky(currentmap_header.light)))
    && ((*PokePadState)->proximity < 2) && ((*PokePadState)->movement_times < 2)) {
        switch((*PokePadState)->environment) {
            case 0:
                oe_stop(&objects[(*PokePadState)->obj_id_shaking_grass], 0x13);
                break;
            case 1:
                oe_stop(&objects[(*PokePadState)->obj_id_shaking_grass], 0x16);
                break;
            default:
                break;
        };
        extern u8 shaking_grass(u8, u8, u8, bool);
        while(!(shaking_grass((*PokePadState)->environment, 8, 8, 1))) {
            __asm__("mov r8, r8"); // nop...do I even need this?
        }
        (*PokePadState)->movement_times += 1;
    }
 
    // check for encounter start
    if ((*PokePadState)-> proximity < 1) {
        extern void dexnav_gen_pkmnn(u16 species, u8 potential, u8 level, u8 ability, u16* moves);
        dexnav_gen_pkmnn((*PokePadState)->species, (*PokePadState)->potential, (*PokePadState)->pokemon_level, 
        (*PokePadState)->ability, (*PokePadState)->move_id);
        task_del(task_id);
        
        // increment the search level
        if (SearchLevels[(*PokePadState)->species] < 100) {
            SearchLevels[(*PokePadState)->species] += 1;
        }
        
        // freeing only the state, objects and hblank cleared on battle start.
        free((void *)*PokePadState); 
        
        
        // exclamation point animation over the player
        an_exclamation_mark(npc_states, &objects[walkrun_state.oamid]);
        oe_exec(0x0);
        
        // do battle
        exec_battle();
        
        
    };
    
    
    /* TODO : CHECK SNEAKING */

    
    // HUD needs updating iff player has moved
    dexnav_proximity_update();
    if (!(tasks[task_id].priv[0] == (*PokePadState)->proximity)) {
        dexnav_icons_vision_update((*PokePadState)->proximity, (*PokePadState)->search_level);
        tasks[task_id].priv[0] = (*PokePadState)->proximity;
    }
    
}







