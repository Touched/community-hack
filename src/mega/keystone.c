#include <pokeagb/pokeagb.h>
#include "../partner.h"
#include "../multi.h"
#include "../config.h"

static enum Item keystone_table[] = {
    ITEM_MASTERBALL,
};

static bool mega_is_item_keystone(enum Item item)
{
    for (u8 i = 0; i < sizeof(keystone_table) / sizeof(enum Item); i++) {
        if (item == keystone_table[i]) {
            return true;
        }
    }

    return false;
}

static bool mega_trainer_has_keystone(u16 trainer_id)
{
    for (u8 i = 0; i < TRAINER_ITEM_COUNT; i++) {
        if (mega_is_item_keystone(trainer_data[trainer_id].items[i])) {
            return true;
        }
    }

    return false;
}

static bool mega_partner_has_keystone(u16 partner_id)
{
    for (u8 i = 0; i < sizeof(keystone_table) / sizeof(enum Item); i++) {
        if (partner_count_item(keystone_table[i]) > 0) {
            return true;
        }
    }

    return false;
}

static bool mega_player_has_keystone(void)
{
    for (u8 i = 0; i < sizeof(keystone_table) / sizeof(enum Item); i++) {
        if (check_item(keystone_table[i], 1)) {
            return true;
        }
    }

    return false;
}

bool mega_bank_has_keystone(enum BattleBank bank)
{
    if (battle_side_get_owner(bank) == SIDE_OPPONENT) {
        if (!is_multi_battle() || bank == BANK_OPPONENT) {
            return mega_trainer_has_keystone(trainerbattle_flag_id);
        } else {
            return mega_trainer_has_keystone(multi_second_opponent_id());
        }
    } else {
        if (!is_partner_battle() || bank == BANK_PLAYER) {
            return mega_player_has_keystone();
        } else {
            return mega_partner_has_keystone(var_load(BATTLE_PARTNER_ID_VAR));
        }
    }
}
