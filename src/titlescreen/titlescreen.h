#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#define BG_LAYER_LOGO 0
#define BG_LAYER_LUGIA 1
#define BG_LAYER_ROCKS 2
#define BG_LAYER_SEA 3

struct TitlescreenImageConfig {
    const void* tiles;
    const void* map;
};

struct TitlescreenFrameConfig {
    s8 index;
    s8 countdown;
};

struct TitlescreenAnimationConfig {
    const struct TitlescreenImageConfig* images;
    const struct TitlescreenFrameConfig* frames;
};

struct TitlescreenResource {
    void* data;
    u32 size;
};

struct TitlescreenImage {
    struct TitlescreenResource tiles;
    struct TitlescreenResource map;
};

struct TitlescreenAnimation {
    struct TitlescreenImage* images;
    const struct TitlescreenAnimationConfig* config;
    u8 image_count;
    u8 frame_count;
    u8 frame_index;
    u8 next_frame_countdown;
};

struct TitlescreenState {
    struct {
        u8 background[0x500];
        u8 lugia[0x600];
        u8 rocks[0x500];
        u8 logo[0x500];
    } tilemaps;

    struct TitlescreenAnimation rocks;
    struct TitlescreenAnimation lugia;

    u32 counter;
};

void titlescreen_animation_init(struct TitlescreenAnimation* animation,
                                struct TitlescreenAnimationConfig* config);

void titlescreen_animation_free(struct TitlescreenAnimation* animation);

void titlescreen_animation_load_image(struct TitlescreenAnimation* animation, u8 layer, u8 index);

void tilescreen_animation_step(struct TitlescreenAnimation* animation, u8 layer);

extern const struct TitlescreenAnimationConfig lugia_animation_config;
extern const struct TitlescreenAnimationConfig rocks_animation_config;

#endif /* TITLESCREEN_H */
