#!/usr/bin/env bash
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set -e

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

# Get sizes in bytes
fromSize=$(xz --robot --list "$fromFile" | tail -n -1 | cut -f 4)
toSize=$(xz --robot --list "$fromFile" | tail -n -1 | cut -f 5)
# Convert sizes to KB
toSizeKB=$(expr "$toSize" / 1000)
fromSizeKB=$(expr "$fromSize" / 1000)

echo "Compressed size:   $fromSizeKB KB"
echo "Uncompressed size: $toSizeKB KB"

steps=100
checkPointStep=$(expr "$toSizeKB" / "$steps" )

echo ""

# Use gtar instead of tar if available (for MacOS compatibility)
tarExec="tar"
if [ -x "$(command -v gtar)" ]; then
	tarExec="gtar"
fi

"$tarExec" -x --record-size=1K --checkpoint="$checkPointStep" --checkpoint-action="echo=%u / $toSize" -f "$fromFile" -C "$targetDir"
