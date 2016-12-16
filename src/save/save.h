#ifndef SAVE_H
#define SAVE_H

#include <pokeagb/pokeagb.h>

#ifdef FLASH_SAVE_SECTORS
#undef FLASH_SAVE_SECTORS
#endif
#define FLASH_SAVE_SECTORS 16

#define OLD_SAVE_BLOCKS_SIZE (sizeof(struct SaveBlock1) + \
                              sizeof(struct SaveBlock2) + \
                              sizeof(struct SaveBlock3))

#define NEW_SAVEBLOCK_MAX_SIZE ((FLASH_SAVE_SECTORS * FLASH_SECTOR_DATA_SIZE) - OLD_SAVE_BLOCKS_SIZE)

struct SaveBlock4 {
    u32 data[NEW_SAVEBLOCK_MAX_SIZE / 4];
};

STATIC_ASSERT(sizeof(struct SaveBlock4) <= NEW_SAVEBLOCK_MAX_SIZE,
              "Additional saveblock too large.");

/* Use the pointer to the saveblock data for consistency with the
 * other blocks */
extern struct SaveBlock4 saveblock4_data;
extern struct SaveBlock4* saveblock4;

#endif /* SAVE_H */
