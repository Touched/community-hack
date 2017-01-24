#include <pokeagb/pokeagb.h>
#include "time.h"

u32 title()
{
    return (((HOURS_TO_SECONDS(saveblock2->playtime_hours)) +
        (MINUTES_TO_SECONDS(saveblock2->playtime_minutes)) +
        (saveblock2->playtime_seconds)) % TIME_SCALER);
};
