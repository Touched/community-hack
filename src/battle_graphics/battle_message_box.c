#include <pokeagb/pokeagb.h>
#include "generated/images/battle_message_box.h"

void battle_message_box_load_palette(void)
{
    u16 base_pal[32];
    memcpy(base_pal, battle_message_boxPal, sizeof(base_pal));

    /* Figure out the colours to change */
    u16 primary, secondary;
    switch (get_bs_elem_env_index()) {
    case BATTLE_TERRAIN_GRASS:
        primary = 0x3B2B;
        secondary = 0x2AA7;
        break;
    default:
        primary = 0x1084;
        secondary = 0x0C63;
        break;
    }


    base_pal[13] = secondary;
    base_pal[14] = primary;
    gpu_pal_apply((u8*) base_pal, 0, 64);
}
