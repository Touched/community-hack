#include <pokeagb/pokeagb.h>
#include "../../generated/images/ball.h"
#include "starter_select.h"

const struct OamData balloam = {
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
                                .priority = 2,
                                .palette_num = 0,
                                .affine_param = 0
};

static const struct Frame (**nullframe)[] = (const struct Frame (**)[])0x8231CF0;
static const struct RotscaleFrame (**nullrsf)[] = (const struct RotscaleFrame (**)[])0x8231CFC;


void set_ball_frame(struct Object* obj, u8 frame) {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    void* src = (*sdp)->ball_tiles;
    void* dst = (void*)((obj->final_oam.tile_num * 32) + SPRITE_RAM);   
    memcpy(dst, src + (512 * frame), 512); 
}

void quick_shake(struct Object* obj) {
    extern void ball_shake(struct Object*);
    if (obj->priv[2] < 8) {
        obj->priv[2]++;
        return;
    }
    obj->priv[2] = 0;
    
    // pick frame to show
    switch (obj->priv[1]) {
        case 0:
            
            if (obj->priv[3]) {
                set_ball_frame(obj, 3);
                obj->priv[1] = 1;
            } else {
                set_ball_frame(obj, 2);            
                obj->priv[1] = 2;
            }
            break;
        case 1:
            obj->priv[3] = 0;
            obj->priv[1] = 0;
            set_ball_frame(obj, 1);
            break;
        default:
            obj->priv[3] = 1;
            obj->priv[1] = 0;
            set_ball_frame(obj, 1);
            obj->callback = (ObjectCallback)ball_shake;
            break;
    };
    
}

void ball_shake(struct Object* obj) {
    // 30 frame delay
    if (obj->priv[0] < 30) {
        obj->priv[0]++;
        return;
    } else {
        obj->priv[0] = 0;
    }
    // set callback for quick shake
    obj->callback = (ObjectCallback)quick_shake;
}


void make_balls() {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    lz77UnCompVram((void*)ballTiles, (*sdp)->ball_tiles);
    
    struct SpritePalette redballpal = {ballPal, 0x100 * 1};
    struct SpriteTiles redballsprite1 = {ballTiles, 512, 0x100 * 1};
    struct SpriteTiles redballsprite2 = {ballTiles, 512, 0x100 * 2};
    struct SpriteTiles redballsprite3 = {ballTiles, 512, 0x100 * 3};
    
    struct Template ball_temp1 = {0x100 * 1, 0x100, &balloam, nullframe, &redballsprite1,
                                    nullrsf, (ObjectCallback)ball_shake};
    struct Template ball_temp2 = {0x100 * 2, 0x100, &balloam, nullframe, &redballsprite2,
                                    nullrsf, (ObjectCallback)oac_nullsub};
    struct Template ball_temp3 = {0x100 * 3, 0x100, &balloam, nullframe, &redballsprite3,
                                    nullrsf, (ObjectCallback)oac_nullsub};  
                                    
    gpu_tile_obj_decompress_alloc_tag_and_upload(&redballsprite1);
    gpu_tile_obj_decompress_alloc_tag_and_upload(&redballsprite2);
    gpu_tile_obj_decompress_alloc_tag_and_upload(&redballsprite3);
    gpu_pal_obj_alloc_tag_and_apply(&redballpal);
  
    (*sdp)->objid_chimchar_ball = template_instanciate_forward_search(&ball_temp1, 120, 132, 0); // chimchar
    (*sdp)->objid_turtwig_ball = template_instanciate_forward_search(&ball_temp2, 60, 120, 0); // turtwig
    (*sdp)->objid_piplup_ball = template_instanciate_forward_search(&ball_temp3, 180, 120, 0); // piplup
}

void highlight_ball(u8 id) {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    
    struct Object* obj_turtwig = &objects[(*sdp)->objid_turtwig_ball];
    struct Object* obj_chimchar = &objects[(*sdp)->objid_chimchar_ball];
    struct Object* obj_piplup = &objects[(*sdp)->objid_piplup_ball];
    
    // nullsub all callbacks
    obj_turtwig->callback = oac_nullsub;
    set_ball_frame(obj_turtwig, 0);
    obj_chimchar->callback = oac_nullsub;
    set_ball_frame(obj_chimchar, 0);
    obj_piplup->callback = oac_nullsub;
    set_ball_frame(obj_piplup, 0);
    
    // pick highlighted ball
    switch (id) {
        case 0:
            // turtwig selected
            obj_turtwig->callback = ball_shake;
            obj_turtwig->priv[0] = 30;
            break;
        case 1:
            // chimchar selected
            obj_chimchar->callback = ball_shake;
            obj_chimchar->priv[0] = 30;
            break;
        case 2:
            // piplup selected
            obj_piplup->callback = ball_shake;
            obj_piplup->priv[0] = 30;
            break;
    };
    return;
}



