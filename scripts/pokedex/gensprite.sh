#!/bin/bash

BUILD=$1
ID=$2
INPUT=$3
OUTPUT=$4

function cropgrit_multi {
    PNG=$1
    SYMBOL_BASE=$2
    shift 2
    IMAGES=$@

    WORKDIR=$(mktemp -p $BUILD -d)
    CROPPED=()

    for image in $IMAGES; do
        DIMENSIONS=${image%,*}; NAME=${image#*,};
        TEMPFILE=$WORKDIR/${SYMBOL_BASE}_${NAME}.png

        # Force an invalid crop to fail
        if convert $PNG -regard-warnings -quiet -crop $DIMENSIONS $TEMPFILE; then
            CROPPED+=(${SYMBOL_BASE}_${NAME}.png)
        fi
    done

    # Grit dumps non-shared assets into the current directory, so
    # start a subshell in the working directory before running it.
    (cd $WORKDIR &&
         grit ${CROPPED[@]} -gB4 -gu8 -gzl -pS -pzl -fa -ftc -fh! \
              -O${SYMBOL_BASE}_sprite_palette.c -S${SYMBOL_BASE}_sprite_normal)

    # Create the shiny palette separately
    convert $PNG -crop 64x64+64+0 $WORKDIR/${SYMBOL_BASE}_sprite_shiny.png
    grit $WORKDIR/${SYMBOL_BASE}_sprite_shiny.png -gB4 -g! -pzl -ftc -fh! \
         -o$WORKDIR/${SYMBOL_BASE}_sprite_shiny.c

    cat $(ls $WORKDIR/*.c)

    rm -rf $WORKDIR
}

NORMAL_SPRITES=(
    '64x64+0+0,sprite_front'
    '64x64+128+0,sprite_back'
    '64x64+0+64,sprite_female_front'
    '64x64+128+64,sprite_female_back'
)

# The palette is shared between the front and back sprites
cropgrit_multi $INPUT $ID ${NORMAL_SPRITES[@]} > $OUTPUT
