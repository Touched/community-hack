#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

off_t fsize(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0) {
        return st.st_size;
    }

    return -1;
}

int generate_ups_patch(FILE* original,
                       size_t original_size,
                       FILE* modified,
                       size_t modified_size,
                       FILE* patch);

int generate_ppf_patch(FILE* original,
                       size_t original_size,
                       FILE* modified,
                       size_t modified_size,
                       FILE* patch);

int main(int argc, char *argv[])
{
    if (argc != 5) {
        printf("Usage: [ups|ppf] patch original modified patch\n");
        return 1;
    }

    const char* patch_type = argv[1];
    const char* original_fname = argv[2];
    const char* modified_fname = argv[3];
    const char* patch_fname = argv[4];

    FILE* original = fopen(original_fname, "rb");
    if (!original) {
        perror("Could not open original file");
        return 1;
    }

    off_t original_size = fsize(original_fname);

    FILE* modified = fopen(modified_fname, "rb");
    if (!modified) {
        perror("Could not open modified file");
        fclose(original);
        return 1;
    }

    off_t modified_size = fsize(modified_fname);

    FILE* patch = fopen(patch_fname, "wb+");
    if (!patch) {
        perror("Could not open output file");
        fclose(original);
        fclose(modified);
        return 1;
    }

    if (!strcmp("ups", patch_type)) {
        if (generate_ups_patch(original, original_size, modified, modified_size, patch)) {
            perror("Failed to generate UPS patch\n");
            fclose(original);
            fclose(modified);
            fclose(patch);
            return 1;
        }
    } else if (!strcmp("ppf", patch_type)) {
        if (generate_ppf_patch(original, original_size, modified, modified_size, patch)) {
            perror("Failed to generate PPF patch\n");
            fclose(original);
            fclose(modified);
            fclose(patch);
            return 1;
        }
    } else {
        fprintf(stderr, "Invalid patch type: %s\n", patch_type);
        return 1;
    }

    fclose(original);
    fclose(modified);
    if (fclose(patch)) {
        perror("Could not close output file");
        return 1;
    }

    return 0;
}
