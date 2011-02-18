/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _INTERNAL_OVERLAY_H
#define _INTERNAL_OVERLAY_H

#define OVERLAY_MAGIC_NUMBER 0x00000005

struct OverlayMsgHeader {
	unsigned int uiMagic;
	int iLength;
	unsigned int uiType;
};

#define OVERLAY_MSGTYPE_INIT 0
struct OverlayMsgInit {
	unsigned int uiWidth;
	unsigned int uiHeight;
};

#define OVERLAY_MSGTYPE_SHMEM 1
struct OverlayMsgShmem {
	char a_cName[2048];
};

#define OVERLAY_MSGTYPE_BLIT 2
struct OverlayMsgBlit {
	unsigned int x, y, w, h;
};

#define OVERLAY_MSGTYPE_ACTIVE 3
struct OverlayMsgActive {
	unsigned int x, y, w, h;
};

#define OVERLAY_MSGTYPE_PID 4
struct OverlayMsgPid {
	unsigned int pid;
};

#define OVERLAY_MSGTYPE_FPS 5
struct OverlayMsgFps {
	float fps;
};
#define OVERLAY_FPS_INTERVAL 0.25f

#define OVERLAY_MSGTYPE_INTERACTIVE 6
struct OverlayMsgInteractive {
	bool state;
};

struct OverlayMsg {
	union {
		char headerbuffer[1];
		struct OverlayMsgHeader omh;
	};
	union {
		char msgbuffer[1];
		struct OverlayMsgShmem oms;
		struct OverlayMsgInit omi;
		struct OverlayMsgBlit omb;
		struct OverlayMsgActive oma;
		struct OverlayMsgPid omp;
		struct OverlayMsgFps omf;
		struct OverlayMsgInteractive omin;
	};
};

static const char *overlayBlacklist[] = {
	"iexplore.exe",
	"ieuser.exe",
	"vlc.exe",
	"dbgview.exe",
	"opera.exe",
	"chrome.exe",
	"acrord32.exe",
	"explorer.exe",
	"wmpnscfg.exe",
	"firefox.exe",
	"wlmail.exe",	// Windows Live Suite (mshtml.dll)
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
	NULL
};

#endif
