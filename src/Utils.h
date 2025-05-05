// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// This header file defines common utility macros and values that are generally valid or used across multiple areas
// giving them generic scope (rather than localized or functionality-specific scope).

#ifndef MUMBLE_UTILS_H_
#define MUMBLE_UTILS_H_

#include <QtCore/QtGlobal>

#ifndef Q_OS_WIN
#	ifdef WId
typedef WId HWND;
#	endif
#	define __cdecl
#	define INVALID_SOCKET -1
#	define SOCKET_ERROR -1
#	define CopyMemory(dst, ptr, len) memcpy(dst, ptr, len)
#	define ZeroMemory(ptr, len) memset(ptr, 0, len)
#endif

#endif // MUMBLE_UTILS_H_
