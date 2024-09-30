// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_OVERLAY_BLACKLIST_H_
#define MUMBLE_OVERLAY_BLACKLIST_H_

static const char *overlayBlacklist[] = {
	"iexplore.exe", "ieuser.exe", "vlc.exe", "crimecraft.exe", "dbgview.exe", "opera.exe", "chrome.exe", "acrord32.exe",
	"explorer.exe", "sihost.exe", "ApplicationFrameHost.exe", "dllhost.exe", "wmpnscfg.exe", "firefox.exe",
	"thunderbird.exe", "instantbird.exe",
	"wlmail.exe", // Windows Live Suite (mshtml.dll)
	"msnmsgr.exe", "MovieMaker.exe", "WLXPhotoGallery.exe",
	"psi.exe", // Secunia PSI (uses mshtml.dll)
	"Photoshop.exe", "blender.exe", "googleearth.exe",
	"XBMC.exe",              // http://xbmc.org/
	"kodi.exe",              // https://kodi.tv/
	"BOXEE.exe",             // http://www.boxee.tv/
	"hammer.exe",            // VALVE Hammer Editor
	"hlmv.exe",              // Half-Life Model Viewer
	"hlfaceposer.exe",       // Face Poser (from Source SDK)
	"LoLLauncher.exe",       // League of Legends Launcher/Patcher
	"acrobat.exe",           // Adobe Acrobat
	"Steam.exe",             // Prevent invisible hooking
	"RzSynapse.exe",         // Prevent invisible hooking - Razer Synapse (settings online synchronization)
	"IpOverUsbSvc.exe",      // Windows Phone IP over USB Transport
	"Origin.exe",            // EA Origin
	"HydraSysTray.exe",      // Razer Hydra system tray
	"devenv.exe",            // Microsoft Visual Studio
	"spotify.exe",           // Spotify
	"EpicGamesLauncher.exe", // Epic, Unreal Tournament launcher
	"dwm.exe",               // Windows Desktop Window Manager
	"MouseKeyboardCenter.exe", "RadeonSettings.exe",
	"mstsc.exe", // Microsoft Remote Desktop Connection
	"iCloudServices.exe", "BethesdaNetLauncher.exe", "GTAVLauncher.exe",

	// Microsoft Office
	"outlook.exe",  // Outlook
	"winword.exe",  // Word
	"excel.exe",    // Excel
	"powerpnt.exe", // PowerPoint
	"onenote.exe",  // OneNote
	"mspub.exe",    // Publisher
	"msaccess.exe", // Access

	nullptr
};

#endif
