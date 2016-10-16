#ifndef MEGA_H
#define MEGA_H

#include <pokeagb/pokeagb.h>

enum MegaVariant {
    /**
     * No mega evolution.
     */
    MEGA_TYPE_NONE,

    /**
     * Standard Mega Evolution.
     */
    MEGA_TYPE_STANDARD,

    /**
     * Primal Reversion.
     */
    MEGA_TYPE_PRIMAL,

    /**
     * Move-based Mega Evolution.
     */
    MEGA_TYPE_WISH,
};

struct MegaEvolution {
    /**
     * Type of mega evolution
     */
    enum MegaVariant variant;

    /**
     * The species to revert to after battle.
     */
    enum PokemonSpecies revert;
};

struct MegaEvolutionUI {
    /**
     * The page on the fight menu.
     */
    u16 page;

    bool loaded;

    /**
     * Task ID for the scroll arrows.
     */
    u8 arrow_task;

    /**
     * Object IDs for the indicators that show if a Pokemon is a
     * mega/primal or not. They show up on the healthbar.
     */
    u8 indicators[BATTLE_SIDES];

    /**
     * Keep track of whether the player has requested Mega Evolution.
     */
    bool activated[BATTLE_SIDES];
};

struct MegaEvolutionState {
    /**
     * Set if the bank has requested a mega evolution.
     */
    bool trigger[BATTLE_SIDES];

    void (*bc_continue)(void);
    enum BattleBank bank;
    union MegaEvolutionEntry* entry;

    /**
     * Mega evolution state for the entire party (both sides).
     */
    struct MegaEvolution party[2][POKEMON_PARTY_SIZE];

    /**
     * User Interface.
     */
    struct MegaEvolutionUI ui;
};

/**
 * Augmented evolution table.
 */
union MegaEvolutionEntry {
    struct {
        enum EvolutionType type;
        enum MegaVariant variant;
        union {
            enum Item item;
            enum Move move;
        } argument;
        enum PokemonSpecies target;
        u16 padding;
    } mega;
    struct EvolutionEntry normal;
};

/* Ensure the agumented entry has the correct layout */
ASSERT_SIZEOF(((union MegaEvolutionEntry*) NULL)->mega, 8);
ASSERT_OFFSETOF(union MegaEvolutionEntry, mega.type, 0);
ASSERT_OFFSETOF(union MegaEvolutionEntry, mega.variant, 1);
ASSERT_OFFSETOF(union MegaEvolutionEntry, mega.argument, 2);
ASSERT_OFFSETOF(union MegaEvolutionEntry, mega.target, 4);

/**
 * Initialize the Mega Evolution state structure.
 */
void mega_init(void);

/**
 * Deallocate the Mega Evolution state structure.
 */
void mega_free(void);

/**
 * Cause a given bank to Mega evolve using the data from entry. No
 * checks are performed.
 */
void mega_evolve_bank(enum BattleBank bank, union MegaEvolutionEntry* entry);

/**
 * Find the associated Mega Evolution entry for this Pokemon. Returns
 * NULL if this Pokemon does not meet the requirements to Mega Evolve.
 */
union MegaEvolutionEntry* mega_find_for_pokemon(struct BattlePokemon* pokemon);

/**
 * Count the Mega evolutions for the given bank's side. Counts allies
 * as separate entities in multi battles.
 */
u8 mega_count_megas_for_side(enum BattleBank bank);

/**
 * Search the trainer's inventory for a keystone and return its index
 * if found.
 */
enum Item mega_bank_find_keystone(enum BattleBank bank);

/* Custom evolution types */
#define EVOLUTION_MEGA 0xFF

#endif /* MEGA_H */
