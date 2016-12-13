#include "save.h"
#include "util/debug.h"

static void* copy(void* dst, void* src, u32 size)
{
    return memcpy(dst, src, size);
}

static void* copy_inverse(void* src, void* dst, u32 size)
{
    return memcpy(dst, src, size);
}

static void saveblock_copy(struct SaveSector* buffer, u8 sector, void* (*copy)(void*, void*, u32))
{
    u32 sector_start = FLASH_SECTOR_DATA_SIZE * sector;
    u32 sector_end = sector_start + FLASH_SECTOR_DATA_SIZE;

    /* The old saveblocks are contiguous in memory so treat them as a
     * single block. Order: Saveblock 2, 1, 3, so put saveblock2 first */
    void* old_saveblocks = saveblock2;
    void* new_saveblock = saveblock4;

    if (sector_end <= OLD_SAVE_BLOCKS_SIZE) {
        /* The offset is in one of the old saveblocks */
        u32 size = MIN(OLD_SAVE_BLOCKS_SIZE - sector_start, FLASH_SECTOR_DATA_SIZE);
        copy(buffer->data, old_saveblocks + sector_start, size);
    } else if (sector_start >= OLD_SAVE_BLOCKS_SIZE) {
        /* The offset is in one of the new saveblocks */
        u32 offset = sector_start - OLD_SAVE_BLOCKS_SIZE;

        /* Do bounds checking */
        if (offset < sizeof(struct SaveBlock4)) {
            u32 size = MIN(FLASH_SECTOR_DATA_SIZE, sizeof(struct SaveBlock4) - offset);

            if (size) {
                copy(buffer->data, new_saveblock + offset, size);
            }
        }
    } else {
        /* The offset overlaps both the old and the new saveblocks */
        u32 old_size = OLD_SAVE_BLOCKS_SIZE - sector_start;
        copy(buffer->data, old_saveblocks + sector_start, old_size);

        u32 new_size = sector_end - OLD_SAVE_BLOCKS_SIZE;
        new_size = MIN(new_size, sizeof(struct SaveBlock4));
        copy(buffer->data + old_size, new_saveblock, new_size);
    }
}

/* 080D9E54 */
int saveblock_read_from_flash(void)
{
    u16 sector_offset = FLASH_SAVE_SECTORS * (save_count % 2);

    for (u16 i = 0; i < FLASH_SAVE_SECTORS; i++) {
        memset(flash_sector_buffer, 0, sizeof(struct SaveSector));
        flash_load_sector(sector_offset + i, flash_sector_buffer);

        u16 checksum = save_calculate_checksum(flash_sector_buffer->data, FLASH_SECTOR_DATA_SIZE);
        if (flash_sector_buffer->meta.magic == FLASH_SECTOR_MAGIC
            && flash_sector_buffer->meta.checksum == checksum) {

            saveblock_copy(flash_sector_buffer, i, copy_inverse);
        }
    }

    return 1;
}

/* 080D9870 */
int saveblock_write_to_flash(u16 sector_index)
{
    /* There is a backup save, so swap between two portions of the chip every save */
    /* This also maximizes the lifespan of the chip */
    u16 sector_offset = FLASH_SAVE_SECTORS * (save_count % 2);

    memset(flash_sector_buffer, 0, sizeof(struct SaveSector));

    flash_sector_buffer->meta.id = sector_index;
    flash_sector_buffer->meta.magic = FLASH_SECTOR_MAGIC;
    flash_sector_buffer->meta.counter = save_count;

    saveblock_copy(flash_sector_buffer, sector_index, copy);
    flash_sector_buffer->meta.checksum = save_calculate_checksum(flash_sector_buffer->data,
                                                                 FLASH_SECTOR_DATA_SIZE);

    return flash_write_sector(sector_offset + sector_index, flash_sector_buffer);
}

/* TODO: Fix 080DA248 (save_game) and 080DA4FC (load_game) */
