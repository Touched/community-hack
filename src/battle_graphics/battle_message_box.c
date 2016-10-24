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
    case BATTLE_TERRAIN_LONG_GRASS:
        primary = 0x3B2B;
        secondary = 0x2AA7;
        break;
    case BATTLE_TERRAIN_OCEAN:
        primary = 0x670a;
        secondary = 0x5626;
        break;
    case BATTLE_TERRAIN_DESERT:
    case BATTLE_TERRAIN_MOUNTAIN:
    case BATTLE_TERRAIN_CAVE:
        primary = 0x56d8;
        secondary = 0x3a35;
        break;
    case BATTLE_TERRAIN_TRAINER_GYM:
        primary = 0x735b;
        secondary = 0x5a75;
        break;
    default:
        primary = 0x6f59;
        secondary = 0x5a93;
        break;
    }

    /* Border */
    base_pal[13] = secondary;
    base_pal[14] = primary;

    /* Arrow */
    base_pal[2] = primary;
    gpu_pal_apply((u8*) base_pal, 0, 64);
}
