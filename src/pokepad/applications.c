#include "pokepad.h"

extern const struct PokepadApplication pokepad_application_main;

const struct PokepadApplication* pokepad_applications[] = {
    &pokepad_application_main,
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
