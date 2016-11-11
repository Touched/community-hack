#include "pokepad.h"

extern const struct PokepadApplication pokepad_application_main;

#include "generated/images/pokepad/icons/pokedex.h"
const struct PokepadApplication pokepad_application_test = {
    .name = _"Pokédex",
    .description = "Test application",
    .role = POKEPAD_APP_BUILTIN,
    .icon = POKEPAD_BUILD_APP_ICON(pokedex),
};

const struct PokepadApplication* pokepad_applications[] = {
    &pokepad_application_main,
    &pokepad_application_test,
    &pokepad_application_test,
    &pokepad_application_test,
    &pokepad_application_test,
};

#define POKEPAD_NUM_APPS (sizeof(pokepad_applications) / sizeof(const struct PokepadApplication*))

const struct PokepadApplication* pokepad_application_find_main(void)
{
    for (u8 i = 0; i < POKEPAD_NUM_APPS; i++) {
        if (pokepad_applications[i]->role == POKEPAD_APP_MAIN) {
            return pokepad_applications[i];
        }
    }

    return NULL;
}

const struct PokepadApplication** pokepad_application_next(const struct PokepadApplication** start)
{
    /* Start at beginning if no app was provided, otherwise start at
     * the app after the one passed in */
    const struct PokepadApplication** it = start ? start + 1 : pokepad_applications;
    const struct PokepadApplication** end = pokepad_applications + sizeof(pokepad_applications);

    /* Invalid application pointer */
    if (it < pokepad_applications || it >= end) {
        return NULL;
    }

    for (; it < end; it++) {
        switch ((*it)->role) {
        case POKEPAD_APP_MAIN:
            continue;
        case POKEPAD_APP_EXTENSION:
            if (!(*it)->unlocked()) {
                continue;
            }
            /* Fall through if unlocked */
        case POKEPAD_APP_BUILTIN:
            return it;
        }
    }

    /* No more applications */
    return NULL;
}

u8 pokepad_application_id(const struct PokepadApplication** it)
{
    return it - pokepad_applications;
}
