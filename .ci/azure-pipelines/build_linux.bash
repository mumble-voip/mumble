#!/bin/bash -ex
#
# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# Below is a list of configuration variables used from environment.
#
#  BUILD_BINARIESDIRECTORY - The local path on the agent that can be used
#                            as an output folder for compiled binaries.
#  BUILD_SOURCESDIRECTORY  - The local path on the agent where the
#                            repository is downloaded.
#

VER=$(python scripts/mumble-version.py)

cd $BUILD_BINARIESDIRECTORY

# QSslDiffieHellmanParameters was introduced in Qt 5.8, Ubuntu 16.04 has 5.5.
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=appdir/usr -Dtests=ON Donline-tests=ON -Dversion=$VER -Dsymbols=ON \
	-Dqssldiffiehellmanparameters=OFF -Donline-tests=ON $BUILD_SOURCESDIRECTORY

cmake --build .
ctest --verbose
cmake --install .

# Get the AppImage-tool - always use the latest available version
wget -c https://github.com/$(wget -q https://github.com/probonopd/go-appimage/releases -O - | grep "appimagetool-.*-x86_64.AppImage" | head -n 1 | cut -d '"' -f 2)
chmod +x appimagetool-*.AppImage

# find the appdir that was created by cmake --install
appDir=$(find $HOME -type d -name 'appdir'| head -n 1)

# Bundle the AppDir
./appimagetool-*.AppImage -s deploy $appDir/usr/share/applications/*.desktop # Bundle EVERYTHING

# Actually create the AppImage
VERSION="$VER" ARCH="x86_64" ./appimagetool-*.AppImage $appDir

for f in Mumble*.AppImage; do
	# Embed update information into AppImage
	echo -n "zsync|https://dl.mumble.info/nightly/latest-x86_64.AppImage.zsync" | dd of=$f seek=175720 conv=notrunc oflag=seek_bytes

	# Generate zsync file for AppImage
	zsyncmake $f
done

mv Mumble*.AppImage* $BUILD_ARTIFACTSTAGINGDIRECTORY
