// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_QATOMICINTCOMPAT_H_
#define MUMBLE_QATOMICINTCOMPAT_H_

// Portably (between Qt 4 and Qt 5) load the value
// of a QAtomicInt.
inline int QAtomicIntLoad(const QAtomicInt &ai) {
#if QT_VERSION >= 0x050000
	return ai.load();
#else
	return int(ai);
#endif
}

#endif
