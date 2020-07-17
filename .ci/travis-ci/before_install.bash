#!/bin/bash -ex
#
# Copyright 2005-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

MUMBLE_HOST_DEB=${MUMBLE_HOST/_/-}

if [ "${TRAVIS_OS_NAME}" == "linux" ]; then
	if [ "${MUMBLE_HOST}" == "aarch64-linux-gnu" ] || [ "${MUMBLE_HOST}" == "x86_64-linux-gnu" ]; then
		sudo apt-get -qq update
		sudo apt-get -y install build-essential ninja-build pkg-config \
                                qt5-default qttools5-dev qttools5-dev-tools libqt5svg5-dev \
                                libboost-dev libssl-dev libprotobuf-dev protobuf-compiler \
                                libcap-dev libxi-dev \
                                libasound2-dev libpulse-dev \
                                libogg-dev libsndfile1-dev libspeechd-dev \
                                libavahi-compat-libdnssd-dev libzeroc-ice-dev
		if [ "${MUMBLE_HOST}" == "aarch64-linux-gnu" ]; then
			# Kitware's APT repository doesn't provide packages for ARM64.
			cd ${HOME}
			if [ -d "cmake-3.17.2" ] && [ "$(ls -A cmake-3.17.2)" ]; then
				# Directory cached, CMake has already been compiled in a previous build.
				cd cmake-3.17.2 && sudo make install
			else
				# Build a recent version of CMake.
				wget https://github.com/Kitware/CMake/releases/download/v3.17.2/cmake-3.17.2.tar.gz && tar -xzvf cmake-3.17.2.tar.gz && cd cmake-3.17.2
				./bootstrap && make -j $(nproc) && sudo make install
			fi
		else
			sudo apt-get -y install g++-multilib
		fi
	elif [ "${MUMBLE_HOST}" == "i686-w64-mingw32" ] || [ "${MUMBLE_HOST}" == "x86_64-w64-mingw32" ]; then
		sudo dpkg --add-architecture i386

		sudo apt-get -qq update
		sudo apt-get -y install ninja-build wine32-development wine-development

		mkdir ${HOME}/mxe && cd ${HOME}/mxe

		# Download lists of MXE packages to install.
		wget https://dl.mumble.info/build/mxe/mxe.deb-list
		wget https://dl.mumble.info/build/mxe/mxe-x86-64-pc-linux-gnu.deb-list
		wget https://dl.mumble.info/build/mxe/mxe-${MUMBLE_HOST_DEB}.static.deb-list

		# Merge the lists into a single one.
		cat *.deb-list > packages.deb-list

		# Download the MXE packages, if not cached.
		while read package; do
			if [ ! -z "$package" ] && [ ! -f "$package" ]; then
				wget https://dl.mumble.info/build/mxe/$package
			fi
		done <packages.deb-list

		# Install the MXE packages.
		sudo dpkg -i *.deb
	else
		exit 1
	fi
else
	exit 1
fi
