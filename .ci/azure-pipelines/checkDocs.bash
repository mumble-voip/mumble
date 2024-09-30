#!/usr/bin/env bash
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set -e

# Get path to the script that generates the cmake options docs
generateScript="$BUILD_SOURCESDIRECTORY/scripts/generate_cmake_options_docs.py"

# Get current content of the docs
currentDocs="$BUILD_SOURCESDIRECTORY/docs/dev/build-instructions/cmake_options.md"

generatedDocs="/tmp/generated_docs.md"

"$generateScript" > "$generatedDocs"

diff -u "$currentDocs" "$generatedDocs"
