#!/bin/bash -ex
#
# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

if [ "${TRAVIS_OS_NAME}" == "linux" ]; then
	if [ "${MUMBLE_QT}" == "qt4" ]; then
		qmake-qt4 CONFIG+="release tests g15-emulator qt4-legacy-compat" -recursive && make -j2 && make check
	elif [ "${MUMBLE_QT}" == "qt5" ]; then
		qmake CONFIG+="release tests g15-emulator" -recursive && make -j2 && make check
	else
		exit 1
	fi
else
	exit 1
fi
