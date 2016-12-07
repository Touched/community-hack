#include <pokeagb/pokeagb.h>
#include "../../generated/images/pointer.h"
#include "starter_select.h"

const struct OamData pointeroam = {
                                .y = 0,
                                .affine_mode = 0,
                                .obj_mode = 0,
                                .mosaic = 0,
                                .bpp = 0,
                                .shape = 0,
                                .x = 0,
                                .matrix_num = 0,
                                .size = 2, //32x32
                                .tile_num = 0,
                                .priority = 1,
                                .palette_num = 0,
                                .affine_param = 0
};


static const struct Frame (**nullframe)[] = (const struct Frame (**)[])0x8231CF0;
static const struct RotscaleFrame (**nullrsf)[] = (const struct RotscaleFrame (**)[])0x8231CFC;


void pointer_bob(struct Object* obj) {
    // 30 frame delay
    if (obj->priv[0] < 20) {
        obj->priv[0]++;
        return;
    } else {
        obj->priv[0] = 0;
    }
    
    // direction invert
    obj->priv[1] = !(obj->priv[1]);   
    obj->pos1.y += (obj->priv[1]) ? 5 : -5;    
    return;
}

void make_pointer() {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    struct SpritePalette pointer_pal = {pointerPal, 0x100 * 4};
    struct SpriteTiles pointersprite = {pointerTiles, 512, 0x100 * 4};
    struct Template pointer_temp = {0x100 * 4, 0x100 * 4, &pointeroam, nullframe, &pointersprite,
                                    nullrsf, (ObjectCallback)pointer_bob};

    gpu_tile_obj_decompress_alloc_tag_and_upload(&pointersprite);                                
    gpu_pal_obj_alloc_tag_and_apply(&pointer_pal);
    (*sdp)->objid_pointer = template_instanciate_forward_search(&pointer_temp, 120, 104, 0); // middle position
    (*sdp)->pointer_pos = 1;
}


void hide_pointer() {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    objects[(*sdp)->objid_pointer].final_oam.affine_mode = 2; // disable
    return;
}

void show_pointer() {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    objects[(*sdp)->objid_pointer].final_oam.affine_mode = 0; // normal   
    return;
}

void pointer_move(u8 dir) {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    
    if ((!(*sdp)->pointer_pos) && (!dir)) {
        // on left and moving left, loop around to right most
        (*sdp)->pointer_pos = 2;
    } else if (((*sdp)->pointer_pos == 2) && (dir)) {
        // on right and moving right, loop around to left most
        (*sdp)->pointer_pos = 0;
    } else {
        // standard case
        (*sdp)->pointer_pos += (dir) ?  1 : -1;
    }
    
    struct Object* obj = &objects[(*sdp)->objid_pointer];
    switch ((*sdp)->pointer_pos) {
        case 0:
            // left
            obj->pos1.x = 60;
            obj->pos1.y = 104 - 12;
            break;
        case 1:
            // middle
            obj->pos1.x = 120;
            obj->pos1.y = 104;
            break;
        case 2:
            // right
            obj->pos1.x = 180;
            obj->pos1.y = 104 - 12;
            break;
    };
}




