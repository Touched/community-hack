#include <pokeagb/pokeagb.h>
#include "dexnav_hud.h"


/* Place holder */
static struct DexnavHudData** PokePadState = (struct DexnavHudData**)(0x203C000 + SPECIES_MAX);
static u8* SearchLevels = (u8*)0x203C000;


void init_dexnav_hud(u16 species, u8 environment) {

    extern void dexnav_generate_move(u16, u8, u8, u16*);
    extern enum PokemonAbility dexnav_generate_hiddenability(u16, u8);
    extern u8 dexnav_generate_potential(u8);
    extern enum Item dexnav_generate_helditem(u16, u8);
    extern u8 get_sightlvl(u8);
    extern u8 dexnav_generate_pokemonlvl(u16, u8, u8);
    extern void dexnav_proximity_update(void);
    extern void dexnav_draw_icons(void);
    extern void dexnav_icons_vision_update(u8, u8);    
    extern void dexnav_hud_manage(u8);    
    extern void dexhud_hblank(void);   

    // allocate dexnav struct
    *PokePadState = (struct DexnavHudData*)malloc_and_clear(sizeof(struct DexnavHudData));
    
    // assign non-objects to struct  
    u8 search_level = SearchLevels[species];
    (*PokePadState)->search_level = search_level;
    (*PokePadState)->species = species;;
    (*PokePadState)->environment = environment;
    (*PokePadState)->pokemon_level = dexnav_generate_pokemonlvl(species, search_level, environment);
    extern void msg_normal(pchar *);
    
    if ((*PokePadState)->pokemon_level < 1) {
        free((void*)*PokePadState);
        pchar empty_string[] = _"This Pokemon cannot be found here.\p";
        msg_normal(empty_string);        
        return;
    }

    // draw shaking grass
    extern u8 shaking_grass(u8, u8, u8, bool);
    if (!shaking_grass(environment, 12, 12, 0)) {
        free((void*)*PokePadState);
        pchar empty_string[] = _"It couldn’t be found nearby.\nTry looking in a different area!\p";
        msg_normal(empty_string);
        return;
    } 
    
    // Populate PokePadState objects
    dexnav_generate_move(species, search_level, (*PokePadState)->pokemon_level, &(*PokePadState)->move_id[0]);
    (*PokePadState)->held_item = dexnav_generate_helditem(species, search_level);
    (*PokePadState)->ability = dexnav_generate_hiddenability(species, search_level);
    (*PokePadState)->potential = dexnav_generate_potential(search_level);
    (*PokePadState)->sight_level = get_sightlvl(species);
    dexnav_proximity_update();

    // draw icons
    dexnav_draw_icons();
    
    // hide icons based on proximity and search level
    dexnav_icons_vision_update((*PokePadState)->proximity, search_level);

    // enable Hblank interrupt
    interrupts_enable(2);
    hblank_handler_set((SuperCallback)dexhud_hblank);
    
    // task update HUD
    u8 t_id = task_add((TaskCallback)dexnav_hud_manage, 0x1);
    tasks[t_id].priv[0] = objects[walkrun_state.oamid].pos1.x;    
    
}

