#include <pokeagb/pokeagb.h>
#include "config.h"
#include "overworld.h"

/*
 * First half of the battle init script. This handles the trainer
 * walking up to the player, playing the encounter music and
 * displaying their battle intro message. We need to do this once for
 * each opponent in the multi battle so we only run this portion.
 */
u8 multi_battle_script_fragment[] = {
    0x6a,                       /* lock */
    0x25, 0x38, 0,              /* special 0x38 */
    0x25, 0x37, 0,              /* special 0x37 */
    0x27,                       /* waitstate */
    0x25, 0x34, 0,              /* special 0x34 */
    0x66,                       /* watimsg */
    0x6d,                       /* waitkeypress */
    0x2                         /* end */
};

void overworld_trainer_battle_flags(void)
{
   battle_type_flags = BATTLE_FLAG_TRAINER;

    if (trainers_spotted.count == 2) {
        battle_type_flags |= BATTLE_FLAG_MULTI | BATTLE_FLAG_DOUBLE;
    }
}

/*
 * Configure the battle for the given trainer and make them walk up to
 * the player.
 */
void overworld_multi_run_script(u8 spotted_trainer_id)
{
    struct TrainerSpotted* trainer = &trainers_spotted.trainers[spotted_trainer_id];
    struct NpcState* npc = &npc_states[trainer->id];

    scripting_npc = trainer->id;
    var_800F = npc->local_id;
    battle_configure_by_script(trainer->script + 1);
    script_env_init_script(multi_battle_script_fragment);
    script_env_enable();
    add_task_trainer_walk(npc, trainer->distance - 1);
}

void mutli_set_second_trainer(u16 trainer_id)
{
    *var_access(MULTI_SECOND_OPPONENT_TRAINER_ID) = trainer_id;
}

/*
 * Handles the sequencing of the multi battle encounter.
 */
void task_overworld_mutli_trainers(u8 id)
{
    struct Task* task = &tasks[id];

    switch (task->priv[0]) {
    case 0:
        overworld_multi_run_script(0);
        task->priv[0]++;
        mutli_set_second_trainer(trainerbattle_flag_id);
        break;

    case 1:
        if (!script_env_2.enabled) {
            task->priv[0]++;
        }
        break;

    case 2:
        overworld_multi_run_script(1);
        task->priv[0]++;
        break;

    case 3:
        if (!script_env_2.enabled) {
            task->priv[0]++;
        }
        break;

    case 4:
        /*
         * Jump halfway into the battle init script to actually start
         * the battle.
         */
        script_env_init_script(0x081A4FCC);
        script_env_enable();
        task_del(id);
        break;
    }
}

u8 overworld_handle_trainer_spot(void)
{
    /*
     * We're gonna leave out the previously on your quest related
     * stuff because it's useless.
     */

    trainers_spotted.count = 0;
    for (u8 i = 0; i < NPC_STATE_ID_MAX; i++) {
        struct NpcState* npc = &npc_states[i];

        /* TODO: Find out what this bit means */
        if (npc->bitfield << 31) {
            if (npc->is_trainer == 1 || npc->is_trainer == 3) {
                if (check_if_npc_spotted_player(i)) {
                    if (player_unable_to_double_battle()) {
                        break;
                    }

                    /* We've found enough opponents */
                    if (trainers_spotted.count > 1) {
                        break;
                    }
                }
            }
        }
    }

    if (trainers_spotted.count == 1) {
        /* TODO: reset state data */
        struct TrainerSpotted* trainer = &trainers_spotted.trainers[0];
        struct NpcState* npc = &npc_states[trainer->id];

        scripting_npc = trainer->id;
        var_800F = npc->local_id;
        battle_configure_by_script(trainer->script + 1);
        script_env_init_script(0x081A4EB4);
        script_env_enable();

        add_task_trainer_walk(npc, trainer->distance - 1);

        return 1;
    } else if (trainers_spotted.count == 2) {
        /* TODO: reset state data */
        if (!task_is_running(task_overworld_mutli_trainers)) {
            task_add(task_overworld_mutli_trainers, 0x50);
        }

        return 1;
    }

    return 0;
}

void overworld_register_trainer(u8 npc_id, void* script, u8 distance)
{
    struct TrainerSpotted trainer = { npc_id, distance, script };
    trainers_spotted.trainers[trainers_spotted.count++] = trainer;
}
