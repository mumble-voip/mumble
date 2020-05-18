#!/bin/bash -ex
#
# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

ver=$(python scripts/mumble-version.py)

# Use the install Qt version
QT_BASE_DIR=/opt/qt512
export QTDIR=$QT_BASE_DIR
export PATH=$QT_BASE_DIR/bin:$PATH
if [[ $(uname -m) == "x86_64" ]]; then
  export LD_LIBRARY_PATH=$QT_BASE_DIR/lib/x86_64-linux-gnu:$QT_BASE_DIR/lib:$LD_LIBRARY_PATH
else
  export LD_LIBRARY_PATH=$QT_BASE_DIR/lib/i386-linux-gnu:$QT_BASE_DIR/lib:$LD_LIBRARY_PATH
fi
export PKG_CONFIG_PATH=$QT_BASE_DIR/lib/pkgconfig:$PKG_CONFIG_PATH

# clone and install AppImageUpdaterBridge to the system
git clone https://github.com/antony-jr/AppImageUpdaterBridge
cd AppImageUpdaterBridge
git checkout "v1.1.8" 
cmake -DLOGGING_DISABLED=ON .
make -j $(nproc)
sudo make install
cd ..
rm -rf AppImageUpdaterBridge # cleanup

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
qmake -recursive CONFIG+="release tests warnings-as-errors APPIMAGE_UPDATER_BRIDGE_ENABLED" DEFINES+="MUMBLE_VERSION=${ver}"

make -j $(nproc)
make check

# TODO: The next few lines should be done by "make install": https://github.com/mumble-voip/mumble/issues/1029
mkdir -p appdir/usr/bin appdir/usr/lib/mumble appdir/usr/share/metainfo/ appdir/usr/share/icons/hicolor/scalable/apps/ appdir/usr/share/applications/
cp release/lib* appdir/usr/lib/
cp release/mumble appdir/usr/bin
cp release/plugins/lib* appdir/usr/lib/mumble/
cp scripts/mumble.desktop appdir/usr/share/applications/
cp scripts/mumble.appdata.xml appdir/usr/share/metainfo/
cp icons/mumble.svg appdir/usr/share/icons/hicolor/scalable/apps/

APPDIR=$(find $HOME -type d -name 'appdir'| head -n 1)
wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage
./linuxdeployqt-continuous-x86_64.AppImage $(find $HOME -type d -name 'appdir'| head -n 1)/usr/share/applications/*.desktop -appimage -extra-plugins=sqldrivers/libqsqlite.so -updateinformation="zsync|https://dl.mumble.info/snapshot/latest-x86_64.AppImage.zsync"

mv Mumble*.AppImage* ${BUILD_ARTIFACTSTAGINGDIRECTORY}
