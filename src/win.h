// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// This header is meant to be included instead of <windows.h>.
// It takes care of including <windows.h> with the right settings and
// for MinGW compilers it defines stuff that is missing in MinGW’s headers.

#ifndef MUMBLE_WIN_H_
#define MUMBLE_WIN_H_

#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif

#ifndef UNICODE
#	define UNICODE
#endif

#ifndef NOMINMAX
#	define NOMINMAX
#endif

#ifdef __MINGW32__
// Our MinGW build targets Windows 7 for now.
// Set up the appropriate Windows macros such that
// MinGW's Windows headers expose all the functionality
// we need.
// This seems to be required only with MXE, with MSYS2
// the target is already Windows 7
#	if (_WIN32_WINNT < 0x0601)
#		undef _WIN32_WINNT
#		define _WIN32_WINNT 0x0601
#	endif
#	if (NTDDI_VERSION < NTDDI_WIN7)
#		undef NTDDI_VERSION
#		define NTDDI_VERSION NTDDI_WIN7
#	endif
#endif

#include <windows.h>

#ifdef __MINGW32__
// MinGW's <qos2.h> header does not provide everything we need,
// so define QOS_FLOWID (and PQOS_FLOWID) as well as QOS_NON_ADAPTIVE_FLOW
// ourselves to allow us to build with QoS support on MinGW.
typedef UINT32 QOS_FLOWID, *PQOS_FLOWID;
#	ifndef QOS_NON_ADAPTIVE_FLOW
#		define QOS_NON_ADAPTIVE_FLOW 0x00000002
#	endif
#endif

#endif // MUMBLE_WIN_H_
