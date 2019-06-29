#!/bin/bash -ex
#
# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

if [ "${TRAVIS_OS_NAME}" == "linux" ]; then
	if [ "${MUMBLE_QT}" == "qt4" ] && [ "${MUMBLE_HOST}" == "x86_64-linux-gnu" ]; then
		EXTRA_CONFIG=
		if [ "${MUMBLE_NO_PCH}" == "1" ]; then
			EXTRA_CONFIG="no-pch ${EXTRA_CONFIG}"
		fi
		qmake-qt4 CONFIG+="release tests g15-emulator qt4-legacy-compat ${EXTRA_CONFIG}" -recursive && make -j2 && make check
	elif [ "${MUMBLE_QT}" == "qt5" ] && [ "${MUMBLE_HOST}" == "x86_64-linux-gnu" ]; then
		EXTRA_CONFIG=PREFIX=/usr
		if [ "${MUMBLE_NO_PCH}" == "1" ]; then
			EXTRA_CONFIG="no-pch ${EXTRA_CONFIG}"
		fi
		qmake CONFIG+="release tests g15-emulator ${EXTRA_CONFIG}" DEFINES+="MUMBLE_VERSION=${TRAVIS_COMMIT:0:7}" -recursive
		if [ "${MUMBLE_NO_PCH}" == "1" ]; then
			# Wraps the compilation with the Build Wrapper to generate the configuration used later by the SonarQube Scanner.
			mkdir build
			build-wrapper-linux-x86-64 --out-dir build/sonar make -j 2
			make check

			# Run the SonarQube analysis
			sonar-scanner
		else
			make -j2 && make check

			# The next few lines should really be done by "make install"; https://github.com/mumble-voip/mumble/issues/1029
			mkdir -p appdir/usr/bin appdir/usr/lib/mumble appdir/usr/share/metainfo/ appdir/usr/share/icons/scalable/apps/ appdir/usr/share/applications/
			cp release/lib* appdir/usr/lib/
			cp release/mumble appdir/usr/bin
			cp release/plugins/lib* appdir/usr/lib/mumble/
			cp scripts/mumble.desktop appdir/usr/share/applications/
			cp scripts/mumble.appdata.xml appdir/usr/share/metainfo/
			cp icons/mumble.svg appdir/usr/share/icons/scalable/apps/
			wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
			chmod a+x linuxdeployqt-continuous-x86_64.AppImage
			./linuxdeployqt-continuous-x86_64.AppImage $(find $HOME -type d -name 'appdir'| head -n 1)/usr/share/applications/*.desktop -appimage -extra-plugins=sqldrivers/libqsqlite.so
			wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
			bash upload.sh Mumble*.AppImage*
		fi
	elif [ "${MUMBLE_QT}" == "qt5" ] && [ "${MUMBLE_HOST}" == "i686-w64-mingw32" ]; then
		wget http://www.steinberg.net/sdk_downloads/asiosdk2.3.zip -P ../
		unzip ../asiosdk2.3.zip -d ../
		mv ../ASIOSDK2.3 3rdparty/asio
		PATH=$PATH:/usr/lib/mxe/usr/bin
		export MUMBLE_PROTOC=/usr/lib/mxe/usr/x86_64-pc-linux-gnu/bin/protoc
		EXTRA_CONFIG=
		if [ "${MUMBLE_NO_PCH}" == "1" ]; then
			EXTRA_CONFIG="no-pch ${EXTRA_CONFIG}"
		fi
		${MUMBLE_HOST}.static-qmake-qt5 -recursive -Wall CONFIG+="release tests warnings-as-errors g15-emulator no-overlay no-bonjour no-elevation no-ice ${EXTRA_CONFIG}"
		make -j2
		make check TESTRUNNER="wine"
	elif [ "${MUMBLE_QT}" == "qt5" ] && [ "${MUMBLE_HOST}" == "x86_64-w64-mingw32" ]; then
		wget http://www.steinberg.net/sdk_downloads/asiosdk2.3.zip -P ../
		unzip ../asiosdk2.3.zip -d ../
		mv ../ASIOSDK2.3 3rdparty/asio
		PATH=$PATH:/usr/lib/mxe/usr/bin
		export MUMBLE_PROTOC=/usr/lib/mxe/usr/x86_64-pc-linux-gnu/bin/protoc
		EXTRA_CONFIG=
		if [ "${MUMBLE_NO_PCH}" == "1" ]; then
			EXTRA_CONFIG="no-pch ${EXTRA_CONFIG}"
		fi
		${MUMBLE_HOST}.static-qmake-qt5 -recursive -Wall CONFIG+="release tests warnings-as-errors g15-emulator no-overlay no-bonjour no-elevation no-ice ${EXTRA_CONFIG}"
		make -j2
		make check TESTRUNNER="wine"
	else
		exit 1
	fi
elif [ "${TRAVIS_OS_NAME}" == "osx" ]; then
	export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
	export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/opt/openssl/lib/pkgconfig
	export PATH=$PATH:/usr/local/opt/qt5/bin:/usr/local/bin
	export MUMBLE_PREFIX=/usr/local
	export MUMBLE_ICE_PREFIX=/usr/local/opt/ice
	qmake CONFIG+="release tests warnings-as-errors" && make -j2 && make check
	./macx/scripts/osxdist.py --no-compat-warning
else
	exit 1
fi
