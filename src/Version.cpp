// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Version.h"

#include <QtCore/QRegExp>

unsigned int MumbleVersion::getRaw(const QString &version) {
	int major, minor, patch;

	if (get(&major, &minor, &patch, version))
		return toRaw(major, minor, patch);

	return 0;
}

QString MumbleVersion::toString(unsigned int v) {
	int major, minor, patch;
	fromRaw(v, &major, &minor, &patch);
	return QString::fromLatin1("%1.%2.%3").arg(major).arg(minor).arg(patch);
}

bool MumbleVersion::get(int *major, int *minor, int *patch, const QString &version) {
	QRegExp rx(QLatin1String("(\\d+)\\.(\\d+)\\.(\\d+)"));

	if (rx.exactMatch(version)) {
		if (major)
			*major = rx.cap(1).toInt();
		if (minor)
			*minor = rx.cap(2).toInt();
		if (patch)
			*patch = rx.cap(3).toInt();

		return true;
	}
	return false;
}

unsigned int MumbleVersion::toRaw(int major, int minor, int patch) {
	return (major << 16) | (minor << 8) | patch;
}

void MumbleVersion::fromRaw(unsigned int version, int *major, int *minor, int *patch) {
	*major = (version & 0xFFFF0000) >> 16;
	*minor = (version & 0xFF00) >> 8;
	*patch = (version & 0xFF);
}
