#include <pokeagb/pokeagb.h>
#include "pokepad.h"

static hblank_handler(void)
{
    pokepad_hblank_handler();

    if (REG_VCOUNT >= 120) {
        /* Enable darkening on BG1 and backdrop */
        REG_BLDCNT = BLDCNT_DARKEN | BLDCNT_BG1_SRC | BLDCNT_BACKDROP_SRC;
    } else {
        REG_BLDCNT = 0;
    }
}

static bool setup(u8* trigger)
{
    switch (*trigger) {
    case 0:
        /* Create the application description box using HBlank */
        hblank_handler_set(hblank_handler);
        lcd_io_set(REG_ID_BLDY, 8);
        *trigger = 1;
        break;
    }

    return false;
}

static bool destroy(u8* trigger)
{
    return false;
}

const struct PokepadApplication pokepad_application_main = {
    .name = _"PokéPad",
    .description = "Main application",
    .role = POKEPAD_APP_MAIN,
    .setup = setup,
    .destroy = destroy,
};
