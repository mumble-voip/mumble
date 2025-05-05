// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SharedMemory.h"

unsigned int SharedMemory2::uiIndex = 0;

QString SharedMemory2::name() const {
	return a_ucData ? qsName : QString();
}

unsigned int SharedMemory2::size() const {
	return a_ucData ? uiSize : 0;
}

unsigned char *SharedMemory2::data() {
	return a_ucData;
}

const unsigned char *SharedMemory2::data() const {
	return a_ucData;
}

void SharedMemory2::erase() {
	if (a_ucData) {
		memset(a_ucData, 0, uiSize);
	}
}
