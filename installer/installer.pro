# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

TEMPLATE = aux

MUMBLE_ARCH = $$QMAKE_TARGET.arch
equals(MUMBLE_ARCH, x86) {
	WIX_ARCH = x86
}
equals(MUMBLE_ARCH, x86_64) {
	WIX_ARCH = x64
}

mumurini.target = murmurini
murmurini.commands = cd ..\scripts && mkini-win32.bat

wix.target = wix
wix.commands = msbuild /p:Configuration=Release;Platform=$$WIX_ARCH MumbleInstall.sln /t:Clean,Build

baketranslations.target = baketranslations
baketranslations.commands = perl build_installer.pl

installer.target = installer
installer.depends = wix baketranslations

QMAKE_EXTRA_TARGETS = murmurini wix baketranslations installer
