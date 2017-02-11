#!/bin/bash

BUILD=$1
ID=$2
INPUT=$3
OUTPUT=$4

function temp {
    mktemp -p $BUILD --suffix=$1
}

function cropgrit {
    PNG=$1
    SYMBOL=$2
    DIMENSIONS=$3
    TEMP_C=$(temp ".c")
    TEMP_PNG=$(temp ".png")
    shift 3

    # Force an invalid crop to fail
    if convert $PNG -regard-warnings -quiet -crop $DIMENSIONS $TEMP_PNG; then
        grit $TEMP_PNG -s$SYMBOL -o $TEMP_C -fh! $@
        cat $TEMP_C
    fi

    rm -f $TEMP_C
    rm -f $TEMP_PNG
}

# Concatenate resources into output file
cropgrit $INPUT ${ID}_sprite_front 64x64+0+0 -p! -gzl -gB4 -gu8 > $OUTPUT
cropgrit $INPUT ${ID}_sprite_back 64x64+128+0 -p! -gzl -gB4 -gu8 >> $OUTPUT
cropgrit $INPUT ${ID}_sprite_normal 64x64+0+0 -g! -gB4 -pzl -pn 16 >> $OUTPUT
cropgrit $INPUT ${ID}_sprite_shiny 64x64+64+0 -g! -gB5 -pzl -pn 16 >> $OUTPUT
cropgrit $INPUT ${ID}_sprite_female_front 64x64+0+64 -p! -gB4 -gu8 >> $OUTPUT
cropgrit $INPUT ${ID}_sprite_female_back 64x64+128+64 -p! -gB4 -gu8 >> $OUTPUT
