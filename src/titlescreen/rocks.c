#include <pokeagb/pokeagb.h>
#include "titlescreen.h"
#include "generated/images/titlescreen/rocks1.h"
#include "generated/images/titlescreen/rocks2.h"
#include "generated/images/titlescreen/rocks3.h"

static const struct TitlescreenImageConfig images[] = {
    { rocks1Tiles, rocks1Map },
    { rocks2Tiles, rocks2Map },
    { rocks3Tiles, rocks3Map },
    { NULL, NULL },
};


static const struct TitlescreenFrameConfig frames[] = {
    { 0, 8 },
    { 1, 8 },
    { 2, 8 },
    { -1, -1 },
};

const struct TitlescreenAnimationConfig rocks_animation_config = {
    images,
    frames,
};
