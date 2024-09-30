// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_OVERLAY_LAUNCHERS_H_
#define MUMBLE_OVERLAY_LAUNCHERS_H_

static const char *overlayLaunchers[] = {
	"Steam.exe",                 // Steam
	"Origin.exe",                // Origin
	"Battle.net.exe",            // Battle.net
	"GalaxyClient.exe",          // GOG Galaxy
	"UbisoftGameLauncher.exe",   // Uplay
	"UbisoftGameLauncher64.exe", // Uplay
	"itch.exe",                  // itch.io
	"LaunchPad.exe",             // Daybreak Games LaunchPad

	"evelauncher.exe",   // EVE Online launcher
	"ffxivlauncher.exe", // Final Fantasy XIV Launcher
	"LoLPatcher.exe",    // League of Legends Patcher (note: this isn't its launcher, but an intermediate process)
	"GTAVLauncher.exe",  // Grand Theft Auto V

	nullptr
};

#endif
