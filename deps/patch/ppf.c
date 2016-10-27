#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define PPF_ENCODING_METHOD 2
#define PPF_DESCRIPTION_LENGTH 50
#define PPF_BLOCK_SIZE 1024

static int ppf_write_header(FILE* patch)
{
    char magic[] = "PPF30";
    char method = PPF_ENCODING_METHOD;
    char description[PPF_DESCRIPTION_LENGTH];

    /* Harcoded settings */
    char image_type = 0;        /* Binary */
    char block_check = 0;       /* Disabled */
    char undo = 0;              /* No Undo */
    char dummy = 0;             /* Nothing */

    memset(description, ' ', sizeof(description));

    if (fwrite(magic, 1, 5, patch) != 5) {
        return 1;
    }

    if (fwrite(&method, 1, 1, patch) != 1) {
        return 1;
    }

    if (fwrite(description, 1, sizeof(description), patch) != sizeof(description)) {
        return 1;
    }

    if (fwrite(&image_type, 1, 1, patch) != 1) {
        return 1;
    }

    if (fwrite(&block_check, 1, 1, patch) != 1) {
        return 1;
    }

    if (fwrite(&undo, 1, 1, patch) != 1) {
        return 1;
    }

    if (fwrite(&dummy, 1, 1, patch) != 1) {
        return 1;
    }

    return 0;
}

static int ppf_write_u64(FILE* patch, uint64_t value)
{
    uint8_t buf[8];
    buf[7] = (value >> 56) & 0xff;
    buf[6] = (value >> 48) & 0xff;
    buf[5] = (value >> 40) & 0xff;
    buf[4] = (value >> 32) & 0xff;
    buf[3] = (value >> 24) & 0xff;
    buf[2] = (value >> 16) & 0xff;
    buf[1] = (value >> 8) & 0xff;
    buf[0] = value & 0xff;
    return fwrite(buf, 1, sizeof(buf), patch) != sizeof(buf);
}

static int ppf_write_change(FILE* patch, uint8_t* buffer, uint16_t length, uint64_t offset)
{
    if (ppf_write_u64(patch, offset)) {
        return 1;
    }

    if (fwrite(&length, 1, 1, patch) != 1) {
        return 1;
    }

    if (fwrite(buffer, 1, length, patch) != length) {
        return 1;
    }

    return 0;
}

int generate_ppf_patch(FILE* original,
                       size_t original_size,
                       FILE* modified,
                       size_t modified_size,
                       FILE* patch)
{
    /* TODO: Copy description */
    if (ppf_write_header(patch)) {
        return 1;
    }

    int original_byte = 0, modified_byte = 0;
    uint64_t pos = 0, change_offset = 0;

    uint16_t change_length = 0;
    uint8_t buffer[255];
    bool should_create_block = false;

    while (original_byte != EOF || modified_byte != EOF) {
        original_byte = fgetc(original);
        modified_byte = fgetc(modified);

        if (original_byte != modified_byte) {
            if (!change_length) {
                change_offset = pos;
            }

            buffer[change_length++] = modified_byte;
        } else {
            if (change_length) {
                should_create_block = true;
            }
        }

        /* We only have one byte to store the patch size */
        if (change_length >= sizeof(buffer)) {
            should_create_block = true;
        }

        if (should_create_block) {
            if (ppf_write_change(patch, buffer, change_length, change_offset)) {
                return 1;
            }

            change_length = 0;
            should_create_block = false;
        }

        pos++;
    }

    if (change_length) {
        if (ppf_write_change(patch, buffer, change_length, change_offset)) {
            return 1;
        }
    }

    return 0;
}
