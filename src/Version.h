// Copyright 2008-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_VERSION_H_
#define MUMBLE_VERSION_H_

#include <cstdint>

#include <QtCore/QString>

#define MUMXTEXT(X) #X
#define MUMTEXT(X) MUMXTEXT(X)

#define MUMBLE_RELEASE MUMTEXT(MUMBLE_VERSION)

namespace Version {

using exact_width_mumble_raw_version_t = std::uint32_t;
using mumble_raw_version_t             = unsigned int;

static_assert(sizeof(exact_width_mumble_raw_version_t) <= sizeof(mumble_raw_version_t),
			  "Invalid scalar type used to represent Mumble versions");

constexpr mumble_raw_version_t UNKNOWN = 0;

unsigned int getRaw(const QString &version = QLatin1String(MUMTEXT(MUMBLE_VERSION)));
QString toString(mumble_raw_version_t version);
bool get(int *major, int *minor, int *patch, const QString &version = QLatin1String(MUMTEXT(MUMBLE_VERSION)));

constexpr Version::mumble_raw_version_t toRaw(int major, int minor, int patch) {
	return (major << 16) | (minor << 8) | patch;
}

constexpr void fromRaw(unsigned int version, int *major, int *minor, int *patch) {
	*major = (version & 0xFFFF0000) >> 16;
	*minor = (version & 0xFF00) >> 8;
	*patch = (version & 0xFF);
}

}; // namespace Version

#endif
