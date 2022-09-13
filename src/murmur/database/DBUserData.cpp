// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBUserData.h"

#include <chrono>

namespace mumble {
namespace server {
	namespace db {

		bool operator==(const DBUserData::PasswordData &lhs, const DBUserData::PasswordData &rhs) {
			return lhs.passwordHash == rhs.passwordHash && lhs.salt == rhs.salt
				   && lhs.kdfIterations == rhs.kdfIterations;
		}

		bool operator!=(const DBUserData::PasswordData &lhs, const DBUserData::PasswordData &rhs) {
			return !(lhs == rhs);
		}

		template< typename TimePoint > std::size_t toSeconds(const TimePoint &tp) {
			return std::chrono::duration_cast< std::chrono::seconds >(tp.time_since_epoch()).count();
		}

		bool operator==(const DBUserData &lhs, const DBUserData &rhs) {
			return lhs.name == rhs.name && lhs.password == rhs.password && lhs.lastChannelID == rhs.lastChannelID
				   && lhs.texture == rhs.texture && toSeconds(lhs.lastActive) == toSeconds(rhs.lastActive)
				   && toSeconds(lhs.lastDisconnect) == toSeconds(rhs.lastDisconnect);
		}

		bool operator!=(const DBUserData &lhs, const DBUserData &rhs) { return !(lhs == rhs); }

	} // namespace db
} // namespace server
} // namespace mumble
