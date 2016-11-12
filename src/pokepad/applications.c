#include "pokepad.h"

extern const struct PokepadApplication pokepad_application_main;
extern const struct PokepadApplication pokepad_application_pokedex;

#include "generated/images/pokepad/icons/phone.h"
#include "generated/images/pokepad/icons/dexnav.h"
const pchar pokepad_application_test_desc[] = _"Phone stuff.";
const struct PokepadApplication pokepad_application_test = {
    .name = _"Phone",
    .description = pokepad_application_test_desc,
    .role = POKEPAD_APP_BUILTIN,
    .icon = POKEPAD_BUILD_APP_ICON(phone),
};

const pchar pokepad_application_test2_desc[] = _"Search for Pokémon.";
const struct PokepadApplication pokepad_application_test2 = {
    .name = _"Dexnav",
    .description = pokepad_application_test2_desc,
    .role = POKEPAD_APP_BUILTIN,
    .icon = POKEPAD_BUILD_APP_ICON(dexnav),
};

const struct PokepadApplication* pokepad_applications[] = {
    &pokepad_application_main,
    &pokepad_application_pokedex,
    &pokepad_application_test,
    &pokepad_application_test2,
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

void pokepad_applications_load(const struct PokepadApplication** buffer, u8 length)
{
    u8 buffer_index = 0;

    /* Fill array with null pointers */
    memset(buffer, 0, length * sizeof(const struct PokepadApplication*));

    for (u8 i = 0; i < POKEPAD_NUM_APPS && buffer_index < length; i++) {
        const struct PokepadApplication* app = pokepad_applications[i];

        switch (app->role) {
        case POKEPAD_APP_MAIN:
            continue;
        case POKEPAD_APP_EXTENSION:
            if (!app->unlocked()) {
                continue;
            }
            /* Fall through if unlocked */
        case POKEPAD_APP_BUILTIN:
            /* Build app */
            buffer[buffer_index++] = pokepad_applications[i];
            break;
        }
    }
}

u8 pokepad_application_id(const struct PokepadApplication** it)
{
    return it - pokepad_applications;
}
