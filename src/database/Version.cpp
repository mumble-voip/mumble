// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Version.h"

namespace mumble {
namespace db {

	Version::Version(unsigned int majorVersion, unsigned int minorVersion, unsigned int patchVersion)
		: m_major(majorVersion), m_minor(minorVersion), m_patch(patchVersion) {}

	std::string Version::toString() const {
		return std::to_string(m_major) + "." + std::to_string(m_minor) + "." + std::to_string(m_patch);
	}

	bool operator==(const Version &lhs, const Version &rhs) {
		return lhs.m_major == rhs.m_major && lhs.m_minor == rhs.m_minor && lhs.m_patch == rhs.m_patch;
	}

	bool operator!=(const Version &lhs, const Version &rhs) { return !(lhs == rhs); }

	bool operator<(const Version &lhs, const Version &rhs) {
		if (lhs.m_major != rhs.m_major) {
			return lhs.m_major < rhs.m_major;
		}
		if (lhs.m_minor != rhs.m_minor) {
			return lhs.m_minor < rhs.m_minor;
		}

		return lhs.m_patch < rhs.m_patch;
	}

	bool operator<=(const Version &lhs, const Version &rhs) { return lhs == rhs || lhs < rhs; }

	bool operator>(const Version &lhs, const Version &rhs) { return lhs != rhs && !(lhs < rhs); }

	bool operator>=(const Version &lhs, const Version &rhs) { return lhs == rhs || !(lhs < rhs); }

} // namespace db
} // namespace mumble
