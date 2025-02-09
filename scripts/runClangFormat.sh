#!/bin/sh
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

git ls-files --cached --modified --others -z -- \
    ':^3rdparty/' ':^build*' '**/*'.cpp '**/*'.c '**/*'.hpp '**/*'.h '**/*'.cxx '**/*'.cc  |
	xargs -0 -r -- clang-format --style=file -i
