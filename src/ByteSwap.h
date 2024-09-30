// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_BYTESWAP_H_
#define MUMBLE_BYTESWAP_H_

#include <QtCore/QtGlobal>

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
#	define SWAP64(x) (x)
#else
#	if defined(__x86_64__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
#		define SWAP64(x) __builtin_bswap64(x)
#	else
#		include <QtCore/QtEndian>
#		define SWAP64(x) qbswap< quint64 >(x)
#	endif
#endif

#endif
