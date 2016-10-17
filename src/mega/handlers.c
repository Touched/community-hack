#include <pokeagb/pokeagb.h>
#include "mega.h"
#include "battle.h"

#define LAST_MESSAGE 0x181

void mega_transformation_cb(void);

const pchar* mega_get_trainer_name(void)
{
    /* TODO: Handle other banks */
    return &saveblock2->name;
}

const pchar* mega_get_keystone_name(void)
{
    enum Item item = mega_bank_find_keystone(b_active_side);
    return &items[item].name;
}

void mega_evolution_handler(void)
{
    struct MegaEvolutionState* mega = extension_state.mega_evolution;

    /* If any of the Pokemon need to Mega Evolve */
    if (mega->trigger) {
        mega->bc_continue = b_c;

        /* Find the next bank that should Mega Evolve - it's run in
         * turn order */
        enum BattleBank bank;
        do {
            bank = b_attackers_in_order[mega->bank_index++];

            /* This side requested Mega Evolution */
            if (mega->trigger & (1 << bank)) {
                break;
            }
        } while (mega->bank_index < b_num_active_sides);

        /* All Mega Evolutions have been run  */
        if (mega->bank_index >= b_num_active_sides) {
            mega->trigger = 0;
            return;
        }

        struct BattlePokemon* battler = &battle_data[bank];
        mega->entry = mega_find_for_pokemon(battler);

        /* TODO: Ignore primals */
        if (!mega->entry) {
            return;
        }

        /* Clear the Mega trigger bit */
        mega->trigger &= ~(1 << bank);

        /*
         * Setup transformation for Mega Evolution and wait for it to
         * complete.
         */
        mega->bank = bank;
        b_c = mega_transformation_cb;

        /* Display the first message */
        b_active_side = bank;
        pstrcpy(fcode_buffer2, mega_get_trainer_name());
        pstrcpy(fcode_buffer3, mega_get_keystone_name());
        b_message_held_item = battler->held_item;

        dp01_build_cmdbuf_x10(BUFFER_A, LAST_MESSAGE + 1);
        dp01_battle_side_mark_buffer_for_execution(b_active_side);
    } else {
        /*
         * Only run the attack start if we're not busy handling a mega
         * transformation
         */
        bs0_start_attack();
        mega->bank_index = 0;
    }
}

void mega_transformation_finish(void)
{
    struct MegaEvolutionState* mega = extension_state.mega_evolution;

    /* FIXME: Wait for intimidate */
    if (!b_buffers_awaiting_execution_bitfield) {
        b_c = extension_state.mega_evolution->bc_continue;
    }
}

void mega_transformation_run_abilities(void)
{
    struct MegaEvolutionState* mega = extension_state.mega_evolution;

    if (!ability_something(0, mega->bank, 0, 0, 0)) {
        b_c = mega_transformation_finish;
    }
}

void mega_transformation_wait_animation_cb(void)
{
    struct MegaEvolutionState* mega = extension_state.mega_evolution;

    /* Wait for animation */
    if (!b_buffers_awaiting_execution_bitfield) {
        /* Final message */
        b_active_side = mega->bank;
        dp01_build_cmdbuf_x10(BUFFER_A, LAST_MESSAGE + 3);
        dp01_battle_side_mark_buffer_for_execution(b_active_side);

        /* Switch-in abilities */
        b_c = mega_transformation_run_abilities;
    }
}

void mega_transformation_cb(void)
{
    struct MegaEvolutionState* mega = extension_state.mega_evolution;

    /* Wait for the message */
    if (!b_buffers_awaiting_execution_bitfield) {
        b_active_side = mega->bank;

        /* Change the data */
        mega_evolve_bank(mega->bank, mega->entry);

        /* Run an animation script */
        dp01_build_cmdbuf_x34(BUFFER_A, 2, 55);
        dp01_battle_side_mark_buffer_for_execution(b_active_side);
        b_c = mega_transformation_wait_animation_cb;
    }
}
