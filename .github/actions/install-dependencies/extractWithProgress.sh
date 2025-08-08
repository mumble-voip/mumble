#!/usr/bin/env bash
#
# Copyright 2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set -e
set -x

fromFile="$1"
targetDir="$2"

if [ -z "$fromFile" ]; then
	echo "[ERROR]: Missing argument"
	exit 1
fi

if [ -z "$targetDir" ]; then
	targetDir="."
fi

echo "Extracting from \"$fromFile\" to \"$targetDir\""
echo ""

# Use gtar instead of tar if available (for MacOS compatibility)
tarExec="tar"
if [ -x "$(command -v gtar)" ]; then
	tarExec="gtar"
fi

if [[ "$fromFile" = *.7z || "$fromFile"  = *.zip ]]; then
	extract_cmd=( 7z x "$fromFile" -o"$targetDir" -spe )

	summary="$( 7z l "$fromFile" | tail -n 1 )"
	fromSize="$( echo "$summary" | tr -s ' ' | cut -d ' ' -f 4 )"
	toSize="$( echo "$summary" | tr -s ' ' | cut -d ' ' -f 3 )"
else
	# Get sizes in bytes
	fromSize=$(xz --robot --list "$fromFile" | tail -n -1 | cut -f 4)
	toSize=$(xz --robot --list "$fromFile" | tail -n -1 | cut -f 5)

	steps=100
	checkPointStep=$(expr "$toSizeKB" / "$steps" )

	extract_cmd=( "$tarExec" -x --record-size=1K --checkpoint="$checkPointStep" --checkpoint-action="echo=%u / $toSize" -f "$fromFile" -C "$targetDir" )
fi

# Convert sizes to KB
toSizeKB=$(expr "$toSize" / 1000)
fromSizeKB=$(expr "$fromSize" / 1000)

echo "Compressed size:   $fromSizeKB KB"
echo "Uncompressed size: $toSizeKB KB"


echo ""

"${extract_cmd[@]}"

