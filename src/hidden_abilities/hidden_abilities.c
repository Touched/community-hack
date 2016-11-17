#include <pokeagb/pokeagb.h>
#include "../pokemon_bst.h"

u8 get_ability(u16 species, u8 ability_index) {
    u8 ability = 0;
    struct PokemonBaseStatNew* bst = (struct PokemonBaseStatNew*)&pokemon_base_stats[species];
    switch (ability_index) {
        case 0:
            {
            // first
            ability = bst->ability[0];
            break;
            }
        case 1:
            {
            // second
            ability = bst->ability[1];
            break;
            }
        default:
            {
            // hidden
            ability = bst->hidden_ability;
            break;
            }
    };
    
    return ability;
}


