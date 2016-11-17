#include <pokeagb/pokeagb.h>

u8 get_ability(u16 species, u8 ability_bit) {
    switch (ability_bit) {
        case 0:
            {
            // first
            ability = pokemon_base_stats[species].ability[0];
            break;
            }
        case 1:
            {
            // second
            ability = pokemon_base_stats[species].ability[1];
            break;
            }
        default:
            {
            // hidden
            ability = pokemon_base_stats[species].padding & 0xFF;
            break;
            }
    };
    
    return ability;
}


