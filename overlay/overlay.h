// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_INTERNAL_OVERLAY_H_
#define MUMBLE_INTERNAL_OVERLAY_H_

// overlay message protocol version number
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

#endif
