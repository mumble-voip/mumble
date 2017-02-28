#!/bin/bash -ex
#
# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

if [ "${TRAVIS_OS_NAME}" == "linux" ]; then
	if [ "${MUMBLE_QT}" == "qt4" ]; then
		sudo apt-get -qq update
		sudo apt-get build-dep -qq mumble
	elif [ "${MUMBLE_QT}" == "qt5" ]; then
		sudo apt-get -qq update
		sudo apt-get build-dep -qq mumble
		sudo apt-get install qt5-default qttools5-dev qttools5-dev-tools qtbase5-dev qtbase5-dev-tools qttranslations5-l10n libqt5svg5-dev
	else
		exit 1
	fi
else
	exit 1
fi
