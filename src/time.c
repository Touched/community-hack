#include <pokeagb/pokeagb.h>
#include "time.h"
#define hours_to_seconds(x) (x*3600)
#define minutes_to_seconds(x) (x*60)

u32 title() {
    return (((hours_to_seconds(saveblock2->playtime_hours)) +
        (minutes_to_seconds(saveblock2->playtime_minutes)) +
        (saveblock2->playtime_seconds)) % time_scaler);
};
