#!/bin/bash -ex
#
# Copyright 2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

ver=$(python scripts/mumble-version.py)

qmake -recursive CONFIG+="release tests warnings-as-errors" DEFINES+="MUMBLE_VERSION=${ver}"

make -j $(nproc) && make check

# TODO: The next few lines should be done by "make install": https://github.com/mumble-voip/mumble/issues/1029
mkdir -p appdir/usr/bin appdir/usr/lib/mumble appdir/usr/share/metainfo/ appdir/usr/share/icons/hicolor/scalable/apps/ appdir/usr/share/applications/
cp release/lib* appdir/usr/lib/
cp release/mumble appdir/usr/bin
cp release/plugins/lib* appdir/usr/lib/mumble/
cp scripts/mumble.desktop appdir/usr/share/applications/
cp scripts/mumble.appdata.xml appdir/usr/share/metainfo/
cp icons/mumble.svg appdir/usr/share/icons/hicolor/scalable/apps/
wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage
./linuxdeployqt-continuous-x86_64.AppImage $(find $HOME -type d -name 'appdir'| head -n 1)/usr/share/applications/*.desktop -appimage -extra-plugins=sqldrivers/libqsqlite.so

for f in Mumble*.AppImage; do
	# Embed update information into AppImage
	echo -n "zsync|https://dl.mumble.info/snapshot/latest-x86_64.AppImage.zsync" | dd of=$f seek=175720 conv=notrunc oflag=seek_bytes

	# Generate zsync file for AppImage
	zsyncmake $f
done

mv Mumble*.AppImage* ${BUILD_ARTIFACTSTAGINGDIRECTORY}
