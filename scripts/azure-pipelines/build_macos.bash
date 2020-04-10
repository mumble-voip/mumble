#!/bin/bash -ex
#
# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/opt/openssl/lib/pkgconfig
export PATH=$PATH:/usr/local/opt/qt5/bin:/usr/local/bin
export MUMBLE_PREFIX=/usr/local
export MUMBLE_ICE_PREFIX=/usr/local/opt/ice

ver=$(python scripts/mumble-version.py)

qmake -recursive CONFIG+="release tests warnings-as-errors static" DEFINES+="MUMBLE_VERSION=${ver}"

make -j $(sysctl -n hw.ncpu)
make check

ls -al release/Mumble.app/Contents

relink_shared_libs() {
	targetDir=release/Mumble.app/Contents/MacOS

	for currentLib in "$@"; do
		echo Processing "$currentLib"
		# First copy the library to the same dir as the executable
		cp "$currentLib" $targetDir

		install_name_tool -change "$currentLib" "@executable_path/$(basename $currentLib)" "$targetDir/Mumble"
	done
}

export -f relink_shared_libs

# print out the shared library the Mumble executable depends on
otool -L release/Mumble.app/Contents/MacOS/Mumble | tail -n +2 | grep --invert-match "/System\|/usr/lib" | grep ".dylib" | awk '{print $1}' | xargs bash -c 'relink_shared_libs "$@"'


otool -L release/Mumble.app/Contents/MacOS/Mumble

# Build installer
./macx/scripts/osxdist.py --version=${ver}

mv release/*.dmg ${BUILD_ARTIFACTSTAGINGDIRECTORY}
