// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_G15_HELPER_H_
#define MUMBLE_G15_HELPER_H_

#define G15_MAX_DEV 5
#define G15_MAX_WIDTH 160
#define G15_MAX_HEIGHT 43
#define G15_MAX_BPP 1
#define G15_MAX_FBMEM (G15_MAX_WIDTH * G15_MAX_HEIGHT * G15_MAX_BPP)
#define G15_MAX_FBMEM_BITS (G15_MAX_FBMEM / 8)
#if defined(WIN32)
#	define G15_WIDGET_NAME L"Mumble G15 Display"
#elif defined(APPLE)
#	define G15_WIDGET_NAME CFSTR("Mumble G15 Display")
#endif

enum {
	G15_ERR_INIT = 1,
	G15_ERR_CONNECT,
	G15_ERR_ENUMERATE,
	G15_ERR_OPEN,
	G15_ERR_READFILE,
	G15_ERR_CLOSE,
	G15_ERR_DISCONNECT,
	G15_ERR_DEINIT,
};

#endif
