#!/bin/bash -ex
#
# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

MUMBLE_HOST_DEB=${MUMBLE_HOST/_/-}

if [ "${TRAVIS_OS_NAME}" == "linux" ]; then
	# Bump the apt http timeout to 120 seconds.
	# Without this, we'd regularly get timeout errors
	# from our MXE mirror on dl.mumble.info.
	#
	# See mumble-voip/mumble#3312 for more information.
	echo 'Acquire::http::Timeout "120";' | sudo tee /etc/apt/apt.conf.d/99zzztimeout

	if [ "${MUMBLE_HOST}" == "x86_64-linux-gnu" ]; then
		sudo apt-get -qq update
		sudo apt-get build-dep -qq mumble
		sudo apt-get install libqt5sql5 libqt5sql5-sqlite qt5-default qttools5-dev qttools5-dev-tools qtbase5-dev qtbase5-dev-tools qttranslations5-l10n libqt5svg5-dev
		sudo apt-get install libjack-jackd2-dev
	elif [ "${MUMBLE_HOST}" == "aarch64-linux-gnu" ]; then
		sudo apt-get -qq update
		sudo apt-get -y install build-essential pkg-config qt5-default qttools5-dev-tools libqt5svg5-dev \
                                libboost-dev libssl-dev libprotobuf-dev protobuf-compiler \
                                libcap-dev libxi-dev \
                                libjack-jackd2-dev libasound2-dev libpulse-dev \
                                libogg-dev libsndfile1-dev libspeechd-dev \
                                libavahi-compat-libdnssd-dev libzeroc-ice-dev libg15daemon-client-dev
	elif [ "${MUMBLE_HOST}" == "i686-w64-mingw32" ]; then
		sudo dpkg --add-architecture i386
		sudo apt-get -qq update
		echo "deb https://dl.mumble.info/mirror/mirror.mxe.cc/repos/apt xenial main" | sudo tee /etc/apt/sources.list.d/mxe.list
		sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 86B72ED9
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
	elif [ "${MUMBLE_HOST}" == "x86_64-w64-mingw32" ]; then
		sudo dpkg --add-architecture i386
		sudo apt-get -qq update
		echo "deb https://dl.mumble.info/mirror/mirror.mxe.cc/repos/apt xenial main" | sudo tee /etc/apt/sources.list.d/mxe.list
		sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 86B72ED9
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
else
	exit 1
fi
