// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_VERSION_H_
#define MUMBLE_VERSION_H_

#include <QtCore/QString>

#define MUMXTEXT(X) #X
#define MUMTEXT(X) MUMXTEXT(X)

#ifndef MUMBLE_VERSION
#define MUMBLE_RELEASE "Compiled by User"
#else
#define MUMBLE_RELEASE MUMTEXT(MUMBLE_VERSION)
#endif

class MumbleVersion {
	public:
		static unsigned int getRaw(const QString &version = QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)));
		static QString toString(unsigned int version);
		static bool get(int *major, int *minor, int *patch, const QString &version = QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)));

		static unsigned int toRaw(int major, int minor, int patch);
		static void fromRaw(unsigned int version, int *major, int *minor, int *patch);
};

#endif
