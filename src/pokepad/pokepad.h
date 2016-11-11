#ifndef POKEPAD_H
#define POKEPAD_H

#include <pokeagb/pokeagb.h>

#define POKEPAD_BUILD_APP_ICON(name) \
    { .palette = name ## Pal, .tiles = name ## Tiles, .size = name ## TilesLen }

struct PokepadApplication {
    /**
     * The name of the application.
     */
    const pchar name[10];

    /**
     * A description of the application that displays in the main menu.
     */
    const pchar* description;

    /**
     * The application's icon sprite.
     */
    struct Icon {
        const u8* palette;
        const u8* tiles;
        u16 size;
    } icon;

    /**
     * The role of the application
     */
    enum Role {
        POKEPAD_APP_MAIN,
        POKEPAD_APP_BUILTIN,
        POKEPAD_APP_EXTENSION,
    } role;

    /**
     * Called if the role is POKEPAD_APP_EXTENSION. If it returns
     * true, then the application is enabled and displayed.
     */
    bool (*unlocked)(void);

    /**
     * Called when the application is loaded. Returns true while busy.
     */
    bool (*setup)(u8*);

    /**
     * Called when the application is unloaded. Returns true while busy.
     */
    bool (*destroy)(u8*);
};

struct PokepadShared {
    u8 bar_textboxes[2];
    u16 gradient_palette[20][16] __attribute__((aligned(4))); /* 20 lines of 16 colours */
};

struct Pokepad {
    const struct PokepadApplication* current_app;
    void* app_state;
    struct PokepadShared* shared_state;
    u8 tracker;
};

extern struct Pokepad* pokepad_state;

extern const struct PokepadApplication* pokepad_applications[];

const struct PokepadApplication* pokepad_application_find_main(void);
const struct PokepadApplication** pokepad_application_next(const struct PokepadApplication** it);
u8 pokepad_application_id(const struct PokepadApplication** it);

/* Handlers */
void pokepad_hblank_handler(void);
void pokepad_vblank_handler(void);

#endif /* POKEPAD_H */
