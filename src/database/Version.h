// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_VERSION_H_
#define MUMBLE_DATABASE_VERSION_H_

#include <string>

namespace mumble {
namespace db {

	struct Version {
		Version(unsigned int majorVersion = 0, unsigned int minorVersion = 0, unsigned int patchVersion = 0);
		~Version() = default;

		unsigned int m_major = 0;
		unsigned int m_minor = 0;
		unsigned int m_patch = 0;

		std::string toString() const;

		friend bool operator==(const Version &lhs, const Version &rhs);
		friend bool operator!=(const Version &lhs, const Version &rhs);
		friend bool operator<(const Version &lhs, const Version &rhs);
		friend bool operator<=(const Version &lhs, const Version &rhs);
		friend bool operator>(const Version &lhs, const Version &rhs);
		friend bool operator>=(const Version &lhs, const Version &rhs);
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_VERSION_H_
