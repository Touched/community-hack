#include "mega.h"
#include "../battle.h"
#include "../partner.h"
#include "../multi.h"

static void mega_register_for_bank(enum BattleBank bank, union MegaEvolutionEntry* entry)
{
    enum BattleSide side = battle_side_get_owner(bank);
    u8 party_index = b_pokemon_team_id_by_side[bank];

    struct MegaEvolution* mega = &extension_state.mega_evolution->party[side][party_index];
    mega->variant = entry->mega.variant;
    mega->revert = battle_data[bank].species;
}

void mega_init(void)
{
    extension_state.mega_evolution = malloc_and_clear(sizeof(struct MegaEvolutionState));
}

void mega_free(void)
{
    if (extension_state.mega_evolution) {
        free(extension_state.mega_evolution);
    }
}

void mega_evolve_bank(enum BattleBank bank, union MegaEvolutionEntry* entry)
{
    mega_register_for_bank(bank, entry);

    u8 party_index = b_pokemon_team_id_by_side[bank];
    struct Pokemon* party = battle_side_get_owner(bank) == SIDE_OPPONENT
        ? party_opponent : party_player;
    struct Pokemon* pokemon = &party[party_index];

    enum PokemonSpecies species = entry->mega.target;

    pokemon_setattr((struct PokemonBase*) pokemon, REQUEST_SPECIES, &species);
    recalculate_stats(pokemon);

    /* Update the battle data */
    struct BattlePokemon *battler = &battle_data[bank];
    battler->species = species;
    battler->atk = pokemon_getattr((struct PokemonBase*) pokemon, REQUEST_ATK, NULL);
    battler->def = pokemon_getattr((struct PokemonBase*) pokemon, REQUEST_DEF, NULL);
    battler->spd = pokemon_getattr((struct PokemonBase*) pokemon, REQUEST_SPD, NULL);
    battler->spatk = pokemon_getattr((struct PokemonBase*) pokemon, REQUEST_SPATK, NULL);
    battler->spdef = pokemon_getattr((struct PokemonBase*) pokemon, REQUEST_SPDEF, NULL);

    /* Mega Evolutions only have one ability */
    struct PokemonBaseStat* base = &pokemon_base_stats[species];
    battler->ability_id = base->ability[0];
    battler->type1 = base->type[0];
    battler->type2 = base->type[1];

    /* TODO: Redraw the healthbar */
}

union MegaEvolutionEntry* mega_find_for_pokemon(struct BattlePokemon* pokemon)
{
    union MegaEvolutionEntry* entry;

    for (u8 i = 0; i < EVOLUTIONS_PER_POKEMON; i++) {
        entry = (union MegaEvolutionEntry*) &evolution_table[pokemon->species][i];

        if (entry->mega.type != EVOLUTION_MEGA) {
            continue;
        }

        switch (entry->mega.variant) {
        case MEGA_TYPE_STANDARD:
        case MEGA_TYPE_PRIMAL:
            /* Check if the Pokemon is holding the required item. */
            if (pokemon->held_item == entry->mega.argument.item) {
                return entry;
            }

            break;
        case MEGA_TYPE_WISH:
            /* Check if the Pokemon knows the required move. */
            for (u8 j = 0; j < POKEMON_MOVE_SLOTS; j++) {
                if (pokemon->moves[j] == entry->mega.argument.move) {
                    return entry;
                }
            }

            break;
        default:
            /* Broken entry */
            return NULL;
        }
    }

    return NULL;
}

u8 mega_count_megas_for_side(enum BattleBank bank)
{
    /* 0 for player, 1 for opponent */
    enum BattleSide side = battle_side_get_owner(bank);

    /* Figure out which half to search through (multi/partner battles) */
    u8 lower = 0, upper = 6;
    if (side == SIDE_OPPONENT) {
        if (is_multi_battle()) {
            lower = 3 * (bank == BANK_OPPONENT_ALLY);
            upper = lower + 3;
        }
    } else {
        if (is_partner_battle()) {
            lower = 3 * (bank == BANK_PLAYER_ALLY);
            upper = lower + 3;
        }
    }

    /* Count the megas */
    /* TODO: Exclude primals */
    u8 count = 0;;
    for (u8 i = lower; i < upper; i++) {
        if (extension_state.mega_evolution->party[side][i].variant) {
            count++;
        }
    }

    return count;
}
