#ifndef POKEPAD_H
#define POKEPAD_H

#include <pokeagb/pokeagb.h>

#define POKEPAD_MAX_APPS 4

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
     * Called when the application is loaded.
     */
    void (*init)(void);

    /**
     * Called when the application is unloaded.
     */
    void (*deinit)(void);
};

struct Pokepad {
    u8 current_app;
    void* app_state;
};

extern struct Pokepad* pokepad_state;

extern const struct PokepadApplication* pokepad_applications[POKEPAD_MAX_APPS];

#endif /* POKEPAD_H */
