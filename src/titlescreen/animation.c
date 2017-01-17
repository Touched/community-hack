#include <pokeagb/pokeagb.h>
#include "titlescreen.h"
#include "util/debug.h"

void titlescreen_animation_init(struct TitlescreenAnimation* animation,
                                struct TitlescreenAnimationConfig* config)
{
    assert(animation);
    assert(config);

    /* Count the number of frames */
    u8 images = 0;
    for (struct TitlescreenImageConfig* image = config->images; image->tiles; image++) {
        images++;
    }

    assert(images > 0);

    u8 frames = 0;
    for (struct TitlescreenFrameConfig* frame = config->frames; frame->index >= 0; frame++) {
        frames++;
    }

    assert(frames > 0);

    animation->image_count = images;
    animation->frame_count = frames;
    animation->frame_index = 0;
    animation->config = config;
    animation->next_frame_countdown = config->frames[0].countdown;

    animation->images = malloc(sizeof(struct TitlescreenImage) * frames);

    assert(animation->images);

    /* Load image data */
    for (u8 i = 0; i < images; i++) {
        struct TitlescreenImage* image = &animation->images[i];
        struct TitlescreenImageConfig* image_config = &config->images[i];

        image->tiles.data = malloc_and_LZ77UnComp(image_config->tiles, &image->tiles.size);
        image->map.data = malloc_and_LZ77UnComp(image_config->map, &image->map.size);
    }
}

void titlescreen_animation_free(struct TitlescreenAnimation* animation)
{
    assert(animation);

    for (u8 i = 0; i < animation->image_count; i++) {
        struct TitlescreenImage* image = &animation->images[i];
        free(image->tiles.data);
        free(image->map.data);
    }

    free(animation->images);
}


void titlescreen_animation_load_image(struct TitlescreenAnimation* animation, u8 layer, u8 index)
{
    assert(animation);
    assert(index < animation->frame_count);

    struct TitlescreenImage* image = &animation->images[index];

    gpu_copy_to_tileset(layer, image->tiles.data, image->tiles.size, 0);
    gpu_copy_tilemap(layer, image->map.data, image->map.size, 0);
    bgid_send_tilemap(layer);
}

void tilescreen_animation_step(struct TitlescreenAnimation* animation, u8 layer)
{
    assert(animation);

    if (animation->next_frame_countdown) {
        animation->next_frame_countdown--;
    } else {
        animation->frame_index++;

        if (animation->frame_index >= animation->frame_count) {
            animation->frame_index = 0;
        }

        u8 index = animation->frame_index;
        struct TitlescreenFrameConfig* frame = &animation->config->frames[index];

        assert(frame->index < animation->image_count);

        animation->next_frame_countdown = frame->countdown;
        titlescreen_animation_load_image(animation, layer, frame->index);
    }
}
