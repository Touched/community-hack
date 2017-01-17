#include <pokeagb/pokeagb.h>
#include "titlescreen.h"
#include "generated/images/titlescreen/lugia1.h"
#include "generated/images/titlescreen/lugia2.h"
#include "generated/images/titlescreen/lugia3.h"
#include "generated/images/titlescreen/lugia4.h"

#define SCALE 4

static const struct TitlescreenImageConfig images[] = {
    { lugia1Tiles, lugia1Map },
    { lugia2Tiles, lugia2Map },
    { lugia3Tiles, lugia3Map },
    { lugia4Tiles, lugia4Map },
    { NULL, NULL },
};


static const struct TitlescreenFrameConfig frames[] = {
    /* { 0, 2 * SCALE }, */
    /* { 1, 2 * SCALE }, */
    /* { 2, 1 * SCALE }, */
    /* { 3, 10 * SCALE }, */
    /* { 2, 3 * SCALE }, */
    { 0, 7 },
    { 1, 7 },
    { 2, 3 },
    { 3, 33 },
    { 2, 10 },
    { -1, -1 },
};

const struct TitlescreenAnimationConfig lugia_animation_config = {
    images,
    frames,
};
