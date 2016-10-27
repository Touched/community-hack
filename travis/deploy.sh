#!/usr/bin/env bash
set -e

PATCHES=("ups" "ppf")
API_UPLOAD_URL="https://content.dropboxapi.com/2/files/upload"

function upload {
    echo curl -X POST --globoff \
         --header "Authorization: Bearer $DROPBOX_OAUTH_ACCESS_TOKEN" \
         --header 'Content-Type: application/octet-stream' \
         --header "Dropbox-API-Arg: {\"path\": \"$2\", \"mode\": {\".tag\": \"overwrite\"}, \"autorename\": false}" --data-binary @"$1" "$API_UPLOAD_URL"
}

if [ -n "$TRAVIS_BRANCH" ]
then
    for ext in "${PATCHES[@]}"
    do
        upload "$TRAVIS_BUILD_DIR/build/patch.$ext" "/community hack/patches/$TRAVIS_BRANCH.$ext"
    done
fi
