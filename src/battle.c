#include <pokeagb/pokeagb.h>
#include "battle.h"
#include "partner.h"

void battle_extension_free(void)
{
    if (is_partner_battle()) {
        partner_unload_pokemon();
    }
}
