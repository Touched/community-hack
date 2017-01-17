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
    { 0, 8 },
    { 1, 8 },
    { 2, 5 },
    { 3, 41 },
    { 2, 12 },
    { -1, -1 },
};

const struct TitlescreenAnimationConfig lugia_animation_config = {
    images,
    frames,
};
