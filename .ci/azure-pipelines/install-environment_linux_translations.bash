#!/usr/bin/env bash
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set -e
set -x

sudo apt-get update

sudo apt-get -y install qt6-l10n-tools

sudo ln -s /usr/lib/qt6/bin/lupdate /usr/bin/

lupdate -version
