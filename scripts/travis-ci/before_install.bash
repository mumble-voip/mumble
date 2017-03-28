#!/bin/bash -ex
#
# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

MUMBLE_HOST_DEB=${MUMBLE_HOST/_/-}

if [ "${TRAVIS_OS_NAME}" == "linux" ]; then
	if [ "${MUMBLE_QT}" == "qt4" ] && [ "${MUMBLE_HOST}" == "x86_64-linux-gnu" ]; then
		sudo apt-get -qq update
		sudo apt-get build-dep -qq mumble
	elif [ "${MUMBLE_QT}" == "qt5" ] && [ "${MUMBLE_HOST}" == "x86_64-linux-gnu" ]; then
		sudo apt-get -qq update
		sudo apt-get build-dep -qq mumble
		sudo apt-get install qt5-default qttools5-dev qttools5-dev-tools qtbase5-dev qtbase5-dev-tools qttranslations5-l10n libqt5svg5-dev
	elif [ "${MUMBLE_QT}" == "qt5" ] && [ "${MUMBLE_HOST}" == "i686-w64-mingw32" ]; then
		sudo dpkg --add-architecture i386
		sudo apt-get -qq update
		echo "deb http://pkg.mxe.cc/repos/apt/debian jessie main" | sudo tee /etc/apt/sources.list.d/mxeapt.list
		sudo apt-key adv --keyserver x-hkp://keys.gnupg.net --recv-keys D43A795B73B16ABE9643FE1AFD8FFF16DB45C6AB
		sudo apt-get -qq update
		sudo apt-get install \
			wine \
			mxe-${MUMBLE_HOST_DEB}.static-qtbase \
			mxe-${MUMBLE_HOST_DEB}.static-qtsvg \
			mxe-${MUMBLE_HOST_DEB}.static-qttools \
			mxe-${MUMBLE_HOST_DEB}.static-qttranslations \
			mxe-${MUMBLE_HOST_DEB}.static-boost \
			mxe-${MUMBLE_HOST_DEB}.static-protobuf \
			mxe-${MUMBLE_HOST_DEB}.static-sqlite \
			mxe-${MUMBLE_HOST_DEB}.static-flac \
			mxe-${MUMBLE_HOST_DEB}.static-ogg \
			mxe-${MUMBLE_HOST_DEB}.static-vorbis \
			mxe-${MUMBLE_HOST_DEB}.static-libsndfile
	elif [ "${MUMBLE_QT}" == "qt5" ] && [ "${MUMBLE_HOST}" == "x86_64-w64-mingw32" ]; then
		sudo dpkg --add-architecture i386
		sudo apt-get -qq update
		echo "deb http://pkg.mxe.cc/repos/apt/debian jessie	 main" | sudo tee /etc/apt/sources.list.d/mxeapt.list
		sudo apt-key adv --keyserver x-hkp://keys.gnupg.net --recv-keys D43A795B73B16ABE9643FE1AFD8FFF16DB45C6AB
		sudo apt-get -qq update
		sudo apt-get install \
			wine \
			mxe-${MUMBLE_HOST_DEB}.static-qtbase \
			mxe-${MUMBLE_HOST_DEB}.static-qtsvg \
			mxe-${MUMBLE_HOST_DEB}.static-qttools \
			mxe-${MUMBLE_HOST_DEB}.static-qttranslations \
			mxe-${MUMBLE_HOST_DEB}.static-boost \
			mxe-${MUMBLE_HOST_DEB}.static-protobuf \
			mxe-${MUMBLE_HOST_DEB}.static-sqlite \
			mxe-${MUMBLE_HOST_DEB}.static-flac \
			mxe-${MUMBLE_HOST_DEB}.static-ogg \
			mxe-${MUMBLE_HOST_DEB}.static-vorbis \
			mxe-${MUMBLE_HOST_DEB}.static-libsndfile
	else
		exit 1
	fi
elif [ "${TRAVIS_OS_NAME}" == "osx" ]; then
	brew install qt5 libogg libvorbis flac libsndfile protobuf openssl ice
else
	exit 1
fi
