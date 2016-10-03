#ifndef BATTLE_H
#define BATTLE_H

#include "overworld.h"

struct BattleExtensionState {
    struct {
        u8 count;
        struct TrainerSpotted trainers[2];
    } spotted;
};

extern struct BattleExtensionState extension_state;

#endif /* BATTLE_H */
