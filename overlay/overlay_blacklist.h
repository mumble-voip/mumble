/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _OVERLAY_BLACKLIST_H
#define _OVERLAY_BLACKLIST_H

static const char *overlayBlacklist[] = {
	"iexplore.exe",
	"ieuser.exe",
	"vlc.exe",
	"crimecraft.exe",
	"dbgview.exe",
	"opera.exe",
	"chrome.exe",
	"acrord32.exe",
	"explorer.exe",
	"wmpnscfg.exe",
	"firefox.exe",
	"thunderbird.exe",
	"instantbird.exe",
	"wlmail.exe",   // Windows Live Suite (mshtml.dll)
	"msnmsgr.exe",
	"MovieMaker.exe",
	"WLXPhotoGallery.exe",
	"psi.exe", // Secunia PSI (uses mshtml.dll)
	"Photoshop.exe",
	"blender.exe",
	"googleearth.exe",
	"XBMC.exe", // http://xbmc.org/
	"BOXEE.exe", // http://www.boxee.tv/
	"hammer.exe", // VALVE Hammer Editor
	"hlmv.exe", // Half-Life Model Viewer
	"hlfaceposer.exe", // Face Poser (from Source SDK)
	"LoLLauncher.exe", // League of Legends Launcher/Patcher
	"acrobat.exe", // Adobe Acrobat
	"Steam.exe", // Prevent invisible hooking
	"RzSynapse.exe", // Prevent invisible hooking - Razer Synapse (settings online synchronization)
	"IpOverUsbSvc.exe", // Windows Phone IP over USB Transport
	NULL
};

#endif  // _OVERLAY_BLACKLIST_H

