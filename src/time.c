#include <pokeagb/pokeagb.h>
#include "time.h"
#define HOURS_TO_SECONDS(x) (x*3600)
#define MINUTES_TO_SECONDS(x) (x*60)

u32 title()
{
    return (((HOURS_TO_SECONDS(saveblock2->playtime_hours)) +
        (MINUTES_TO_SECONDS(saveblock2->playtime_minutes)) +
        (saveblock2->playtime_seconds)) % time_scaler);
};
