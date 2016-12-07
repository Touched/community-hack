#include <pokeagb/pokeagb.h>
#include "starter_select.h"

const struct OamData starteroam = {
                                .y = 0,
                                .affine_mode = 0,
                                .obj_mode = 1,
                                .mosaic = 0,
                                .bpp = 0,
                                .shape = 0,
                                .x = 0,
                                .matrix_num = 0,
                                .size = 3, //64x64
                                .tile_num = 0,
                                .priority = 0,
                                .palette_num = 0,
                                .affine_param = 0
};


static const struct Frame (**nullframe)[] = (const struct Frame (**)[])0x8231CF0;
static const struct RotscaleFrame (**nullrsf)[] = (const struct RotscaleFrame (**)[])0x8231CFC;

#define STARTING_SPECIES 1

void make_species(u16 species) {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    
    struct SpritePalette starter_pal = {(void*)pokemon_palette_normal[species + STARTING_SPECIES].data, 0x600 * species};
    struct SpriteTiles starter_sprite = {(void*)pokemon_graphics_front[species + STARTING_SPECIES].data, 2048, 0x600 * species};
    struct Template starter_temp = {0x600 * species, 0x600 * species, &starteroam, nullframe, &pokemon_graphics_front[species + STARTING_SPECIES],
                                    nullrsf, (ObjectCallback)oac_nullsub};

    gpu_tile_obj_decompress_alloc_tag_and_upload(&starter_sprite);                                
    gpu_pal_decompress_alloc_tag_and_upload(&starter_pal);
    u8 objid = template_instanciate_forward_search(&starter_temp, 120, 48, 0); // middle position
    
    // hide and disable
    objects[objid].final_oam.affine_mode = 2;
    
    switch (species + STARTING_SPECIES) {
        case (STARTING_SPECIES):
            // turtwig
            (*sdp)->objid_turtwig = objid;
            break;
        case (STARTING_SPECIES + 3):
            // chimchar
            (*sdp)->objid_chimchar = objid;
            break;
        case (STARTING_SPECIES + 6):
            // piplup
            (*sdp)->objid_piplup = objid;
            break;
    };
    return;
}


void make_pokemon_pics() {
    make_species(0);
    make_species(3);
    make_species(6);
    return;
}


void display_mode_starterpic(u8 id, u8 mode) {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    // mode 0 = disable
    // mode 1 = enable
    switch (id) {
        case (0):
            // turtwig
            objects[(*sdp)->objid_turtwig].final_oam.affine_mode = mode;
            break;
        case (1):
            // chimchar
            objects[(*sdp)->objid_chimchar].final_oam.affine_mode = mode;
            break;
        case (2):
            // piplup
            objects[(*sdp)->objid_piplup].final_oam.affine_mode = mode;
            break;
    };
    return;
}

void hideall_starterpic() {
   display_mode_starterpic(0, 2);
   display_mode_starterpic(1, 2);
   display_mode_starterpic(2, 2);
   return;
}




