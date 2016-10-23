#!/usr/bin/env bash
set -e

UPLOAD_FILE="$TRAVIS_BUILD_DIR/build/patch.ups"
UPLOAD_PATH="/community hack/patches/latest.ups"
API_UPLOAD_URL="https://content.dropboxapi.com/2/files/upload"

curl -X POST --globoff \
     --header "Authorization: Bearer $DROPBOX_OAUTH_ACCESS_TOKEN" \
     --header 'Content-Type: application/octet-stream' \
     --header "Dropbox-API-Arg: {\"path\": \"$UPLOAD_PATH\", \"mode\": {\".tag\": \"overwrite\"}, \"autorename\": false}" --data-binary @"$UPLOAD_FILE" "$API_UPLOAD_URL"
