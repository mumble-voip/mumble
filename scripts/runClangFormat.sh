#!/usr/bin/env bash
#
# Copyright 2020-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

currentDir=$(dirname $0)

mainDir="$currentDir/.."

cd "$mainDir"

find -type f \( -iname "*.cpp" -o -iname "*.c" -o -iname "*.hpp" -o -iname "*.h" -o -iname "*.cxx" -o -iname "*.cc" \) \
	-a -not -ipath "./3rdparty/*" -a -not -path "./.git/*" -a -not -ipath "./build/*" \
	| xargs clang-format --style=file -i
