#!/usr/bin/env bash
set -e

OUTPUT_FILE="$TRAVIS_BUILD_DIR/roms/BPRE0.gba.gz"
DOWNLOAD_PATH="/community hack/roms/BPRE0.gba.gz"
API_DOWNLOAD_URL="https://content.dropboxapi.com/2/files/download"

curl -X POST --globoff -o "$OUTPUT_FILE" \
     --header "Authorization: Bearer $DROPBOX_OAUTH_ACCESS_TOKEN" \
     --header "Dropbox-API-Arg: {\"path\": \"$DOWNLOAD_PATH\"}" "$API_DOWNLOAD_URL"

gunzip $OUTPUT_FILE
