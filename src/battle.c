#include <pokeagb/pokeagb.h>
#include "battle.h"
#include "partner.h"
#include "mega/mega.h"

void battle_extension_free(void)
{
    if (is_partner_battle()) {
        partner_unload_pokemon();
    }

    mega_free();
}

void battle_extension_malloc(void)
{
    mega_init();
}
