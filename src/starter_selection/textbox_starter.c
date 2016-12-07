#include <pokeagb/pokeagb.h>
#include "starter_select.h"
#include "../../generated/images/arrowhead.h"



void starter_textbox(void) {
    if ((REG_VCOUNT > 118)) {
        //WRITE_REG_WININ(0x3F3F);
        REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_BLD,  WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ | WIN_BLD);
                        
        //WRITE_REG_BLDCNT(0x0FFE);
        REG_BLDCNT = (BLDCNT_BG1_SRC | BLDCNT_BG2_SRC | BLDCNT_BG3_SRC | BLDCNT_SPRITES_SRC | BLDCNT_BACKDROP_SRC | BLDCNT_BG0_DST | BLDCNT_BG1_DST | BLDCNT_BG2_DST | BLDCNT_BG3_DST | BLDCNT_DARKEN);
        REG_BLDY = 0xC;
        return;

    } else {    
        // WRITE_REG_WININ(0x1F1F);
        REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ, WIN_BG0 |
                        WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
        //WRITE_REG_BLDCNT(0x401E);
        REG_BLDCNT = BLDALPHA_BUILD(BLDCNT_BG1_SRC | BLDCNT_BG2_SRC | BLDCNT_BG3_SRC | BLDCNT_SPRITES_SRC, 0);
    }
}


void update_message(u8 id) {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    u8 r_id = (*sdp)->rboxid;
    
    pchar turtwig_text [] = _"{HIGHLIGHT TRANSPARENT}{COLOR 1}{SHADOW 2}Do you want this useless bush-looking\ntwig-looking Turtwig thing?";
    pchar chimchar_text [] = _"{HIGHLIGHT TRANSPARENT}{COLOR 1}{SHADOW 2}Do you want this naked ape-money,\nLord Chimchamus Chimchar?";
    pchar piplip_text [] = _"{HIGHLIGHT TRANSPARENT}{COLOR 1}{SHADOW 2}Do you want this penguin?";
    
    rboxid_clear_pixels(r_id, 0x0); 
    switch(id) {
        case 0:
            rboxid_draw_text(r_id, 2, (pchar *)turtwig_text, 5, 1, 0xFF, 0);
            break;
        case 1:
            rboxid_draw_text(r_id, 2, (pchar *)chimchar_text, 5, 1, 0xFF, 0);
            break;
        case 2:
            rboxid_draw_text(r_id, 2, (pchar *)piplip_text, 5, 1, 0xFF, 0);
            break;
    };
    rboxid_update(r_id, 3);
    rboxid_tilemap_update(r_id);
    gpu_sync_bg_show(0);
    
    // enable Hblank interrupt
    interrupts_enable(2);
    hblank_handler_set((SuperCallback)starter_textbox);
}

void hide_messagebox() {
    gpu_sync_bg_hide(0);
    super.hblank_handler = NULL;
    REG_WININ = WININ_BUILD(WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ, WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ);
    REG_BLDCNT = BLDALPHA_BUILD(BLDCNT_BG1_SRC | BLDCNT_BG2_SRC | BLDCNT_BG3_SRC | BLDCNT_SPRITES_SRC, 0);
    return;
}


void messagebox_init() {
	struct Textbox this_box = {0x0, 0x2, 0xF, 0x1E, 0x4, 0xF, 132, (u8*)0x0};
	u8 r_id = rboxid_init(&this_box);   
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    (*sdp)->rboxid = r_id;
    update_message(0);
    return;
}


void update_indicator(u8 dir) {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);

    // if on edge, loop around
    
    u8* t = (u8*)(0x203C030);
    *t = (*sdp)->yesno_cursor_pos;
    *(t+2) = dir;
    
    if (dir && (*sdp)->yesno_cursor_pos) {
        (*sdp)->yesno_cursor_pos = 0;
    } else if ((!(*sdp)->yesno_cursor_pos) && (!dir)) {
        (*sdp)->yesno_cursor_pos = 1;
    } else {
        (*sdp)->yesno_cursor_pos += (dir) ? 1 : -1;
    }
    
    objects[(*sdp)->yesno_cursor_id].pos1.y = (60 + ( (*sdp)->yesno_cursor_pos * 16));
    return;
}

void hide_indicator() {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    objects[(*sdp)->yesno_cursor_id].final_oam.affine_mode = 2;
    return;
}


void show_indicator() {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    objects[(*sdp)->yesno_cursor_id].final_oam.affine_mode = 0;
    return;
}

const struct OamData text_indicator_oam = {
                                .y = 0,
                                .affine_mode = 0,
                                .obj_mode = 0,
                                .mosaic = 0,
                                .bpp = 0,
                                .shape = 0,
                                .x = 0,
                                .matrix_num = 0,
                                .size = 1, //16x16
                                .tile_num = 0,
                                .priority = 0,
                                .palette_num = 0,
                                .affine_param = 0
};

static const struct Frame (**nullframe)[] = (const struct Frame (**)[])0x8231CF0;
static const struct RotscaleFrame (**nullrsf)[] = (const struct RotscaleFrame (**)[])0x8231CFC;

void init_indicator() {
    struct starter_select_data** sdp = (struct starter_select_data**)(0x20370B8);
    
    struct SpritePalette indicator_pal = {(void*)arrowheadPal, 0x1100};
    struct SpriteTiles indicator_sprite = {(void*)arrowheadTiles, 128, 0x1100};
    
    struct Template indicator_temp = {0x1100, 0x1100, &text_indicator_oam, nullframe, 
                                    &indicator_sprite,nullrsf, (ObjectCallback)oac_nullsub};

    gpu_tile_obj_alloc_tag_and_upload(&indicator_sprite);                                
    gpu_pal_obj_alloc_tag_and_apply(&indicator_pal);
    u8 objid = template_instanciate_forward_search(&indicator_temp, 195, 60, 0);
    (*sdp)->yesno_cursor_id = objid;
    (*sdp)->yesno_cursor_pos = 0;
    return;
}

    
    
    


