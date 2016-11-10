#ifndef POKEPAD_H
#define POKEPAD_H

#include <pokeagb/pokeagb.h>

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
        u8* palette;
        u8* tiles;
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

#endif /* POKEPAD_H */
