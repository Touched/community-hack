#!/usr/bin/env bash
set -e

PATCHES=("ups" "ppf")
API_UPLOAD_URL="https://content.dropboxapi.com/2/files/upload"

function upload {
    timestamp=$(date -u +"%Y-%m-%dT%H:%M:%SZ")
    curl -X POST --globoff \
         --header "Authorization: Bearer $DROPBOX_OAUTH_ACCESS_TOKEN" \
         --header 'Content-Type: application/octet-stream' \
         --header "Dropbox-API-Arg: {\"path\": \"$2\", \"mode\": {\".tag\": \"overwrite\"}, \"autorename\": false, \"client_modified\": \"$timestamp\"}" --data-binary @"$1" "$API_UPLOAD_URL"
}

if [ "$TRAVIS_PULL_REQUEST" = "false" ]
then
    if [ -n "$TRAVIS_BRANCH" ]
    then
        for ext in "${PATCHES[@]}"
        do
            upload "$TRAVIS_BUILD_DIR/build/patch.$ext" "/community hack/patches/$TRAVIS_BRANCH.$ext"
        done
    fi
fi
