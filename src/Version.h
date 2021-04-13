// Copyright 2008-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_VERSION_H_
#define MUMBLE_VERSION_H_

#include <QtCore/QString>

#define MUMXTEXT(X) #X
#define MUMTEXT(X) MUMXTEXT(X)

#ifndef MUMBLE_RELEASE_ID
#	define MUMBLE_RELEASE "Compiled by User"
#else
#	define MUMBLE_RELEASE MUMTEXT(MUMBLE_RELEASE_ID)
#endif

class MumbleVersion {
public:
	static unsigned int getRaw(const QString &version = QLatin1String(MUMTEXT(MUMBLE_VERSION)));
	static QString toString(unsigned int version);
	static bool get(int *major, int *minor, int *patch,
					const QString &version = QLatin1String(MUMTEXT(MUMBLE_VERSION)));

	static unsigned int toRaw(int major, int minor, int patch);
	static void fromRaw(unsigned int version, int *major, int *minor, int *patch);
};

#endif
